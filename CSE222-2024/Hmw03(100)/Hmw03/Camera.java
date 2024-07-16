package Hmw03;

/**
* Represents a camera device.
* Implements the Device interface.
*/
public class Camera implements Device{
    private String category = "Camera";
    private String name;
    private double price;
    private int quantity;

    /**
    * Constructs a camera with the specified name, price, and quantity.
    * @param name The name of the camera.
    * @param price The price of the camera.
    * @param quantity The quantity of the camera available.
    */
    public Camera(String name, double price, int quantity){
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
    * Returns the category of the camera.
    * Time Complexity: O(1)
    * @return The category of the camera.
    */
    public String getCategory(){
        return category;
    }

    /**
    * Returns the name of the camera.
    *Time Complexity: O(1)
    * @return The name of the camera.
    */
    public String getName(){
        return name;
    }
    /**
    * Sets the name of the camera.
    * Time Complexity: O(1)
    * @param name The new name of the camera.
    */
    public void setName(String name){
        this.name = name;
    }

    /**
    * Returns the price of the camera.
    * Time Complexity: O(1)
    * @return The price of the camera.
    */
    public double getPrice(){
        return price; 
    }
    /**
    * Sets the price of the camera.
    * Time Complexity: O(1)
    * @param price The new price of the camera.
    */
    public void setPrice(double price){
        this.price = price;
    }

    /**
    * Returns the quantity of the camera available.
    * Time Complexity: O(1)
    * @return The quantity of the camera available.
    */
    public int getQuantity(){
        return quantity;
    }
    /**
    * Sets the quantity of the camera available.
    * Time Complexity: O(1)
    * @param quantity The new quantity of the camera available.
    */
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}