package Hmw07;

public class StockDataManager {
    private AVLTree<Stock> avlTree;

    public StockDataManager() {
        avlTree = new AVLTree<>();
    }

    // Add or update a stock
    public void addOrUpdateStock(String symbol, double price, long volume, long marketCap) {
        Stock target = new Stock(symbol, 0, 0, 0);
        Stock existingStock = avlTree.find(target);
        if (existingStock != null) {
            existingStock.setPrice(price);
            existingStock.setVolume(volume);
            existingStock.setMarketCap(marketCap);
        } else {
            Stock newStock = new Stock(symbol, price, volume, marketCap);
            avlTree.add(newStock);
        }
    }

    public void printTree(){
        avlTree.preOrderTraversal();
    }

    // Remove a stock
    public void removeStock(String symbol) {
        Stock target = new Stock(symbol, 0, 0, 0);
        avlTree.delete(target);
    }

    // Search for a stock
    public Stock searchStock(String symbol) {
        Stock target = new Stock(symbol, 0, 0, 0);
        return avlTree.find(target);
    }

    // Update stock details
    public void updateStock(String symbol, String newSymbol, double newPrice, long newVolume, long newMarketCap) {
        Stock target = new Stock(symbol, 0, 0, 0);
        Stock stock = avlTree.find(target);
        if (stock != null) {
            stock.setSymbol(newSymbol);
            stock.setPrice(newPrice);
            stock.setVolume(newVolume);
            stock.setMarketCap(newMarketCap);
        }
    }

    // Main method for testing
    public static void main(String[] args) {
        StockDataManager manager = new StockDataManager();
        manager.addOrUpdateStock("AAPL", 150.0, 1000000, 2500000000L);
        manager.addOrUpdateStock("GOOGL", 2800.0, 500000, 1500000000L);
        System.out.println(manager.searchStock("AAPL"));
        manager.removeStock("AAPL");
        System.out.println(manager.searchStock("AAPL"));
    }
}
