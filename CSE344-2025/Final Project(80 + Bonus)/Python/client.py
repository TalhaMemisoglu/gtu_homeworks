import socket
import threading
import time
import os
import sys
import signal
import re
from pathlib import Path

# Constants
MAX_MSG_SIZE = 4096
MAX_CMD_SIZE = 256
MAX_USERNAME_LEN = 16
BUFFER_SIZE = 1024
FILE_BUFFER_SIZE = 1024

# ANSI color codes
class Colors:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN = '\033[36m'
    RESET = '\033[0m'

class ChatClient:
    def __init__(self, server_ip, port):
        self.server_ip = server_ip
        self.port = port
        self.server_socket = None
        self.username = ""
        self.keep_running = True
        self.receive_thread = None
        
        # Setup signal handler
        signal.signal(signal.SIGINT, self.handle_sigint)
        
    def is_valid_username(self, username):
        """Validate username format"""
        if not username or len(username) > MAX_USERNAME_LEN:
            return False
        return username.isalnum()
        
    def get_file_size(self, filename):
        """Get file size"""
        try:
            return os.path.getsize(filename)
        except:
            return 0
            
    def is_valid_file_extension(self, filename):
        """Check if file has valid extension"""
        valid_extensions = ['.txt', '.pdf', '.jpg', '.png']
        if '.' not in filename:
            return False
        ext = '.' + filename.split('.')[-1].lower()
        return ext in valid_extensions
        
    def send_message(self, message):
        """Send message to server"""
        try:
            self.server_socket.send(message.encode('utf-8'))
            return True
        except:
            return False
            
    def receive_messages(self):
        """Thread function to receive messages from server"""
        while self.keep_running:
            try:
                data = self.server_socket.recv(MAX_MSG_SIZE)
                if not data:
                    print(f"\n{Colors.RED}Connection closed by server{Colors.RESET}")
                    self.keep_running = False
                    break
                    
                message = data.decode('utf-8')
                
                # Handle special server commands
                if message.startswith("REQUEST_FILE_SIZE:"):
                    filename = message[18:]
                    file_size = self.get_file_size(filename)
                    
                    if file_size == 0:
                        print(f"{Colors.RED}ERROR: Could not access file '{filename}'{Colors.RESET}")
                        response = "FILE_SIZE:0"
                    else:
                        response = f"FILE_SIZE:{file_size}"
                        
                    self.send_message(response)
                    
                elif message.startswith("START_UPLOAD:"):
                    filename = message[13:]
                    self.upload_file(filename)
                    
                else:
                    # Regular message from server
                    print(f"\r{message}")
                    if self.keep_running:
                        print("> ", end="", flush=True)
                        
            except Exception as e:
                if self.keep_running:
                    print(f"\nError receiving message: {e}")
                break
                
    def upload_file(self, filename):
        """Upload file to server"""
        try:
            file_size = self.get_file_size(filename)
            if file_size == 0:
                print(f"{Colors.RED}ERROR: Could not open file '{filename}'{Colors.RESET}")
                return
                
            print(f"{Colors.YELLOW}Uploading file '{filename}' ({file_size} bytes)...{Colors.RESET}")
            
            with open(filename, 'rb') as f:
                total_sent = 0
                
                while total_sent < file_size:
                    chunk = f.read(FILE_BUFFER_SIZE)
                    if not chunk:
                        break
                        
                    bytes_sent = self.server_socket.send(chunk)
                    total_sent += bytes_sent
                    
                    # Display progress
                    percent = (total_sent / file_size) * 100
                    print(f"\rProgress: {percent:.1f}% ({total_sent}/{file_size} bytes)", end="", flush=True)
                    
            print(f"\n{Colors.GREEN}File upload completed!{Colors.RESET}")
            
        except Exception as e:
            print(f"{Colors.RED}Error uploading file: {e}{Colors.RESET}")
            
    def send_file(self, filename, recipient):
        """Send file to another user"""
        # Check if file exists
        if not os.path.isfile(filename):
            print(f"{Colors.RED}ERROR: Could not open file '{filename}'{Colors.RESET}")
            return False
            
        # Check file size
        file_size = self.get_file_size(filename)
        if file_size == 0:
            print(f"{Colors.RED}ERROR: File '{filename}' is empty or cannot be accessed{Colors.RESET}")
            return False
            
        # Check file extension
        if not self.is_valid_file_extension(filename):
            print(f"{Colors.RED}ERROR: File has invalid extension. Accepted: .txt, .pdf, .jpg, .png{Colors.RESET}")
            return False
            
        # Send file request to server
        request = f"/sendfile {filename} {recipient}"
        return self.send_message(request)
        
    def print_help(self):
        """Print help message"""
        print(f"\n{Colors.CYAN}=== Chat Client Help ==={Colors.RESET}")
        print("Available commands:")
        print(f"  {Colors.GREEN}/join <room>{Colors.RESET} - Join a chat room")
        print(f"  {Colors.GREEN}/leave{Colors.RESET} - Leave the current room")
        print(f"  {Colors.GREEN}/broadcast <message>{Colors.RESET} - Send a message to all users in the room")
        print(f"  {Colors.GREEN}/whisper <user> <message>{Colors.RESET} - Send a private message to a user")
        print(f"  {Colors.GREEN}/sendfile <filename> <user>{Colors.RESET} - Send a file to a user")
        print(f"  {Colors.GREEN}/exit{Colors.RESET} - Disconnect from the server")
        print(f"  {Colors.GREEN}/help{Colors.RESET} - Show this help message")
        print()
        
    def handle_sigint(self, signum, frame):
        """Handle SIGINT signal"""
        print(f"\n{Colors.YELLOW}Received Ctrl+C, exiting gracefully...{Colors.RESET}")
        self.keep_running = False
        
        # Try to send exit command
        if self.server_socket:
            try:
                self.send_message("/exit")
                time.sleep(0.1)  # Brief delay
            except:
                pass
                
        self.cleanup_and_exit()
        
    def cleanup_and_exit(self):
        """Cleanup and exit"""
        self.keep_running = False
        
        # Close socket
        if self.server_socket:
            try:
                self.server_socket.close()
            except:
                pass
                
        print(f"{Colors.YELLOW}Disconnected from server{Colors.RESET}")
        sys.exit(0)
        
    def connect_and_run(self):
        """Connect to server and run client"""
        try:
            # Create socket
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            
            # Connect to server
            print(f"Connecting to server at {self.server_ip}:{self.port}...")
            self.server_socket.connect((self.server_ip, self.port))
            print("Connected to server!")
            
            # Username validation loop
            username_accepted = False
            
            while not username_accepted:
                # Get username from user
                while True:
                    username = input("Enter your username (max 16 alphanumeric characters): ").strip()
                    
                    if self.is_valid_username(username):
                        break
                    else:
                        print(f"{Colors.RED}Invalid username. Use only alphanumeric characters (max 16).{Colors.RESET}")
                        
                # Send username to server
                if not self.send_message(username):
                    print("Failed to send username")
                    return False
                    
                # Wait for server response
                try:
                    response = self.server_socket.recv(MAX_MSG_SIZE).decode('utf-8')
                    print(response)
                    
                    if response.startswith("SUCCESS:"):
                        username_accepted = True
                        self.username = username
                    elif response.startswith("ERROR:"):
                        print(f"{Colors.YELLOW}Please try a different username.{Colors.RESET}")
                    else:
                        print("Unexpected server response. Disconnecting.")
                        return False
                        
                except Exception as e:
                    print(f"Error receiving response: {e}")
                    return False
                    
            # Create directory for received files
            received_dir = Path("received_files") / self.username
            received_dir.mkdir(parents=True, exist_ok=True)
            
            # Start receive thread
            self.receive_thread = threading.Thread(target=self.receive_messages, daemon=True)
            self.receive_thread.start()
            
            # Print help
            self.print_help()
            
            # Main command loop
            while self.keep_running:
                try:
                    command = input("> ").strip()
                    
                    if not self.keep_running:
                        break
                        
                    if not command:
                        continue
                        
                    # Process commands
                    if command == "/help":
                        self.print_help()
                        
                    elif command == "/exit":
                        if self.send_message(command):
                            time.sleep(0.1)  # Give server time to process
                        break
                        
                    elif command.startswith("/sendfile"):
                        parts = command.split()
                        if len(parts) != 3:
                            print(f"{Colors.RED}ERROR: Invalid format. Use /sendfile <filename> <username>{Colors.RESET}")
                            continue
                            
                        _, filename, recipient = parts
                        self.send_file(filename, recipient)
                        
                    else:
                        # Send command to server
                        if not self.send_message(command):
                            print("Failed to send command")
                            break
                            
                except KeyboardInterrupt:
                    self.handle_sigint(None, None)
                except EOFError:
                    print("\nExiting...")
                    break
                except Exception as e:
                    print(f"Error: {e}")
                    break
                    
        except Exception as e:
            print(f"Connection error: {e}")
            return False
        finally:
            self.cleanup_and_exit()
            
        return True

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 client.py <server_ip> <port>")
        sys.exit(1)
        
    server_ip = sys.argv[1]
    
    try:
        port = int(sys.argv[2])
        if port <= 0 or port > 65535:
            print("Error: Port number must be between 1 and 65535")
            sys.exit(1)
    except ValueError:
        print("Error: Invalid port number")
        sys.exit(1)
        
    client = ChatClient(server_ip, port)
    client.connect_and_run()

if __name__ == "__main__":
    main() 