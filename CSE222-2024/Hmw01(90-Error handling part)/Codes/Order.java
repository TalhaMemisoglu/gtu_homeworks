package Hmw01;

public class Order {
    private String product_name;
    private int count;
    private int total_price;
    private int status;
    private int customer_ID;

    public Order(String product_name, int count, int total_price, int status, int customer_ID){
        this.product_name = product_name;
        this.count = count;
        this.total_price = total_price;
        this.status = status;
        this.customer_ID = customer_ID;
    }
    public int get_customer_ID(){
        return customer_ID;
    }

    private void print_product_name(){
        System.out.print("Product Name: " + product_name + " - ");
    }
    private void print_count(){
        System.out.print("Count: " + count + " - ");
    }
    private void print_total_price(){
        System.out.print("Total Price: " + total_price + " - ");
    }
    private void print_Status(){
        System.out.print("Status: ");
        switch(status){
            case 0:
                System.out.print("Initialized.\n");
                break;
            case 1:
                System.out.print("Processing.\n");
                break;
            case 2:
                System.out.print("Completed.\n");
                break;
            case 3:
                System.out.print("Cancelled.\n");
                break;
            default:
                System.out.print("Wrong Type!");
        }
    }
    public void print_order(){
        print_product_name();
        print_count();
        print_total_price();
        print_Status();
    }
}
