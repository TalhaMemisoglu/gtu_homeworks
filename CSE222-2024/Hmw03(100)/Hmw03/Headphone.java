package Hmw03;

/**
* Represents a headphone device.
* Implements the Device interface.
*/
public class Headphone implements Device{
    private String category = "Headphone";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a headphone with the specified name, price, and quantity.
    * @param name The name of the headphone.
    * @param price The price of the headphone.
    * @param quantity The quantity of the headphone.
    */
    public Headphone(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
    * Returns the category of the headphone.
    * Time Complexity: O(1)
    * @return The category of the headphone.
    */
    public String getCategory(){
        return category;
    }
    /**
    * Returns the name of the headphone.
    * Time Complexity: O(1)
    * @return The name of the headphone.
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the headphone.
    * Time Complexity: O(1)
    * @param name The new name of the headphone.
    */
    public void setName(String name){
        this.name = name;
    }

    /**
    * Returns the price of the headphone.
    * Time Complexity: O(1)
    * @return The price of the headphone.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the headphone.
    * Time Complexity: O(1)
    * @param price The new price of the headphone.
    */
    public void setPrice(double price){
        this.price = price;
    }
    /**
    * Returns the quantity of the headphone.
    * Time Complexity: O(1)
    * @return The quantity of the headphone.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the headphone.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the headphone.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}