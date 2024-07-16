package Hmw01;

import java.util.Scanner;

public class Main {
    public static void main(String args[]){
        Database db = new Database("Hmw01\\content.txt");
        db.readFile();
        
        boolean status = true;
        Scanner input = new Scanner(System.in);
        int ID;
        
        do{
            System.out.println("Please Enter your ID:");
            ID = input.nextInt();
            if(ID > 0){
                status = db.print_Informations(ID);/**If given ID is not valid, status is false. */
            } else {
                System.out.println("ID is not be negative!");
            }
        } while(status);
        input.close();
    }
}
