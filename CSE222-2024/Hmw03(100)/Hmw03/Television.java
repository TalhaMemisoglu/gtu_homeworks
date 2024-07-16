package Hmw03;

/**
 * Represents a television device.
 * Implements the Device interface.
 */
public class Television implements Device{
    private String category = "Television";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a television with the specified name, price, and quantity.
    * @param name The name of the television.
    * @param price The price of the television.
    * @param quantity The quantity of the television.
    */
    public Television(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
    * Returns the category of the television.
    * Time Complexity: O(1)
    * @return The category of the television.
    */
    public String getCategory(){
        return category;
    }

    /**
    * Returns the name of the television.
    * Time Complexity: O(1)
    * @return The name of the television.
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the television.
    * Time Complexity: O(1)
    * @param name The new name of the television.
    */
    public void setName(String name){
        this.name = name;
    }

    /**
    * Returns the price of the television.
    * Time Complexity: O(1)
    * @return The price of the television.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the television.
    * Time Complexity: O(1)
    * @param price The new price of the television.
    */
    public void setPrice(double price){
        this.price = price;
    }

    /**
     * Returns the quantity of the television.
     * Time Complexity: O(1)
     * @return The quantity of the television.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the television.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the television.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}