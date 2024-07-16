package Hmw03;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.invoke.WrongMethodTypeException;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.LinkedList;
import java.util.Scanner;

import javax.naming.NoPermissionException;

/**
* Inventory class represents a collection of electronic devices categorized by type.
*/
public class Inventory {
    private LinkedList<ArrayList<Device>> Devices = new LinkedList<>();
    
    /**
    * Checks if a device with given category and name exists in the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @param category The category of the device.
    * @param name The name of the device.
    * @return true if the device exists, false otherwise.
    */
    private boolean contains(String category, String name){
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                if(currentDevice.getCategory().equalsIgnoreCase(category) 
                    && currentDevice.getName().equalsIgnoreCase(name))
                    return true;
            }
        }
        return false;
    }
    /**
    * Adds a new device to the inventory.
    * Time Complexity: O(n^2), where n is the number of categories in the inventory.
    * @param category The category of the device.
    * @param name The name of the device.
    * @param price The price of the device.
    * @param quantity The quantity of the device.
    * @throws InputMismatchException If the category is undefined.
    */
    private void addDevice(String category, String name, double price, int quantity) throws InputMismatchException{
        Device newDevice;
        category = category.toLowerCase();
        category = category.replaceAll("\\s", "");
        switch(category){
            case "camera":
                newDevice = new Camera(name, price, quantity);
                break;
            case "gamingconsole":
                newDevice = new GamingConsole(name, price, quantity);
                break;
            case "headphone":
                newDevice = new Headphone(name, price, quantity);
                break;
            case "laptop":
                newDevice = new Laptop(name, price, quantity);
                break;
            case "smartphone":
                newDevice = new Smartphone(name, price, quantity);
                break;
            case "television":
                newDevice = new Television(name, price, quantity);
                break;
            default:
                System.out.println("UNDEFİNED CATEGORY!");
                newDevice = null;
                throw new InputMismatchException();         
        }
        boolean status = false;
        for(int i=0; i<Devices.size(); i++){
            Device firstElementInLL = Devices.get(i).get(0);
            if(newDevice.getClass() == firstElementInLL.getClass()){
                Devices.get(i).add(newDevice);
                status = true;
            }
        }
        if(!status){
            ArrayList<Device> newList = new ArrayList<>();
            newList.add(newDevice);
            Devices.add(newList);
        }
    }
    /**
    * Adds a device to the inventory based on user input.
    * Time Complexity: Depends on addDevice() method, 
    * so O(n^2), where n is the number of categories in the inventory.
    */
    public void addDevice() throws InputMismatchException{
        Scanner in = new Scanner(System.in);
        System.out.print("Enter category name: ");
        String category = in.nextLine();
        System.out.print("Enter device name: ");
        String name = in.nextLine();
        System.out.print("Enter price: ");
        String Strprice = in.nextLine();
        Strprice = Strprice.replace("$", "");
        double price = Double.parseDouble(Strprice);
        System.out.print("Enter quantity: ");
        int quantity = in.nextInt();
        if(!contains(category, name))
            addDevice(category, name, price, quantity);
        System.out.println(category + ", " + name + ", " + price + ", " + quantity + " amount added. ");
    }
    /**
     * Removes a device from the inventory.
     * Time Complexity: O(n*m), where n is the total number of Device objects in all the ArrayLists in 'Devices' 
     * and m is the average number of devices per category.
     * @param category The category of the device to remove.
     * @param name The name of the device to remove.
     * @return true if removal is successful, false otherwise.
     */
    private boolean removeDevice(String category, String name){
        int index;
        name = name.replaceAll("//s", "");
        category = category.replaceAll("//s", "");
        for(ArrayList<Device> List : Devices)
        {
            index = 0;
            for(Device currentDevice : List)
            {
                if(currentDevice.getCategory().equalsIgnoreCase(category) &&
                    currentDevice.getName().equalsIgnoreCase(name)){
                    List.remove(index);
                    return true;
                }
                ++index;
            }
        }
        return false;
    }
    /**
    * Removes a device from the inventory based on user input.
    * Time Complexity: Depends on removeDevice() method
    * so O(n*m), where n is the total number of Device objects in all the ArrayLists in 'Devices' 
    * and m is the average number of devices per category.
    */
    public void removeDevice(){
        Scanner in = new Scanner(System.in);
        System.out.print("Enter the category of the device to remove: ");
        String category = in.nextLine();
        System.out.print("Enter the name of the device to remove: ");
        String name = in.nextLine();
        if(removeDevice(category, name))
            System.out.println("Removel successful.");
        else{
            System.out.println("Device you entered couldn't find.");
        }
    }
    /**
    * Updates details of a device in the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @param name The name of the device to update.
    * @param price The new price of the device.
    * @param quantity The new quantity of the device.
    */
    private void updateDevice(String name, double price, int quantity){
        name = name.replaceAll("//s", "");
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                if(currentDevice.getName().equalsIgnoreCase(name)){
                    System.out.print(currentDevice.getName() + " details updated: ");
                    if(price != -1){
                        currentDevice.setPrice(price);
                        System.out.print("Price - " + price + "$, ");
                    } else {
                        System.out.print("Price - Same, ");
                    }
                    if(quantity != -1){
                        currentDevice.setQuantity(quantity);
                        System.out.print("Quantity - " + quantity);
                    } else {
                        System.out.print("Quantity - Same");
                    }
                }
            }
        }
    }
    /**
    * Updates details of a device in the inventory based on user input.
    * Time Complexity: Depends on updateDevice() method.
    * so O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @throws NoPermissionException If the price or quantity is negative.
    */
    public void updateDevice() throws NoPermissionException{
        Scanner in = new Scanner(System.in);
        System.out.print("Enter the name of the device to update: ");
        String name = in.nextLine();
        System.out.print("Enter new price (leave blank to keep current price): ");
        String Strprice = in.nextLine();
        Strprice = Strprice.replace("$", "");
        double newPrice = -1;
        if(Strprice != " "){
            newPrice = Double.parseDouble(Strprice);
            if(newPrice < 0)
                throw new NoPermissionException("Price can't be negative number.");
        }
        System.out.print("Enter new quantity (leave blank to keep current quantity): ");
        String Strquantity = in.nextLine();
        int newQuantity = -1;
        if(Strquantity != " "){
            newQuantity = Integer.parseInt(Strquantity);
            if(newPrice < 0)
                throw new NoPermissionException("Quantity can't be negative number.");
        }
        updateDevice(name, newPrice, newQuantity);
    }

    /**
    * Displays all devices in the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'. 
    */
    public void displayAllDevices(){
        System.out.println("Device List:");
        int deviceCounter = 1;
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                System.out.println(deviceCounter + ". Category: " + currentDevice.getCategory() + ", Name: " + currentDevice.getName() +
                                 ", Price: " + currentDevice.getPrice() + "$" + ", Quantity: " + currentDevice.getQuantity());
                ++deviceCounter;
            }
        }
    }

    /**
    * Finds the cheapest device in the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @return The cheapest device.
    */
    public Device findCheapestDevice(){
        Device min_Element = Devices.get(0).get(0); 
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                if(currentDevice.getPrice() < min_Element.getPrice()){
                    min_Element = currentDevice;
                }
            }
        }
        System.out.println("Category: " + min_Element.getCategory() + ", Name: " + min_Element.getName() +
                                 ", Price: " + min_Element.getPrice() + "$" + ", Quantity: " + min_Element.getQuantity());
        return min_Element;
    }

    /**
    * Sorts the devices in the inventory by price in ascending order.
    * Time Complexity: O(n^2), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    */
    public void sortDevicesByPrice(){
        Device max = new Laptop("max", Double.MAX_VALUE, 0);
        Device minElement = null;
        int counter = 0;

        System.out.print(++counter + ". ");
        Device lastMinimum = findCheapestDevice();
        while(minElement != max){
            minElement = max;
            for(ArrayList<Device> List : Devices)
            {
                for(Device currentDevice : List)
                {
                    if(currentDevice.getPrice() > lastMinimum.getPrice()){
                        if(currentDevice.getPrice() < minElement.getPrice()){
                            minElement = currentDevice;
                        }
                    } else if(currentDevice.getPrice() == lastMinimum.getPrice() && !(currentDevice.getName().equals(lastMinimum.getName()) 
                        && currentDevice.getCategory().equals(lastMinimum.getCategory()))){
                            System.out.println(++counter + ". Category: " + currentDevice.getCategory() + ", Name: " + currentDevice.getName() +
                            ", Price: " + currentDevice.getPrice() + "$" + ", Quantity: " + currentDevice.getQuantity());
                    }
                }
            }
            if(minElement != max){
                System.out.println(++counter + ". Category: " + minElement.getCategory() + ", Name: " + minElement.getName() +
                                        ", Price: " + minElement.getPrice() + "$" + ", Quantity: " + minElement.getQuantity());
            }
            lastMinimum = minElement;
        }
    }

    /**
    * Calculates the total value of the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @return The total value of all devices in the inventory.
    */
    public double calculateTotalValue(){
        double totalValue = 0;
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                totalValue += currentDevice.getPrice() * currentDevice.getQuantity();
            }
        }
        return totalValue;
    } 

    /**
    * Restocks a device in the inventory.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @param name The name of the device to restock.
    * @param quantity The quantity to add or remove.
    * @return true if restocking successful, false otherwise.
    */
    private boolean restock(String name, int quantity){
        name = name.replaceAll("\\s", "");
        for(ArrayList<Device> List : Devices)
        {
            for(Device currentDevice : List)
            {
                if(currentDevice.getName().equalsIgnoreCase(name)){
                    currentDevice.setQuantity(currentDevice.getQuantity() + quantity);
                    if(quantity >= 0){
                        System.out.println(currentDevice.getName() + " restocked. New quantity: " + currentDevice.getQuantity());
                    } else {
                        System.out.println(currentDevice.getName() + " reduced. New quantity: " + currentDevice.getQuantity());
                    }
                    return true;
                }
            }
        }
        return false;
    }
    /**
    * Restocks a device in the inventory based on user input.
    * Time Complexity: Depends on restock() method
    * so O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    * @throws WrongMethodTypeException If an invalid operation is entered.
    */
    public void restockADevice() throws WrongMethodTypeException{
        System.out.print("Enter the name of the device to restock: ");
        Scanner in = new Scanner(System.in);
        String name = in.nextLine();
        System.out.print("Do you want to add or remove stock?(Add/Remove): ");
        String operation = in.nextLine();
        System.out.print("Enter the quantity to add: ");
        int quantity = in.nextInt();
        switch(operation){
            case "Add":
            case "add":
                break;
            case "Remove":
            case "remove":
                quantity *= -1;
                break;
            default:
        
                throw new WrongMethodTypeException();
        }
        restock(name, quantity);
    }

    /**
    * Exports an inventory report to a text file.
    * Time Complexity: O(n), where n is the total number of Device objects in all the ArrayLists in 'Devices'.
    */
    public void exportInventoryReport(){
        try{
            FileWriter writer = new FileWriter("Report.txt");
            writer.write("Electronics Shop Inventory Report\n\n");
            writer.write("---------------------------------------\n");
            writer.write("| No. | Category | Name | Price | Quantity |\n");
            writer.write("---------------------------------------\n");
            int counter = 1;
            for(ArrayList<Device> List : Devices)
            {
                for(Device currentDevice : List)
                {
                    writer.write("| " + counter + " |" + currentDevice.getCategory() + " | " + currentDevice.getName() + " | " +
                                    currentDevice.getPrice() + "$  | " + currentDevice.getQuantity()  + " |\n");
                    ++counter;
                }
            }
            writer.write("---------------------------------------\n\n");
            writer.write("Summary:\n");
            writer.write("- Total Number of Devices: " + (counter - 1) + "\n");
            writer.write("- Total Inventory Value: " + calculateTotalValue() + "$\n\n");
            writer.write("End of Report");
            writer.close();
        }
        catch(IOException e){
            System.out.println("An error occurred while writing to the file.");
            e.printStackTrace();
        }
    }
}
