package Hmw01;

public class Person {
    private String name;
    private String surname;
    private String address;
    private String phone;
    private int ID;

    public Person(String name, String surname, String address, String phone, int ID){
        this.name = name;
        this.surname = surname;
        this.address = address;
        this.phone = phone;
        this.ID = ID;
    }
    public int get_ID(){
        return ID;
    }
    /**Print informations about the person.*/
    public void print_Person(){
        System.out.println("Name & Surname: " + name + " " + surname);
        System.out.println("Address: " + address);
        System.out.println("Phone: " + phone);
        if(ID > 0)
            System.out.println("ID: " + ID);
        else{
            System.out.println("ID: Unidentified ID");
        }
    }
}
