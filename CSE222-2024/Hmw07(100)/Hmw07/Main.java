package Hmw07;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.io.File;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("Enter initial size of AVL Tree:");
        System.out.print("> ");
        int initialSize = scanner.nextInt();
        System.out.println("Enter amount of increase size:");
        System.out.print("> ");
        int increaseAmount = scanner.nextInt();
        System.out.println("Enter bound(inclusive):");
        System.out.print("> ");
        int bound = scanner.nextInt();

        //Random file creation part
        for(int i=initialSize; i<=bound; i+=increaseAmount){
            String fileName = "AVLTree" + (i/increaseAmount) + ".txt";
            System.out.println("Creating " + fileName + "...");
            randomTreeGenerator randomFile = new randomTreeGenerator(fileName , i);
            randomFile.createFile();
            System.out.println(fileName + " was created.");
        }

        List<Integer> DataX = new ArrayList<>();
        List<Long> DataYforSearch = new ArrayList<>();
        List<Long> DataYforAdd = new ArrayList<>();
        List<Long> DataYforRemove = new ArrayList<>();

        //Reading Part
        int numOfFile = bound/increaseAmount;
        for(int i=1; i<=numOfFile; i++){
            StockDataManager manager = new StockDataManager();
            String fileName = "AVLTree" + i + ".txt";
            System.out.println(fileName + " is being read...");
            
            try{
                Scanner fileScanner = new Scanner(new File(fileName));
                String line;
                while (fileScanner.hasNextLine()) {
                    line = fileScanner.nextLine();
                    processCommand(line, manager);
                }
                fileScanner.close();
                manager.printTree();
                System.out.println(fileName + " was read.");
                System.out.println(fileName + " is being analyzed");
                performanceAnalysis analyzer = new performanceAnalysis(manager,100);
                DataX.add(initialSize + (increaseAmount*(i - 1)));
                analyzer.performAnalysis();
                DataYforAdd.add(analyzer.getDataYforAdd());
                DataYforSearch.add(analyzer.getDataYforSearch());
                DataYforRemove.add(analyzer.getDataYforRemove());
                System.out.println(fileName + " was analyzed");
            } catch (Exception e) {
                System.err.println("Error analyzing file: " + fileName);
                e.printStackTrace();
            }
        }

        String plotType = "scatter"; // Change to "scatter" for scatter plot
        GUIVisualization frame = new GUIVisualization(plotType); // Create a new instance of GUIVisualization
        frame.setX(DataX);

        System.out.println("---Performance Graphs---");
        System.out.println("1. Adding Performance");
        System.out.println("2. Searching Performance");
        System.out.println("3. Removing Performance");
        System.out.print("> ");
        int menu = scanner.nextInt();

        switch(menu){
            case 1:
                frame.setTitle("Adding Performance Graph Visualization");
                frame.setY(DataYforAdd);
                frame.setVisible(true);
                break;
            case 2:
                frame.setTitle("Searching Performance Graph Visualization");
                frame.setY(DataYforSearch);
                frame.setVisible(true);
                break;    
            case 3:
                frame.setTitle("Removing Performance Graph Visualization");
                frame.setY(DataYforRemove);
                frame.setVisible(true);
                break;
            default:
                System.out.println("Wrong Typing");
        }

        scanner.close();
    }

    private static void processCommand(String line, StockDataManager manager) {
        String[] tokens = line.split(" ");
        String command = tokens[0];

        switch (command) {
            case "ADD":
                manager.addOrUpdateStock(tokens[1], Double.parseDouble(tokens[2]), Long.parseLong(tokens[3]), Long.parseLong(tokens[4]));
                break;
            case "REMOVE":
                manager.removeStock(tokens[1]);
                break;
            case "SEARCH":
                Stock stock = manager.searchStock(tokens[1]);
                if (stock != null) {
                    System.out.println(stock);
                } else {
                    System.out.println("Stock not found: " + tokens[1]);
                }
                break;
            case "UPDATE":
                manager.updateStock(tokens[1], tokens[2] ,Double.parseDouble(tokens[3]), Long.parseLong(tokens[4]), Long.parseLong(tokens[5]));
                break;
            default:
                System.out.println("Unknown command: " + command);
                break;
        }
    }
}
