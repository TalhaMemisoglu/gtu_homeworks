package Hmw08;

import java.util.*;

public class Person {
    private String name;
    private int age;
    private List<String> hobbies;
    private Date timestamp;

    public Person(String name, int age, List<String> hobbies) {
        this.name = name;
        this.age = age;
        this.hobbies = new ArrayList<>(hobbies);
        this.timestamp = new Date();
    }

    public String getName(){
        return name;
    }

    public int getAge(){
        return age;
    }

    public List<String> getHobbies(){
        return hobbies;
    }

    public Date getDate(){
        return timestamp;
    }

    @Override
    public String toString() {
        return name + " (Age: " + age + ", Hobbies: " + hobbies + ")";
    }
}
