package Hmw04;
import java.sql.Timestamp;

public abstract class FileSystemElement {
    protected String name;
    protected Timestamp dataCreated;
    protected FileSystemElement parent;

    public FileSystemElement(String name, FileSystemElement parent){
        this.name = name;
        this.parent = parent;
        this.dataCreated = new Timestamp(System.currentTimeMillis());
    }

    public String getName(){
        return name;
    }

    public Timestamp getDataCreated(){
        return dataCreated;
    }

    public FileSystemElement getParent(){
        return parent;
    }

    public void setParent(FileSystemElement parent){
        this.parent = parent;
    }

    public abstract void print(String prefix);
}