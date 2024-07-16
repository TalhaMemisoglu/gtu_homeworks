package Hmw01;

import java.io.File;
import java.util.Scanner;

public class Database {
    private Order[] orders = new Order[20];
    /**Used space counter in orders array*/
    private int used_orders = 0;
    private Customer[] customers = new Customer[20];
    /**Used space counter in customers array*/
    private int used_customers = 0;
    private Operator[] operators = new Operator[20];
    /**Used space counter in operators array */
    private int used_operators = 0;
    /**File path that will be read by program.*/
    private String filePath;

    public Database(String filePath){
        this.filePath = filePath;
    }
    
    /**Determine whether same ID exist or not.*/
    private boolean ID_Check(int ID){
        for(int i=0; i < used_customers; i++)
        {
            if(customers[i].get_ID() == ID)
                return false;
        }
        for(int i=0; i < used_operators; i++)
        {
            if(operators[i].get_ID() == ID)
                return false;
        }
        return true;
    }
    /**Read the line and store the datas.*/
    private void defineLine(String line){
        Scanner input = new Scanner(line);
        input.useDelimiter(";");
        String type = input.next();
        if(type.equals("order")){
            try{
                String productName = input.next();
                int count = input.nextInt();
                int total_price = input.nextInt();
                int status = input.nextInt();
                int customer_ID = input.nextInt();
                if(count > 0 && total_price >= 0 && status >= 0 && customer_ID > 0) /**checking whether integer is positive or not.*/
                    orders[used_orders++] = new Order(productName, count, total_price, status, customer_ID);
            }catch(Throwable e){} /**If there is a problem in that line, just ignore it.*/
        }
        else if(type.equals("retail_customer")){
            try{
                String name = input.next();
                String surname = input.next();
                String address = input.next();
                String phone = input.next();
                int ID = input.nextInt();
                int operator_ID = input.nextInt();
                if(ID_Check(ID) && operator_ID > 0){ /**checking whether operator ID is positive or not and ID is valid or not.*/
                    customers[used_customers++] = new Retail_customer(name, surname, address, phone, ID, operator_ID);
                }
            } catch(Throwable e) {} /**If there is a problem in that line, just ignore it.*/
        }
        else if(type.equals("corporate_customer"))
        {
            try{
                String name = input.next();
                String surname = input.next();
                String address = input.next();
                String phone = input.next();
                int ID = input.nextInt();
                int operator_ID = input.nextInt();
                String company_name = input.next();
                if(ID_Check(ID) && operator_ID > 0){ /**checking whether operator ID is positive or not and ID is valid or not.*/
                    customers[used_customers++] = new Corporate_customer(name, surname, address, phone, ID, operator_ID, company_name);
                }
            } catch(Throwable e) {} /**If there is a problem in that line, just ignore it.*/
        }
        else if(type.equals("operator"))
        {
            try
            {
                String name = input.next();
                String surname = input.next();
                String address = input.next();
                String phone = input.next();
                int ID = input.nextInt();
                int wage = input.nextInt();
                if(ID_Check(ID) && wage >= 0){ /**checking whether wage is positive or not and ID is valid or not.*/
                    operators[used_operators++] = new Operator(name, surname, address, phone, ID, wage);
                }
            } catch(Throwable e) {} /**If there is a problem in that line, just ignore it.*/
        }
        input.close();
    }
    
    /**Reading function from file given.*/
    public void readFile(){
        try{
            Scanner input = new Scanner(new File(filePath));
            String line;
            while(input.hasNextLine())
            {
                line = input.nextLine();
                defineLine(line);
            }
            for(int i=0; i<used_customers; ++i){
                customers[i].define_orders(orders);
            }
            for(int i=0; i<used_operators; ++i){
                operators[i].define_customers(customers);
            }
            input.close();
        }
        catch(Throwable e)
        {
            System.out.println("File not found.");
        }
    }

    /**Printing informations about person by given ID
     *If given ID is not valid, return false. 
    */
    public boolean print_Informations(int ID)
    {
        boolean status = false;
        for(int i=0; i<used_operators; ++i){
            if(operators[i].get_ID() == ID)
            {
                System.out.println("*** Operator Screen ***");
                System.out.println("----------------------------");
                operators[i].print_operator();
                System.out.println("----------------------------");
                operators[i].print_customers();
                status = true;
            }
        }
        for(int i=0; i<used_customers; ++i){
            if(customers[i].get_ID() == ID)
            {
                System.out.println("*** Customer Screen ***");
                System.out.println("----------------------------");
                customers[i].print_customer();
                customers[i].print_orders();
                System.out.println("----------------------------");
                status = true;
            }
        }
        if(status == false)
        {
            System.out.println("No operator/customer was found with ID " + ID + ". Please try again.");
        }
        return status;
    }
}
