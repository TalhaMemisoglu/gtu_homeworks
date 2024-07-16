package Hmw04;

import java.util.Scanner;

public class Main {
    private static FileSystem fs = new FileSystem();

    private static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args){
        boolean status = true;
        while(status){
            System.out.println("===== File System Management Menu =====");
            System.out.println("1. Change directory");
            System.out.println("2. List directory contents ");
            System.out.println("3. Create file/directory");
            System.out.println("4. Delete file/directory");
            System.out.println("5. Move file/directory");
            System.out.println("6. Search file/directory");
            System.out.println("7. Print directory tree");
            System.out.println("8. Sort contents by date created");
            System.out.println("9. Exit");
            System.out.println("Current directory: " + fs.getCurrentPath());
            System.out.print("Please select an option: ");

            int choice = scanner.nextInt();
            scanner.nextLine();

            switch(choice){
                case 1:
                    changeDirectory();
                    break;
                case 2:
                    ListDirectoryContents();
                    break;
                case 3:
                    createFilesAndDirectories();
                    break;
                case 4:
                    deleteFilesAndDirectories();
                    break;
                case 5:
                    moveFileAndDirectories();
                    break;
                case 6:
                    search();
                    break;
                case 7:
                    print();
                    break;
                case 8:
                    printContentsSorted();
                    break;
                case 9:
                    status = false;
                    break;

            }
        }
    }

    public static void changeDirectory(){
        System.out.println("Current directory: " + fs.getCurrentPath());
        System.out.print("Enter new directory path: " );
        String newPath = scanner.nextLine();
        fs.changeDirectory(newPath);
    }

    public static void ListDirectoryContents(){
        fs.ListDirectoryContents();
    }

    public static void createFilesAndDirectories(){
        System.out.println("Current directory: " + fs.getCurrentPath());
        System.out.print("Create file or directory(f/d): ");
        String choice = scanner.nextLine();
        String name;
        switch(choice){
            case "f":
                System.out.print("Enter name for new file: ");
                name = scanner.nextLine();
                fs.createFile(name);
                System.out.println("File created: " + name);
                break;
            case "d":
                System.out.print("Enter name for new directory: ");
                name = scanner.nextLine();    
                fs.createDirectory(name);
                System.out.println("Directory created: " + name + "/");
                break;
            default:
                System.out.println("Wrong typing!");
        }
    }

    public static void deleteFilesAndDirectories(){
        System.out.println("Current directory: " + fs.getCurrentPath());
        System.out.print("Enter name of file/directory to delete: ");
        String name = scanner.nextLine();
        fs.deleteFileOrDirectory(name);
    }

    public static void moveFileAndDirectories(){
        System.out.println("Current directory: " + fs.getCurrentPath());
        System.out.print("Enter name of file/directory to move: ");
        String name = scanner.nextLine();
        System.out.print("Enter new directory path: ");
        String newPath = scanner.nextLine();
        fs.move(name, newPath);
    }

    public static void search(){
        System.out.print("Search query: ");
        String name = scanner.nextLine();
        fs.search(name);
    }

    public static void print(){
        fs.print();
    }

    public static void printContentsSorted(){
        fs.printContentsSorted();
    }
}
