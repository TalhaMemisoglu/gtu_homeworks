package Hmw03;

/**
* Represents a laptop device.
* Implements the Device interface.
*/
public class Laptop implements Device{
    private String category = "Laptop";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a laptop with the specified name, price, and quantity.
    * @param name The name of the laptop.
    * @param price The price of the laptop.
    * @param quantity The quantity of the laptop.
    */
    public Laptop(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }
    
    /**
    * Returns the category of the laptop.
    * Time Complexity: O(1)
    * @return The category of the laptop.
    */
    public String getCategory(){
        return category;
    }

    /**
    * Returns the name of the laptop.
    * @return The name of the laptop.
    * Time Complexity: O(1)
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the laptop.
    * Time Complexity: O(1)
    * @param name The new name of the laptop.
    */
    public void setName(String name){
        this.name = name;
    }

    /**
    * Returns the price of the laptop.
    * Time Complexity: O(1)
    * @return The price of the laptop.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the laptop.
    * Time Complexity: O(1)
    * @param price The new price of the laptop.
    */
    public void setPrice(double price){
        this.price = price;
    }

    /**
    * Returns the quantity of the laptop.
    * Time Complexity: O(1)
    * @return The quantity of the laptop.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the laptop.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the laptop.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}