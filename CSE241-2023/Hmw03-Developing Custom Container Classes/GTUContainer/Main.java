package GTUContainer;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

/**
 * A class containing a main method for testing JavaVector, JavaSet and JavaIterator.
 */
public class Main {
    /**
     * Writes the string representation of a JavaContainer object to a file which is specified.
     * 
     * @param object       The JavaContainer object to be written to the file.
     * @param fileName    The name of the file to write to.
     * @param <T>  The type of elements in the JavaContainer.
     */
    public static <T> void writeSetToFile(JavaContainer<T> object, String fileName)
    {
        File file = new File(fileName);
        if(!file.exists())
        {
            try
            {
                file.createNewFile();
            } catch(IOException e) {
                System.out.println("Unexpected error occured!");
                e.printStackTrace();
            }
        } else {
            try
            {
                FileWriter fwriter = new FileWriter(fileName);
                String data = object.toString() + "\n";
                fwriter.write(data);
                fwriter.close();
            } catch(IOException e) {
                System.out.println("Unexpected error occured!");
                e.printStackTrace();
            }
        }
    }
    /**
     * The main method for testing JavaVector, JavaSet, and JavaIterator implemantations.
     * @param args
     */
    public static void main(String[] args)
    {
        JavaVector<Integer> myVector = new JavaVector<Integer>(20);
        myVector.add(5);
        myVector.add(10);
        myVector.add(20);
        myVector.add(30);
        myVector.remove(20);
        
        JavaSet<Integer> mySet = new JavaSet<Integer>(20);
        mySet.add(5);
        mySet.add(10);
        mySet.add(20);
        mySet.add(20);

        System.out.printf("Vector: %s\n",myVector);
        System.out.printf("Set: %s\n",mySet);
        System.out.printf("Equality: %b\n\n",myVector.equals(mySet));

        JavaIterator<Integer> i = myVector.getIterator();
        int current = 0;
        System.out.println("Iterator Test for the Vector:");
        while(i.hasNext())
        {
            System.out.print("Current index : " + current);
            System.out.println(" Next Element is: " + i.next());
            current++;
        }
        System.out.println();

        JavaIterator<Integer> j = mySet.getIterator();
        current = 0;
        System.out.println("Iterator Test for the Set:");
        while(j.hasNext())
        {
            System.out.print("Current index : " + current);
            System.out.println(" Next Element is: " + j.next());
            current++;
        }
        System.out.println();

        System.out.println("Writing to file...");
        writeSetToFile(myVector, "set1.txt");
        writeSetToFile(mySet, "set2.txt");
        System.out.println("Written to file.\n");
        System.out.println("Terminating the program...");
    }
}
