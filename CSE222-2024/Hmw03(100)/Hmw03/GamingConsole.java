package Hmw03;

/**
* Represents a gaming console device.
* Implements the Device interface.
*/
public class GamingConsole implements Device{
    private String category = "Gaming Console";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a gaming console with the specified name, price, and quantity.
    * @param name The name of the gaming console.
    * @param price The price of the gaming console.
    * @param quantity The quantity of the gaming console available.
    */
    public GamingConsole(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
    * Returns the category of the gaming console.
    * Time Complexity: O(1)
    * @return The category of the gaming console.
    */
    public String getCategory(){
        return category;
    }

    /**
    * Returns the name of the gaming console.
    * Time Complexity: O(1)
    * @return The name of the gaming console.
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the gaming console.
    * Time Complexity: O(1)
    * @param name The new name of the gaming console.
    */
    public void setName(String name){
        this.name = name;
    }
    
    /**
    * Returns the price of the gaming console.
    * Time Complexity: O(1)
    * @return The price of the gaming console.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the gaming console.
    * Time Complexity: O(1)
    * @param price The new price of the gaming console.
    */
    public void setPrice(double price){
        this.price = price;
    }

    /**
    * Returns the quantity of the gaming console.
    * Time Complexity: O(1)
    * @return The quantity of the gaming console.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the gaming console.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the gaming console.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}