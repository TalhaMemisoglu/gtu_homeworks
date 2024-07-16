package Hmw08;

import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        SocialNetworkGraph network = new SocialNetworkGraph();

        int menu;
        do{
            System.out.println("===== Social Network Analysis Menu =====");
            System.out.println("1. Add person");
            System.out.println("2. Remove person");
            System.out.println("3. Add friendship");
            System.out.println("4. Remove friendship");
            System.out.println("5. Find shortest path");
            System.out.println("6. Suggest friends");
            System.out.println("7. Count clusters");
            System.out.println("8. Exit");
            System.out.println("9. Test");
            System.out.print("> ");
            menu = scanner.nextInt();
            scanner.nextLine();
            switch(menu){
                case 1:
                    System.out.print("Enter name: ");
                    String name = scanner.nextLine();
                    System.out.print("Enter age: ");
                    int age = scanner.nextInt();
                    scanner.nextLine();
                    System.out.print("Enter hobbies (comma-seperated): ");
                    String hobbies = scanner.nextLine();
                    List<String> hobbiesList = Arrays.asList(hobbies.split(","));
                    network.addPerson(name, age, hobbiesList);
                    break;
                case 2:
                    System.out.print("Enter name:");
                    name = scanner.nextLine();
                    network.removePerson(name, null);
                    break;
                case 3:
                    System.out.print("Enter first person's name: ");
                    name = scanner.nextLine();
                    System.out.print("Enter second person's name: ");
                    String name2 = scanner.nextLine();
                    network.addFriendship(name, name2);
                    break;
                case 4:
                    System.out.print("Enter first person's name: ");
                    name = scanner.nextLine();
                    System.out.print("Enter second person's name: ");
                    name2 = scanner.nextLine();
                    network.removeFriendship(name, name2);
                    break;
                case 5:
                    System.out.print("Enter first person's name: ");
                    name = scanner.nextLine();
                    System.out.print("Enter second person's name: ");
                    name2 = scanner.nextLine();
                    network.findShortestPath(name, name2);
                    break;
                case 6:
                    System.out.print("Enter person's name: ");
                    name = scanner.nextLine();
                    System.out.print("Enter maximum number of friends to suggest: ");
                    int num = scanner.nextInt();
                    network.suggestFriends(name, num);
                    break;
                case 7:
                    System.out.println("Counting clusters in the social network...");
                    network.countClusters();
                    break;
                case 8:
                    break;
                case 9: 
                    test();
                    break;
                default:
                    System.out.println("Wrong typing!");
            }
        } while(menu != 8);
        scanner.close();
    }

    /**
     * Test method to demonstrate the functionality of the social network graph.
     */
    public static void test(){
        SocialNetworkGraph network = new SocialNetworkGraph();

        // Adding some people for demonstration
        network.addPerson("John Doe", 25, Arrays.asList("reading", "hiking", "cooking"));
        network.addPerson("Jane Smith", 22, Arrays.asList("swimming", "cooking"));
        network.addPerson("Alice Johnson", 27, Arrays.asList("hiking", "painting"));
        network.addPerson("Bob Brown", 30, Arrays.asList("reading", "swimming"));
        network.addPerson("Emily Davis", 28, Arrays.asList("running", "swimming"));
        network.addPerson("Frank Wilson", 26, Arrays.asList("reading", "hiking"));

        // Adding friendships for demonstration
        network.addFriendship("John Doe", "Jane Smith");
        network.addFriendship("John Doe", "Alice Johnson");
        network.addFriendship("Jane Smith", "Bob Brown");
        //network.addFriendship("Bob Brown", "Alice Johnson");
        network.addFriendship("Emily Davis", "Frank Wilson");

        //Suggesting Friends
        String name = "Alice Johnson";
        List<Person> sgtFriend = network.suggestFriends(name, 3);
        System.out.println("\nSuggested friends for " + name + ":");
        for(Person person : sgtFriend){
            System.out.println(person.getName());
        }
        System.out.print("\n");

        // Finding shortest path for demonstration
        network.findShortestPath("John Doe", "Bob Brown");

        // Counting clusters for demonstration
        network.countClusters();
    }
}