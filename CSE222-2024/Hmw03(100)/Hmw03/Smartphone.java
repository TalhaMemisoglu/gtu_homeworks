package Hmw03;

/**
* Represents a smartphone device.
* Implements the Device interface.
*/
public class Smartphone implements Device{
    private String category = "Smartphone";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a smartphone with the specified name, price, and quantity.
    * @param name The name of the smartphone.
    * @param price The price of the smartphone.
    * @param quantity The quantity of the smartphone.
    */
    public Smartphone(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
    * Returns the category of the smartphone.
    * Time Complexity: O(1)
    * @return The category of the smartphone.
    */
    public String getCategory(){
        return category;
    }

    /**
    * Returns the name of the smartphone.
    * Time Complexity: O(1)
    * @return The name of the smartphone.
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the smartphone.
    * Time Complexity: O(1)
    * @param name The new name of the smartphone.
    */
    public void setName(String name){
        this.name = name;
    }
    
    /**
    * Returns the price of the smartphone.
    * Time Complexity: O(1)
    * @return The price of the smartphone.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the smartphone.
    * Time Complexity: O(1)
    * @param price The new price of the smartphone.
    */
    public void setPrice(double price){
        this.price = price;
    }
    
    /**
    * Returns the quantity of the smartphone.
    * Time Complexity: O(1)
    * @return The quantity of the smartphone.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the smartphone.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the smartphone.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}