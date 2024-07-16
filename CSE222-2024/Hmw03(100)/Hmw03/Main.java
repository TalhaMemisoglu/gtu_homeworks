package Hmw03;

import java.util.Scanner;

public class Main {
    public static void main(String args[]){
        try{
            Inventory crInventory = new Inventory();
            Scanner in = new Scanner(System.in);
            System.out.println("Welcome to the Electronics Inventory Management System!\n");
            System.out.println("Please select an option:");
            System.out.println("1. Add a new device");
            System.out.println("2. Remove a device");
            System.out.println("3. Update device details");
            System.out.println("4. List all devices");
            System.out.println("5. Find the cheapest device");
            System.out.println("6. Sort devices by price");
            System.out.println("7. Calculate total inventory value");
            System.out.println("8. Restock a device");
            System.out.println("9. Export inventory report");
            System.out.println("0. Exit");
            int menu = 1;
            while(menu != 0){
                System.out.print("\nEnter your choice: ");
                menu = in.nextInt();
                switch(menu){
                    case 0:
                        System.out.println("Exiting the program...");
                        break;
                    case 1:
                        crInventory.addDevice();
                        break;
                    case 2:
                        crInventory.removeDevice();
                        break;
                    case 3:
                        crInventory.updateDevice();
                        break;
                    case 4:
                        crInventory.displayAllDevices();
                        break;
                    case 5:
                        crInventory.findCheapestDevice();
                        break;
                    case 6:
                        crInventory.sortDevicesByPrice();
                        break;
                    case 7:
                        System.out.println("Total inventory value: " + crInventory.calculateTotalValue() + "$");
                        break;
                    case 8:
                        crInventory.restockADevice();
                        break;
                    case 9:
                        crInventory.exportInventoryReport();
                        break;
                    default:
                        System.out.println("Wrong typing, please try again!");
                }
            }
            in.close();
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}
