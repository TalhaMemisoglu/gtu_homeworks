package Hmw08;

import java.util.*;
import java.util.stream.Collectors;

/**
* A class representing a social network graph where people are nodes and friendships are edges.
*/
public class SocialNetworkGraph {
    Map<String, Person> people = new HashMap<>();
    Map<Person, List<Person>> friendships = new HashMap<>();

    /**
     * Adds a person to the social network.
     *
     * @param name    The name of the person.
     * @param age     The age of the person.
     * @param hobbies The hobbies of the person.
     */
    public void addPerson(String name, int age, List<String> hobbies) {
        Person person = new Person(name, age, hobbies);
        people.put(name, person);
        friendships.put(person, new ArrayList<>());
        System.out.println("The Person added: " + person);
    }

    /**
     * Removes a person from the social network.
     *
     * @param name The name of the person to be removed.
     * @param time The time of the removal (not used in the current implementation).
     */
    public void removePerson(String name, Date time){
        Person deletedPerson = people.get(name);
        if(deletedPerson != null){
            people.remove(deletedPerson.getName());
            friendships.remove(deletedPerson);

            for (List<Person> friends : friendships.values()) {
                friends.remove(deletedPerson);
            }
            System.out.println ("Person removed: " + deletedPerson);
        } else {
            System.out.println("The Person not found.");
        }
    }

    /**
     * Adds a friendship between two people in the social network.
     *
     * @param name1 The name of the first person.
     * @param name2 The name of the second person.
     */
    public void addFriendship(String name1, String name2) {
        Person person1 = people.get(name1);
        Person person2 = people.get(name2);
        if (person1 != null && person2 != null) {
            friendships.get(person1).add(person2);
            friendships.get(person2).add(person1);
            System.out.println("Friendship added between " + person1.getName() + " and " + person2.getName());
        } else {
            System.out.println("One or both persons not found in the network.");
        }
    }

    /**
     * Removes a friendship between two people in the social network.
     *
     * @param name1 The name of the first person.
     * @param name2 The name of the second person.
     */
    public void removeFriendship(String name1, String name2){
        Person person1 = people.get(name1);
        Person person2 = people.get(name2);
        if(person1 != null && person2 != null){
            friendships.get(person1).remove(person2);
            friendships.get(person2).remove(person1);
            System.out.println("Friendship removed between " + person1.getName() + " and " + person2.getName());
        } else {
            System.out.println("One or both persons not found in the network.");
        }
    }

    /**
     * Finds and prints the shortest path between two people using Breadth-First Search (BFS).
     *
     * @param startName The name of the starting person.
     * @param endName   The name of the ending person.
     */
    public void findShortestPath(String startName, String endName) {
        if(people.containsKey(startName) && people.containsKey(endName)){
            //Perform breadth‐first search.
            Map<Person, Person> prev = bfs(people.get(startName), new HashSet<>(), new ArrayList<>());
            //Print the path.
            printPath(people.get(startName), people.get(endName), prev);
        } else {
            System.out.println("One or both persons not found in the network.");
        }
    }

    /**
     * Prints the shortest path between two people.
     *
     * @param start The starting person.
     * @param end   The ending person.
     * @param prev  A map of previous nodes in the path.
     */
    private void printPath(Person start, Person end, Map<Person, Person> prev) {
        Deque<String> thePath = new ArrayDeque<>();
        Person curr = end;
        while(curr != null){
            thePath.push(curr.getName());
            curr = prev.get(curr);
        }

        System.out.print("The Shortest path is: ");
        while (!thePath.isEmpty()) {
            System.out.print(thePath.pop());
            if(!thePath.isEmpty()){
                System.out.print("->");
            } else {
                System.out.print("\n");
            }
        }
    }

    /**
     * Counts and prints the number of clusters (connected components) in the social network.
     */
    public void countClusters() {
        Set<Person> visited = new HashSet<>();
        List<List<Person>> clusters = new ArrayList<>(); 

        for(Person person : people.values()){
            if(!visited.contains(person)){
                List<Person> cluster = new ArrayList<>();
                bfs(person, visited, cluster);
                clusters.add(cluster);
            }
        }

        System.out.println("Number of clusters found: " + clusters.size());
        int index = 1;
        for(List<Person> clusterEx : clusters){
            System.out.println("Cluster " + index + ":");
            for(Person person: clusterEx){
                System.out.println(person.getName());
            }
            System.out.print("\n");
            index++;
        }
    }

    /**
     * Performs Breadth-First Search (BFS) from a starting person.
     *
     * @param start    The starting person.
     * @param visited  A set of visited people.
     * @param cluster  A list to store the current cluster.
     * @return A map of previous nodes in the BFS tree.
     */
    private Map<Person,Person> bfs(Person start, Set<Person> visited, List<Person> cluster) {
        Queue<Person> queue = new LinkedList<>();
        Map<Person,Person> parent = new HashMap<>();
        parent.put(start, null);

        queue.add(start);
        visited.add(start);

        while (!queue.isEmpty()) {
            Person current = queue.poll();
            cluster.add(current);

            for (Person neighbor : friendships.get(current)) {
                if (!visited.contains(neighbor)) {
                    queue.add(neighbor);
                    visited.add(neighbor);
                    parent.put(neighbor, current);
                }
            }
        }

        return parent;
    }

    /**
     * Suggests friends for a person based on mutual friends and common hobbies.
     *
     * @param name           The name of the person for whom to suggest friends.
     * @param maxSuggestions The maximum number of friend suggestions.
     * @return A list of suggested friends.
     */
    public List<Person> suggestFriends(String name, int maxSuggestions){
        Person person = people.get(name);
        if(person != null){
            Map<Person, Double> scores = new HashMap<>();

            for(Person friend : friendships.get(person)){
                for(Person friendOfFriend : friendships.get(friend)){
                    if(!friendOfFriend.equals(person) && !friendships.get(person).contains(friendOfFriend)){
                        double score = calculateScore(person, friendOfFriend);
                        scores.put(friendOfFriend, scores.getOrDefault(friendOfFriend, 0.0) + score);
                    }
                }
            }
            return scores.entrySet().stream().sorted(Map.Entry.<Person, Double>comparingByValue().reversed())
                    .limit(maxSuggestions).map(Map.Entry::getKey).collect(Collectors.toList());
        } else {
            System.out.println("The Person not found.");
            return null;
        }
    }
    
    /**
     * Calculates a friendship score based on mutual friends and common hobbies.
     *
     * @param person    The person for whom to calculate the score.
     * @param candidate The candidate friend.
     * @return The calculated score.
     */
    private double calculateScore(Person person, Person candidate){
        long mutualFriends = friendships.get(person).stream().filter(friendships.get(candidate)::contains).count();
        long commonHobbies = person.getHobbies().stream().filter(candidate.getHobbies()::contains).count();

        return mutualFriends * 1.0 + commonHobbies * 0.5;
    }
}