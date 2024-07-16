package Hmw07;

import java.util.Random;

/**
* A class to perform performance analysis on AVL Tree operations.
*/
public class performanceAnalysis {
    private Long DataYforSearch = 0L;
    private Long DataYforAdd = 0L;
    private Long DataYforRemove = 0L;
    private int size;
    private StockDataManager manager;

    public performanceAnalysis(StockDataManager manager){
        this.manager = manager;
        this.size = 100;
    }
    
    public performanceAnalysis(StockDataManager manager, int size){
        this.manager = manager;
        this.size = size;
    }

    public Long getDataYforSearch(){
        return DataYforSearch;
    }

    public Long getDataYforAdd(){
        return DataYforAdd;
    }

    public Long getDataYforRemove(){
        return DataYforRemove;
    }

    /*public void performAnalysis(){
        long startTime, endTime;
        
        startTime = System.nanoTime();
        for(int i=0; i<size; i++){
            manager.addOrUpdateStock("SYM" + i , Math.random() * 100, (long) (Math.random() * 1000000), (long) (Math.random() * 1000000000));
        }
        endTime = System.nanoTime();
        DataYforAdd = (endTime - startTime) / size;
        
        startTime = System.nanoTime();
        for(int i=0; i<size; i++){
            manager.searchStock("SYM" + i);
        }
        endTime = System.nanoTime();
        DataYforSearch = (endTime - startTime)  / size;
        
        startTime = System.nanoTime();
        for(int i=0; i<size; i++){
            manager.removeStock("SYM" + i);
        }
        endTime = System.nanoTime();
        DataYforRemove = (endTime - startTime) / size;
    }*/

    /**
     * Performs performance analysis on stock data operations.
     */
    public void performAnalysis(){        
        long startTime, endTime;
        Random random = new Random();
        String stockSymbol = "";
        for(int i=0; i<size; i++){
            stockSymbol = "";
            for(int j = 0; j<3; j++){
                int randomNumber = random.nextInt(26);
                char randomUppercaseChar = (char) ('A' + randomNumber);
                stockSymbol += randomUppercaseChar;
            }
            
            startTime = System.nanoTime();
            manager.addOrUpdateStock(stockSymbol , Math.random() * 100, (long) (Math.random() * 1000000), (long) (Math.random() * 1000000000));
            endTime = System.nanoTime();
            Long Time = (endTime - startTime);
            DataYforAdd += Time;
        
            startTime = System.nanoTime();
            manager.searchStock(stockSymbol);
            endTime = System.nanoTime();
            Time = (endTime - startTime);
            DataYforSearch += Time;
        
            startTime = System.nanoTime();
            manager.removeStock(stockSymbol);
            endTime = System.nanoTime();
            Time = (endTime - startTime);
            DataYforRemove += Time;
        }
        DataYforAdd /= size;
        DataYforSearch /= size;
        DataYforRemove /= size;
    }
}