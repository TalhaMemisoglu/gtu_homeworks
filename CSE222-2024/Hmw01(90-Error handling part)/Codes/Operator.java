package Hmw01;

public class Operator extends Person{
    private int wage;
    private Customer[] customers = new Customer[20];
    private int used = 0;
    
    public Operator(String name, String surname, String address, String phone, int ID, int wage){
        super(name, surname, address, phone, ID);
        this.wage = wage;
    }
    /**Print informations about the operator. */
    public void print_operator(){
        super.print_Person();
        System.out.println("Wage: " + wage);
    }
    /**Print informations of customers under the operator.*/
    public void print_customers(){
        int customerCounter = 1;
        for(Customer customer : customers){
            if(customer instanceof Retail_customer){
                System.out.println("Customer #" + customerCounter++ + "  (a retail customer):");
                customer.print_customer();
                customer.print_orders();
                System.out.println("----------------------------");
            }
            else if(customer instanceof Corporate_customer){
                System.out.println("Customer #" + customerCounter++ + "  (a corporate customer):");
                customer.print_customer();
                customer.print_orders();
                System.out.println("----------------------------");
            }
        }
        if(customerCounter == 1){
            System.out.println("This operator doesn't have any customer.");
            System.out.println("----------------------------");
        }
    }
    public void define_customers(Customer[] _customers){
        for(Customer customer : _customers){
            if(customer instanceof Customer)
            {
                if(customer.getOperatorID() == super.get_ID()){
                    customers[used++] = customer;
                }
            }
        }
    }
}
