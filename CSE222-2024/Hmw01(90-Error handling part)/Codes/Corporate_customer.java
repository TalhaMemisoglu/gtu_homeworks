package Hmw01;

public class Corporate_customer extends Customer{
    private String company_name;

    public Corporate_customer(String name, String surname, String address, 
                              String phone, int ID, int operator_ID, String company_name){
        super(name, surname, address, phone, ID, operator_ID);
        this.company_name = company_name;
    }
    /**Print informations about the customer.*/
    public void print_customer(){
        super.print_customer();
        System.out.println("Company name: " + company_name);
    }
}
