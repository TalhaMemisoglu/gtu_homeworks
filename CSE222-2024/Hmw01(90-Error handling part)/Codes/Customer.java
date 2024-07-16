package Hmw01;

public class Customer extends Person{
    private Order[] orders = new Order[20];
    private int used = 0;
    private int operator_ID;
    
    public Customer(String name, String surname, String address, String phone, int ID, int operator_ID){
        super(name, surname, address, phone, ID);
        this.operator_ID = operator_ID;
    }
    public int getOperatorID(){
        return operator_ID;
    }
    /**Print informations about the customer.*/
    public void print_customer(){
        super.print_Person();
        System.out.println("Operator ID: " + operator_ID);
    }
    /**Print informations of customers under the customers.*/
    public void print_orders(){
        int orderNum = 1;
        for(Order order : orders){
            if(order instanceof Order){
                System.out.print("Order #" + (orderNum) + " => ");
                order.print_order();
                orderNum++;
            }
        }
    }
    public void define_orders(Order[] _orders){
        for(Order order : _orders){
            if(order instanceof Order)
            {
                if(order.get_customer_ID() == super.get_ID()){
                    orders[used++] = order;
                }
            }
        }
    }
}
