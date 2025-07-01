import socket
import threading
import time
import os
import sys
import signal
import json
import struct
import hashlib
from datetime import datetime
from pathlib import Path
import queue
import re

# Constants
MAX_CLIENTS = 50
MAX_ROOMS = 20
MAX_ROOM_CAPACITY = 15
MAX_USERNAME_LEN = 16
MAX_ROOMNAME_LEN = 32
MAX_MSG_SIZE = 4096
MAX_CMD_SIZE = 256
MAX_UPLOAD_QUEUE = 5
MAX_FILE_SIZE = 3 * 1024 * 1024  # 3MB
BUFFER_SIZE = 1024
FILE_TRANSFER_BUFFER = 1024

# Valid file extensions
VALID_EXTENSIONS = ['.txt', '.pdf', '.jpg', '.png']

# ANSI color codes
class Colors:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN = '\033[36m'
    RESET = '\033[0m'

class Client:
    def __init__(self, socket, username, ip_address):
        self.socket = socket
        self.username = username
        self.ip_address = ip_address
        self.current_room = ""
        self.connected = True
        self.thread = None
        self.previous_rooms = []

class Room:
    def __init__(self, name):
        self.name = name
        self.clients = []
        self.lock = threading.Lock()
        self.active = True

class FileTransfer:
    def __init__(self, filename, sender, recipient, sender_socket, file_size):
        self.filename = filename
        self.sender = sender
        self.recipient = recipient
        self.sender_socket = sender_socket
        self.file_size = file_size
        self.request_time = time.time()
        self.start_time = 0
        self.in_progress = True

class ChatServer:
    def __init__(self, port):
        self.port = port
        self.server_socket = None
        self.clients = {}  # client_id -> Client
        self.rooms = {}    # room_name -> Room
        self.upload_queue = queue.Queue(maxsize=MAX_UPLOAD_QUEUE)
        self.clients_lock = threading.Lock()
        self.rooms_lock = threading.Lock()
        self.log_lock = threading.Lock()
        self.keep_running = True
        self.log_file = None
        self.client_counter = 0
        
        # Setup signal handler
        signal.signal(signal.SIGINT, self.handle_sigint)
        
        # Setup directories
        self.setup_directories()
        
        # Setup logging
        self.setup_logging()
        
    def setup_directories(self):
        """Create necessary directories"""
        os.makedirs("logs", exist_ok=True)
        os.makedirs("uploads", exist_ok=True)
        os.makedirs("received_files", exist_ok=True)
        
    def setup_logging(self):
        """Setup log file"""
        now = datetime.now()
        log_filename = f"logs/server_log_{now.strftime('%Y-%m-%d_%H-%M-%S')}.log"
        self.log_file = open(log_filename, 'w')
        print(f"Log file: {log_filename}")
        
    def write_log(self, message):
        """Write message to log file with timestamp"""
        if not self.log_file:
            return
            
        with self.log_lock:
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            self.log_file.write(f"{timestamp} - {message}\n")
            self.log_file.flush()
            
    def is_valid_username(self, username):
        """Validate username format"""
        if not username or len(username) > MAX_USERNAME_LEN:
            return False
        return username.isalnum()
        
    def is_valid_roomname(self, roomname):
        """Validate room name format"""
        if not roomname or len(roomname) > MAX_ROOMNAME_LEN:
            return False
        return roomname.isalnum()
        
    def is_valid_file_extension(self, filename):
        """Check if file has valid extension"""
        if '.' not in filename:
            return False
        ext = '.' + filename.split('.')[-1].lower()
        return ext in VALID_EXTENSIONS
        
    def send_message(self, client_socket, message):
        """Send message to client"""
        try:
            client_socket.send(message.encode('utf-8'))
        except:
            pass
            
    def find_client_by_username(self, username):
        """Find client by username"""
        with self.clients_lock:
            for client in self.clients.values():
                if client.username == username:
                    return client
        return None
        
    def add_client(self, client_socket, username, ip_address):
        """Add new client"""
        with self.clients_lock:
            if len(self.clients) >= MAX_CLIENTS:
                return None
                
            self.client_counter += 1
            client_id = self.client_counter
            client = Client(client_socket, username, ip_address)
            self.clients[client_id] = client
            return client_id
            
    def remove_client(self, client_id):
        """Remove client"""
        with self.clients_lock:
            if client_id in self.clients:
                client = self.clients[client_id]
                client.connected = False
                try:
                    client.socket.close()
                except:
                    pass
                del self.clients[client_id]
                
    def create_or_join_room(self, client_id, room_name):
        """Create or join a room"""
        with self.clients_lock:
            if client_id not in self.clients:
                return -2
            client = self.clients[client_id]
            
        with self.rooms_lock:
            if room_name in self.rooms:
                room = self.rooms[room_name]
                with room.lock:
                    if len(room.clients) >= MAX_ROOM_CAPACITY:
                        return -1  # Room full
                    room.clients.append(client)
                    client.current_room = room_name
                    return 0
            else:
                # Create new room
                room = Room(room_name)
                room.clients.append(client)
                client.current_room = room_name
                self.rooms[room_name] = room
                print(f"{Colors.CYAN}[ROOM-CREATE]{Colors.RESET} New room '{room_name}' created by '{client.username}'")
                self.write_log(f"[ROOM] New room '{room_name}' created by user '{client.username}'")
                return 0
                
    def leave_room(self, client_id):
        """Leave current room"""
        with self.clients_lock:
            if client_id not in self.clients:
                return
            client = self.clients[client_id]
            
        if not client.current_room:
            return
            
        room_name = client.current_room
        
        with self.rooms_lock:
            if room_name in self.rooms:
                room = self.rooms[room_name]
                with room.lock:
                    if client in room.clients:
                        room.clients.remove(client)
                        
                    if len(room.clients) == 0:
                        room.active = False
                        del self.rooms[room_name]
                        self.write_log(f"[ROOM] Room '{room_name}' is now empty and removed")
                        
        client.current_room = ""
        
    def broadcast_message(self, client_id, message):
        """Broadcast message to room"""
        with self.clients_lock:
            if client_id not in self.clients:
                return
            sender = self.clients[client_id]
            
        if not sender.current_room:
            return
            
        room_name = sender.current_room
        formatted_msg = f"{Colors.CYAN}[ROOM '{room_name}'] {sender.username}: {message}{Colors.RESET}"
        
        with self.rooms_lock:
            if room_name in self.rooms:
                room = self.rooms[room_name]
                with room.lock:
                    for client in room.clients:
                        if client != sender:
                            self.send_message(client.socket, formatted_msg)
                            
        # Send confirmation to sender
        confirmation = f"{Colors.GREEN}SUCCESS: Message broadcast to room '{room_name}'{Colors.RESET}"
        self.send_message(sender.socket, confirmation)
        
    def whisper_message(self, client_id, recipient_name, message):
        """Send private message"""
        with self.clients_lock:
            if client_id not in self.clients:
                return
            sender = self.clients[client_id]
            
        recipient = self.find_client_by_username(recipient_name)
        if not recipient:
            error_msg = f"{Colors.RED}ERROR: User '{recipient_name}' not found{Colors.RESET}"
            self.send_message(sender.socket, error_msg)
            return
            
        # Send message to recipient
        whisper_msg = f"{Colors.MAGENTA}[WHISPER] from {sender.username}: {message}{Colors.RESET}"
        self.send_message(recipient.socket, whisper_msg)
        
        # Send confirmation to sender
        confirmation = f"{Colors.GREEN}SUCCESS: Message sent to {recipient_name}{Colors.RESET}"
        self.send_message(sender.socket, confirmation)
        
    def has_been_in_room(self, client_id, room_name):
        """Check if client has been in room before"""
        with self.clients_lock:
            if client_id not in self.clients:
                return False
            client = self.clients[client_id]
            return room_name in client.previous_rooms
            
    def add_room_to_history(self, client_id, room_name):
        """Add room to client's history"""
        with self.clients_lock:
            if client_id not in self.clients:
                return
            client = self.clients[client_id]
            if room_name not in client.previous_rooms:
                client.previous_rooms.append(room_name)
                
    def generate_unique_filename(self, directory, filename):
        """Generate unique filename to avoid conflicts"""
        base_path = Path(directory) / filename
        if not base_path.exists():
            return filename
            
        name_part = base_path.stem
        ext_part = base_path.suffix
        
        counter = 1
        while counter <= 999:
            new_filename = f"{name_part}_{counter}{ext_part}"
            new_path = Path(directory) / new_filename
            if not new_path.exists():
                return new_filename
            counter += 1
            
        # Fallback with timestamp
        timestamp = int(time.time())
        return f"{name_part}_{timestamp}{ext_part}"
        
    def file_transfer_worker(self):
        """Worker thread for file transfers"""
        while self.keep_running:
            try:
                transfer = self.upload_queue.get(timeout=1)
                if transfer is None:  # Shutdown signal
                    break
                    
                self.process_file_transfer(transfer)
                self.upload_queue.task_done()
                
            except queue.Empty:
                continue
            except Exception as e:
                self.write_log(f"[ERROR] File transfer worker error: {e}")
                
    def process_file_transfer(self, transfer):
        """Process a file transfer"""
        recipient = self.find_client_by_username(transfer.recipient)
        if not recipient:
            error_msg = f"{Colors.RED}ERROR: User '{transfer.recipient}' not found{Colors.RESET}"
            self.send_message(transfer.sender_socket, error_msg)
            self.write_log(f"[FILE] Failed: '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}'. Recipient not found.")
            return
            
        # Create unique filename
        timestamp = int(time.time())
        unique_filename = f"{timestamp}_{transfer.filename}"
        upload_path = Path("uploads") / unique_filename
        
        # Create recipient directory
        recipient_dir = Path("received_files") / transfer.recipient
        recipient_dir.mkdir(exist_ok=True)
        
        # Generate unique filename for recipient
        final_filename = self.generate_unique_filename(str(recipient_dir), transfer.filename)
        conflict_detected = final_filename != transfer.filename
        recipient_path = recipient_dir / final_filename
        
        if conflict_detected:
            print(f"{Colors.YELLOW}[FILE-CONFLICT]{Colors.RESET} '{transfer.filename}' → renamed to '{final_filename}'")
            self.write_log(f"[FILE] Conflict: '{transfer.filename}' received twice → renamed '{final_filename}'")
            
        # Record start time and calculate wait time
        transfer.start_time = time.time()
        wait_time = transfer.start_time - transfer.request_time
        
        if wait_time > 1.0:
            print(f"{Colors.YELLOW}[FILE-QUEUE]{Colors.RESET} '{transfer.filename}' from '{transfer.sender}' started upload after {wait_time:.0f} seconds in queue")
            self.write_log(f"[FILE] '{transfer.filename}' from user '{transfer.sender}' started upload after {wait_time:.0f} seconds in queue.")
            
        # Tell sender to start sending
        start_msg = f"START_UPLOAD:{transfer.filename}"
        self.send_message(transfer.sender_socket, start_msg)
        
        # Receive file
        try:
            with open(upload_path, 'wb') as f:
                total_received = 0
                start_time = time.time()
                
                self.write_log(f"[FILE] Started upload '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}'. Size: {transfer.file_size} bytes")
                
                while total_received < transfer.file_size:
                    remaining = transfer.file_size - total_received
                    chunk_size = min(FILE_TRANSFER_BUFFER, remaining)
                    
                    data = transfer.sender_socket.recv(chunk_size)
                    if not data:
                        break
                        
                    f.write(data)
                    total_received += len(data)
                    
                end_time = time.time()
                duration = end_time - start_time
                
                if total_received == transfer.file_size:
                    # Copy to recipient directory
                    with open(upload_path, 'rb') as src, open(recipient_path, 'wb') as dst:
                        dst.write(src.read())
                        
                    # Notify sender
                    if conflict_detected:
                        success_msg = f"{Colors.GREEN}SUCCESS: File '{transfer.filename}' sent to '{transfer.recipient}' (saved as '{final_filename}' due to name conflict){Colors.RESET}"
                    else:
                        success_msg = f"{Colors.GREEN}SUCCESS: File '{transfer.filename}' sent to '{transfer.recipient}'{Colors.RESET}"
                    self.send_message(transfer.sender_socket, success_msg)
                    
                    # Notify recipient
                    if conflict_detected:
                        recv_msg = f"{Colors.BLUE}[FILE] Received file '{transfer.filename}' from '{transfer.sender}' (saved as '{final_filename}'){Colors.RESET}"
                    else:
                        recv_msg = f"{Colors.BLUE}[FILE] Received file '{transfer.filename}' from '{transfer.sender}'{Colors.RESET}"
                    self.send_message(recipient.socket, recv_msg)
                    
                    # Log success
                    if conflict_detected:
                        print(f"{Colors.GREEN}[FILE-SUCCESS]{Colors.RESET} '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}' (renamed to '{final_filename}')")
                        self.write_log(f"[FILE] Success: '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}' completed in {duration:.1f} seconds (renamed to '{final_filename}')")
                    else:
                        print(f"{Colors.GREEN}[FILE-SUCCESS]{Colors.RESET} '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}'")
                        self.write_log(f"[FILE] Success: '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}' completed in {duration:.1f} seconds")
                else:
                    # Incomplete transfer
                    error_msg = f"{Colors.RED}ERROR: File transfer incomplete. Only received {total_received} of {transfer.file_size} bytes{Colors.RESET}"
                    self.send_message(transfer.sender_socket, error_msg)
                    self.write_log(f"[FILE] Error: '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}' - Incomplete transfer ({total_received}/{transfer.file_size} bytes)")
                    
        except Exception as e:
            error_msg = f"{Colors.RED}ERROR: File transfer failed: {str(e)}{Colors.RESET}"
            self.send_message(transfer.sender_socket, error_msg)
            self.write_log(f"[FILE] Error: '{transfer.filename}' from '{transfer.sender}' to '{transfer.recipient}' - {str(e)}")
            
    def process_command(self, client_id, command):
        """Process client command"""
        parts = command.split(' ', 1)
        cmd = parts[0]
        args = parts[1] if len(parts) > 1 else ""
        
        with self.clients_lock:
            if client_id not in self.clients:
                return
            client = self.clients[client_id]
            
        if cmd == "/join":
            if not args:
                error_msg = f"{Colors.RED}ERROR: Missing room name{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            if not self.is_valid_roomname(args):
                error_msg = f"{Colors.RED}ERROR: Invalid room name. Use only alphanumeric characters (max 32){Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            safe_room_name = args[:MAX_ROOMNAME_LEN]
            old_room = client.current_room
            is_room_switch = bool(old_room)
            
            if is_room_switch:
                self.leave_room(client_id)
                
            result = self.create_or_join_room(client_id, safe_room_name)
            
            if result == 0:
                is_rejoin = self.has_been_in_room(client_id, safe_room_name)
                
                if is_rejoin:
                    success_msg = f"{Colors.GREEN}SUCCESS: Rejoined room '{safe_room_name}' (previous messages not shown){Colors.RESET}"
                else:
                    success_msg = f"{Colors.GREEN}SUCCESS: Joined room '{safe_room_name}'{Colors.RESET}"
                    
                self.send_message(client.socket, success_msg)
                
                if not is_rejoin:
                    self.add_room_to_history(client_id, safe_room_name)
                    
                # Log room activity
                if is_room_switch and is_rejoin:
                    print(f"{Colors.CYAN}[ROOM]{Colors.RESET} User '{client.username}' left '{old_room}', rejoined '{safe_room_name}'")
                    self.write_log(f"[ROOM] user '{client.username}' left room '{old_room}', rejoined '{safe_room_name}'")
                elif is_room_switch and not is_rejoin:
                    print(f"{Colors.CYAN}[ROOM]{Colors.RESET} User '{client.username}' left '{old_room}', joined '{safe_room_name}'")
                    self.write_log(f"[ROOM] user '{client.username}' left room '{old_room}', joined '{safe_room_name}'")
                elif not is_room_switch and is_rejoin:
                    print(f"{Colors.CYAN}[ROOM]{Colors.RESET} User '{client.username}' rejoined '{safe_room_name}'")
                    self.write_log(f"[ROOM] user '{client.username}' rejoined '{safe_room_name}'")
                else:
                    print(f"{Colors.CYAN}[JOIN]{Colors.RESET} User '{client.username}' joined room '{safe_room_name}'")
                    self.write_log(f"[JOIN] user '{client.username}' joined room '{safe_room_name}'")
                    
            elif result == -1:
                error_msg = f"{Colors.RED}ERROR: Room '{safe_room_name}' is full{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                
                if is_room_switch:
                    self.create_or_join_room(client_id, old_room)
                    
            else:
                error_msg = f"{Colors.RED}ERROR: Failed to join room{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                
                if is_room_switch:
                    self.create_or_join_room(client_id, old_room)
                    
        elif cmd == "/leave":
            if not client.current_room:
                error_msg = f"{Colors.RED}ERROR: You are not in any room{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            old_room = client.current_room
            self.leave_room(client_id)
            
            success_msg = f"{Colors.GREEN}SUCCESS: Left room '{old_room}'{Colors.RESET}"
            self.send_message(client.socket, success_msg)
            print(f"{Colors.YELLOW}[LEAVE]{Colors.RESET} User '{client.username}' left room '{old_room}'")
            self.write_log(f"[ROOM] user '{client.username}' left room '{old_room}'")
            
        elif cmd == "/broadcast":
            if not client.current_room:
                error_msg = f"{Colors.RED}ERROR: You are not in any room{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            if not args:
                error_msg = f"{Colors.RED}ERROR: Missing message{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            self.broadcast_message(client_id, args)
            print(f"{Colors.MAGENTA}[BROADCAST]{Colors.RESET} User '{client.username}' in room '{client.current_room}': {args}")
            self.write_log(f"[BROADCAST] user '{client.username}': {args}")
            
        elif cmd == "/whisper":
            parts = args.split(' ', 1)
            if len(parts) < 2:
                error_msg = f"{Colors.RED}ERROR: Invalid whisper format. Use /whisper <username> <message>{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            recipient_name, message = parts
            self.whisper_message(client_id, recipient_name, message)
            print(f"{Colors.BLUE}[WHISPER]{Colors.RESET} From '{client.username}' to '{recipient_name}': {message}")
            self.write_log(f"[WHISPER] From '{client.username}' to '{recipient_name}': {message}")
            
        elif cmd == "/sendfile":
            parts = args.split(' ', 1)
            if len(parts) < 2:
                error_msg = f"{Colors.RED}ERROR: Invalid format. Use /sendfile <filename> <username>{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            filename, recipient_name = parts
            
            if not self.is_valid_file_extension(filename):
                error_msg = f"{Colors.RED}ERROR: Invalid file type. Accepted: .txt, .pdf, .jpg, .png{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            recipient = self.find_client_by_username(recipient_name)
            if not recipient:
                error_msg = f"{Colors.RED}ERROR: User '{recipient_name}' not found{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                return
                
            # Request file size
            size_request = f"REQUEST_FILE_SIZE:{filename}"
            self.send_message(client.socket, size_request)
            
            # Wait for response
            try:
                response = client.socket.recv(MAX_MSG_SIZE).decode('utf-8')
                if not response.startswith("FILE_SIZE:"):
                    error_msg = f"{Colors.RED}ERROR: Invalid file size response{Colors.RESET}"
                    self.send_message(client.socket, error_msg)
                    return
                    
                file_size = int(response[10:])
                if file_size > MAX_FILE_SIZE:
                    error_msg = f"{Colors.RED}ERROR: File size exceeds limit (3MB){Colors.RESET}"
                    self.send_message(client.socket, error_msg)
                    self.write_log(f"[ERROR] File '{filename}' from user '{client.username}' exceeds size limit.")
                    return
                    
                # Add to upload queue
                transfer = FileTransfer(filename, client.username, recipient_name, client.socket, file_size)
                
                try:
                    self.upload_queue.put(transfer, block=False)
                    queue_size = self.upload_queue.qsize()
                    
                    if queue_size > 1:
                        success_msg = f"{Colors.GREEN}SUCCESS: File '{filename}' queued for upload to '{recipient_name}' (position {queue_size} in queue){Colors.RESET}"
                    else:
                        success_msg = f"{Colors.GREEN}SUCCESS: File '{filename}' queued for upload to '{recipient_name}'{Colors.RESET}"
                        
                    self.send_message(client.socket, success_msg)
                    print(f"{Colors.YELLOW}[FILE-QUEUE]{Colors.RESET} User '{client.username}' sending '{filename}' to '{recipient_name}' (position {queue_size})")
                    self.write_log(f"[FILE-QUEUE] Upload '{filename}' from {client.username} to {recipient_name} added to queue at position {queue_size}.")
                    
                except queue.Full:
                    error_msg = f"{Colors.RED}ERROR: Upload queue is full, try again later{Colors.RESET}"
                    self.send_message(client.socket, error_msg)
                    self.write_log(f"[FILE-QUEUE] Upload queue full - rejected '{filename}' from {client.username} to {recipient_name}")
                    
            except Exception as e:
                error_msg = f"{Colors.RED}ERROR: Failed to process file upload{Colors.RESET}"
                self.send_message(client.socket, error_msg)
                self.write_log(f"[ERROR] File upload processing failed: {e}")
                
        elif cmd == "/exit":
            success_msg = f"{Colors.GREEN}SUCCESS: Disconnecting from server...{Colors.RESET}"
            self.send_message(client.socket, success_msg)
            
            print(f"{Colors.RED}[LOGOUT]{Colors.RESET} User '{client.username}' disconnected")
            self.write_log(f"[LOGOUT] user '{client.username}' disconnected")
            
            self.leave_room(client_id)
            self.remove_client(client_id)
            
        else:
            error_msg = f"{Colors.RED}ERROR: Unknown command. Available commands: /join, /leave, /broadcast, /whisper, /sendfile, /exit{Colors.RESET}"
            self.send_message(client.socket, error_msg)
            
    def handle_client(self, client_id):
        """Handle client connection"""
        with self.clients_lock:
            if client_id not in self.clients:
                return
            client = self.clients[client_id]
            
        try:
            while self.keep_running and client.connected:
                data = client.socket.recv(MAX_MSG_SIZE)
                if not data:
                    break
                    
                command = data.decode('utf-8').strip()
                if command:
                    self.process_command(client_id, command)
                    
        except Exception as e:
            self.write_log(f"[ERROR] Client handler error for {client.username}: {e}")
        finally:
            print(f"{Colors.RED}[DISCONNECT]{Colors.RESET} User '{client.username}' lost connection")
            self.write_log(f"[DISCONNECT] user '{client.username}' lost connection. Cleaned up resources.")
            self.leave_room(client_id)
            self.remove_client(client_id)
            
    def handle_sigint(self, signum, frame):
        """Handle SIGINT signal"""
        # Prevent multiple shutdown attempts
        if not self.keep_running:
            return
            
        print(f"\n{Colors.YELLOW}=== SERVER SHUTTING DOWN ==={Colors.RESET}")
        self.keep_running = False
        self.cleanup_and_exit()
        
    def cleanup_and_exit(self):
        """Cleanup and exit"""
        message = f"{Colors.YELLOW}[SERVER] Server is shutting down. Goodbye!{Colors.RESET}"
        
        # Notify all clients
        with self.clients_lock:
            for client in list(self.clients.values()):
                try:
                    self.send_message(client.socket, message)
                    time.sleep(0.01)  # Brief delay
                    client.socket.close()
                except:
                    pass
                    
        # Stop file transfer worker
        try:
            self.upload_queue.put(None, timeout=1)  # Shutdown signal
        except:
            pass
            
        # Close server socket
        if self.server_socket:
            self.server_socket.close()
            
        # Close log file safely
        if self.log_file and not self.log_file.closed:
            try:
                self.log_file.write("Server shutdown complete\n")
                self.log_file.close()
            except:
                pass
            
        print(f"{Colors.GREEN}=== SERVER SHUTDOWN COMPLETE ==={Colors.RESET}")
        sys.exit(0)
        
    def start(self):
        """Start the server"""
        try:
            # Create server socket
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            
            # Bind and listen
            self.server_socket.bind(('', self.port))
            self.server_socket.listen(10)
            
            print(f"{Colors.GREEN}=== CHAT SERVER STARTED ==={Colors.RESET}")
            print(f"Server started on port {self.port}")
            print(f"{Colors.CYAN}Listening for connections...{Colors.RESET}\n")
            
            self.write_log(f"Server starting on port {self.port}")
            self.write_log(f"Server listening on port {self.port}")
            
            # Start file transfer worker
            file_worker = threading.Thread(target=self.file_transfer_worker, daemon=True)
            file_worker.start()
            
            # Main server loop
            while self.keep_running:
                try:
                    self.server_socket.settimeout(1.0)  # Check keep_running periodically
                    client_socket, client_addr = self.server_socket.accept()
                    
                    if not self.keep_running:
                        client_socket.close()
                        break
                        
                    client_ip = client_addr[0]
                    print(f"New connection from {client_ip}:{client_addr[1]}")
                    
                    # Username validation loop
                    username_accepted = False
                    username = ""
                    
                    while not username_accepted and self.keep_running:
                        try:
                            data = client_socket.recv(MAX_USERNAME_LEN)
                            if not data:
                                break
                                
                            username = data.decode('utf-8').strip()
                            
                            if not self.is_valid_username(username):
                                error_msg = "ERROR: Invalid username. Use only alphanumeric characters (max 16)"
                                client_socket.send(error_msg.encode('utf-8'))
                                self.write_log(f"[REJECTED] Invalid username format: {username} from {client_ip}")
                                continue
                                
                            # Check if username is taken
                            if self.find_client_by_username(username):
                                error_msg = "ERROR: Username already taken. Please try another username."
                                client_socket.send(error_msg.encode('utf-8'))
                                self.write_log(f"[REJECTED] Duplicate username attempted: {username} from {client_ip}")
                                continue
                                
                            username_accepted = True
                            
                        except socket.timeout:
                            continue
                        except:
                            break
                            
                    if not username_accepted:
                        client_socket.close()
                        continue
                        
                    # Add client
                    client_id = self.add_client(client_socket, username, client_ip)
                    if client_id is None:
                        error_msg = "ERROR: Server is full. Try again later."
                        client_socket.send(error_msg.encode('utf-8'))
                        client_socket.close()
                        self.write_log(f"REJECTED: Server full. Connection from {client_ip} rejected")
                        continue
                        
                    # Send welcome message
                    welcome_msg = f"SUCCESS: Welcome to the chat server, {username}!"
                    client_socket.send(welcome_msg.encode('utf-8'))
                    
                    print(f"{Colors.GREEN}[LOGIN]{Colors.RESET} User '{username}' connected from {client_ip}")
                    self.write_log(f"[LOGIN] user '{username}' connected from {client_ip}")
                    
                    # Start client handler thread
                    client_thread = threading.Thread(target=self.handle_client, args=(client_id,), daemon=True)
                    client_thread.start()
                    
                    with self.clients_lock:
                        if client_id in self.clients:
                            self.clients[client_id].thread = client_thread
                            
                except socket.timeout:
                    continue
                except Exception as e:
                    if self.keep_running:
                        print(f"Error accepting connection: {e}")
                        
        except Exception as e:
            if self.keep_running:
                print(f"Server error: {e}")
        finally:
            if self.keep_running:
                self.cleanup_and_exit()

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 server.py <port>")
        sys.exit(1)
        
    try:
        port = int(sys.argv[1])
        if port <= 0 or port > 65535:
            print("Error: Port number must be between 1 and 65535")
            sys.exit(1)
    except ValueError:
        print("Error: Invalid port number")
        sys.exit(1)
        
    server = ChatServer(port)
    server.start()

if __name__ == "__main__":
    main() 