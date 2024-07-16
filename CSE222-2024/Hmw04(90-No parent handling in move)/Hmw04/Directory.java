package Hmw04;

import java.util.Collections;
import java.util.LinkedList;

public class Directory extends FileSystemElement{
    private LinkedList<FileSystemElement> children;

    public Directory(String name, FileSystemElement parent){
        super(name, parent);
        children = new LinkedList<>();
    }

    /**
    * Returns the child file system element with the specified name.
    *
    * @param name the name of the child file system element to be retrieved
    * @return the child file system element with the specified name, or null if not found
    */
    public FileSystemElement get(String name){
        for(FileSystemElement a : children){
            if(a.name.equals(name))
                return a;
        }
        return null;
    }

    /**
    * Prints the names of the contents of the directory.
    */
    public void ListContents(){
        for(FileSystemElement a: children){
            if(a instanceof Directory){
                System.out.println("* " + a.name + "/");
            } else if(a instanceof File){
                System.out.println(a.name);
            }
        }
    }

    /**
    * Searches for a file system element with the specified name recursively within the directory.
    *
    * @param targetName the name of the file system element to search for
    * @return the file system element with the specified name, or null if not found
    */
    public FileSystemElement search(String targetName){
        for(FileSystemElement a: children){
            if(targetName.equals(a.name)){
                return a;
            } else {
                if(a instanceof Directory){
                    return ((Directory)a).search(targetName);
                }
            }
        }
        return null;
    }

    /**
    * Adds a file system element to the directory.
    *
    * @param element the file system element to be added
    */
    public void addElement(FileSystemElement element){children.add(element);}

    /**
    * Removes a file system element from the directory.
    *
    * @param element the file system element to be removed
    */
    public void removeElement(FileSystemElement element){children.remove(element);}

    /**
    * Removes a file system element with copying the specified name from the directory without copying.
    *
    * @param elementName the name of the file system element to be removed
    * @return the removed file system element, or null if not found
    */
    public FileSystemElement remove(String elementName)
    {
        int index = 0;
        for(FileSystemElement a : children){
            if(a.name.equals(elementName))
                return children.remove(index);
            index++;
        }
        return null;
    }

    /**
    * Deletes the contents of the directory recursively and the directory itself.
    */
    public void deleteWContents()
    {
        for(FileSystemElement a : children){
            if(a instanceof Directory){
                ((Directory) a).deleteWContents();
            } else if(a instanceof File){
                removeElement(a);
            }
        }
        ((Directory)parent).removeElement(this);
    }

    /**
    * Prints the contents of the directory sorted by creation date.
    */
    @SuppressWarnings("unchecked")
    public void printContentsSorted(){
        LinkedList<FileSystemElement> sortedElement = (LinkedList<FileSystemElement>) children.clone();
        Collections.sort(sortedElement, (x,y) -> x.getDataCreated().compareTo(y.getDataCreated()));
        
        for(FileSystemElement a : sortedElement) {
            if(a instanceof Directory){
                System.out.println("* " + a.getName() + "/ " + "(" + a.getDataCreated() + ")");
            } else if(a instanceof File){
                System.out.println(a.getName() + " (" + a.getDataCreated() + ")");
            }
        }
    }

    /**
    * Prints the directory and its contents with the given prefix.
    *
    * @param prefix the prefix string to be printed before the directory and its contents
    */
    public void print(String prefix){
        System.out.println(prefix + "* " + getName() + "/");
        for(FileSystemElement a : children) {
            if(a instanceof Directory){
                a.print("   " + prefix);
            } else if(a instanceof File){
                a.print(prefix + "  ");
            }
        }
    }
}