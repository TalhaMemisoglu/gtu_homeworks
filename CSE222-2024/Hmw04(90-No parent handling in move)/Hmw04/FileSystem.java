package Hmw04;

public class FileSystem {
    private Directory root;
    private Directory currentDirectory;

    public FileSystem() {
        this.root = new Directory("D:", null);
        this.currentDirectory = root;
    }

    /**
    * Creates a new file in the current directory with the given name.
    *
    * @param name the name of the file to be created
    */
    public void createFile(String name){
        File newfile = new File(name, currentDirectory);
        currentDirectory.addElement(newfile);
    }

    /**
    * Creates a new directory in the current directory with the given name.
    *
    * @param name the name of the directory to be created
    */
    public void createDirectory(String name){
        Directory newDirectory = new Directory(name, currentDirectory);
        currentDirectory.addElement(newDirectory);
    }

    /**
    * Moves a file or directory with the given name to the specified new path without copying.
    *
    * @param name    the name of the file or directory to be moved
    * @param newPath the new path for the file or directory
    * @return true if the move operation is successful, false otherwise
    */
    public boolean move(String name, String newPath){
        FileSystemElement direc;
        Directory targetPlace = root;
        String[] directories = newPath.split("/");
        if(!newPath.equals(root.name)){
            for(int i=1; i<directories.length; i++){
                direc = targetPlace.get(directories[i]);
                if(direc == null){
                    System.out.println("There is no directory according to this path.");
                    return false;
                }
                if(direc instanceof Directory){
                    targetPlace = (Directory) direc;

                } else {
                    System.out.println("You attempted to enter a file!");
                    return false;
                }
            }
        }

        FileSystemElement object = currentDirectory.remove(name);//This function used for not copying.
        if(object == null){
            System.out.println("There is no directory or file in this directory.");
            return false;
        }
        targetPlace.addElement(object);

        return true;
    }

    /**
    * Searches for a file or directory with the given name from the root directory.
    *
    * @param name the name of the file or directory to search for
    */
    public void search(String name){
        System.out.println("Searching from root...");
        FileSystemElement target =  root.search(name);//recursive searching algorithm.
        if(target != null){
            System.out.println("Found: " + getCurrentPath(target));
        } else {
            System.out.println("Not Found!");
        }
    }
    
    /**
    * Deletes a file or directory with the given name from the current directory using recursive algorithm.
    *
    * @param name the name of the file or directory to be deleted
    */
    public void deleteFileOrDirectory(String name){
        FileSystemElement target = currentDirectory.get(name);
        if(target != null){
            if(target instanceof File){
                currentDirectory.removeElement(target); //delete itself.
                System.out.println("File deleted: " + target.getName() + "/");
            } else if(target instanceof Directory) {
                ((Directory) target).deleteWContents(); //delete its content recursively
                System.out.println("Directory deleted: " + target.getName() + "/");
            }
        } else {
            System.out.println("There is no file/directory that you typed.");
        }
    }
    
    /**
    * Changes the current directory to the specified new path.
    *
    * @param newPath the new path for the current directory
    */
    public void changeDirectory(String newPath){
        Directory lastCurrentDirectory = currentDirectory;
        currentDirectory = root;

        FileSystemElement direc;
        String[] directories = newPath.split("/");
        if(!newPath.equals(root.name)){
            for(int i=1; i<directories.length; i++){
                direc = currentDirectory.get(directories[i]);
                if(direc instanceof Directory){
                    currentDirectory = (Directory) direc;
                } else {
                    System.out.println("You attempted to enter a file!");
                }

                if(currentDirectory == null){
                    System.out.println("There is no directory according to this path.");
                    currentDirectory = lastCurrentDirectory;
                    break;
                }
            }
        }
        System.out.println("Directory changed to: " + getCurrentPath(currentDirectory));
    }

    /**
    * Lists the contents of the current directory.
    */
    public void ListDirectoryContents(){
        System.out.println("Listing contents of " + getCurrentPath() + ":");
        currentDirectory.ListContents();
    }

    /**
    * Prints the path to the current directory from the root directory recursively.
    */
    private String print(FileSystemElement current){
        if(current == root){
            System.out.println("* " + current.getName() + "/");
            return ("   ");
        } else {
            String prefix = print(current.getParent());
            System.out.println( prefix + "* " + current.getName() + "/");
            return ("   " + prefix);
        }
    }
    /**
    * Prints the path to the current directory from the root directory.
    */
    public void print(){
        System.out.println("Path to current directory from root:");
        String prefix = "";
        if(currentDirectory != root)
            prefix = print(currentDirectory.getParent()); //Print objects above recursively.
        currentDirectory.print(prefix);
    }

    /**
    * Prints the contents of the current directory sorted by creation date.
    */
    public void printContentsSorted(){
        System.out.println("Sorted contents of " + getCurrentPath());
        currentDirectory.printContentsSorted();
    }

    private String getCurrentPath(FileSystemElement currentDirectory){
        if(currentDirectory.parent == null){
            return currentDirectory.name;
        }
        return getCurrentPath(currentDirectory.parent) + "/" + currentDirectory.name;
    }
    /**
    * Returns the current path of the current directory.
    *
    * @return the current path of the current directory
    */
    public String getCurrentPath(){
        if(currentDirectory == root)
            return getCurrentPath(currentDirectory) + "/";
        else{
            return getCurrentPath(currentDirectory);
        }
    }
}