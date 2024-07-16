package Hmw07;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

/**
 * A class to generate a random tree data file.
 */
public class randomTreeGenerator {
    private String fileName;
    private int treeSize;

    /**
     * Constructs a RandomTreeGenerator object.
     *
     * @param fileName the name of the file to create
     * @param treeSize the number of entries in the tree
     */
    public randomTreeGenerator(String fileName, int treeSize){
        this.fileName = fileName;
        this.treeSize = treeSize;
    }

    /**
     * Creates a file with random node comments.
     */
    public void createFile(){
        try {
            File myObj = new File(fileName);
            myObj.createNewFile();
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        try {
            FileWriter myWriter = new FileWriter(fileName);
            Random random = new Random();
            String stockSymbol = "";
            int size = 0;
            while(size < treeSize) {
                stockSymbol = "";
                for(int j = 0; j<3; j++){
                    int randomNumber = random.nextInt(26);
                    char randomUppercaseChar = (char) ('A' + randomNumber);
                    stockSymbol += randomUppercaseChar;
                }
                String data = "ADD " + stockSymbol + " " + Math.random() * 100 + " " + (long) (Math.random() * 1000000) + " " + (long) (Math.random() * 1000000000) + "\n";
                myWriter.write(data);
                if(random.nextInt(100) <  10){
                    stockSymbol = "";
                    for(int j = 0; j<3; j++){
                        int randomNumber = random.nextInt(26);
                        char randomUppercaseChar = (char) ('A' + randomNumber);
                        stockSymbol += randomUppercaseChar;
                    }
                    data = "REMOVE " + stockSymbol + "\n";
                    myWriter.write(data);
                }
                if(random.nextInt(100) < 3){
                    stockSymbol = "";
                    for(int j = 0; j<3; j++){
                        int randomNumber = random.nextInt(26);
                        char randomUppercaseChar = (char) ('A' + randomNumber);
                        stockSymbol += randomUppercaseChar;
                    }
                    data = "SEARCH " + stockSymbol + "\n";
                    myWriter.write(data);
                }
                if(random.nextInt(100) < 10){
                    stockSymbol = "";
                    for(int j = 0; j<3; j++){
                        int randomNumber = random.nextInt(26);
                        char randomUppercaseChar = (char) ('A' + randomNumber);
                        stockSymbol += randomUppercaseChar;
                    }
                    data = "UPDATE " + stockSymbol + " ";
                    stockSymbol = "";
                    for(int j = 0; j<3; j++){
                        int randomNumber = random.nextInt(26);
                        char randomUppercaseChar = (char) ('A' + randomNumber);
                        stockSymbol += randomUppercaseChar;
                    }
                    data += stockSymbol + " " + Math.random() * 100 + " " + (long) (Math.random() * 1000000) + " " + (long) (Math.random() * 1000000000) + "\n";
                    myWriter.write(data);
                }
                size++;
            }
            myWriter.close();
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}
