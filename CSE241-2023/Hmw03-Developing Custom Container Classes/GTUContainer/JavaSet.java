package GTUContainer;

/**
 * A generic set implementation in Java, implementing the JavaContainer.
 * @see GTUContainer.JavaContainer
 * 
 * @param <T> the of elements in the set
 */

public class JavaSet<T> implements JavaContainer<T> {
    /**
     * Default capacity for the set.
     */
    public static final int DEFAULT_CAP = 10;
    /**
     * Array to store elements.
     */
    private T[] elements;
    /**
     * Capacity of the set.
     */
    private int cap;
    /**
     * Number of used elements in the set.
     */
    private int used;
    /**
     * Constructs an empty set with the default capacity.
     */
    public JavaSet()
    {
        elements = (T[]) new Object[DEFAULT_CAP];
        cap = DEFAULT_CAP;
        used = 0;
    }
    /**
     * Constructs an empty set with the specified capacity.
     * 
     * @param capacity The initial capacity of the set.
     */
    public JavaSet(int capacity)
    {
        if(capacity > 0)
        {
            elements = (T[]) new Object[capacity];
            cap = capacity;
            used = 0;
        }
    }
    /**
     * Checks if the set already contains the given element.
     * 
     * @param element The element to check.
     * @return {@code true} if the set contains the element, {@code false} otherwise.
     */
    private boolean hasSameElement(T element)
    {
        for(int i=0; i<used; i++)
        {
            if(elements[i] == element)
                return true;
        }   
        return false;
    }
    /**
     * Adds an element to the set if it is not already present 
     * and there is enough space.
     * 
     * @param element The element to add.
     * {@inheritDoc}
     */
    @Override
    public void add(T element)
    {
        if(used < cap)
        {
            if(!hasSameElement(element))
                elements[used++] = element;
        } else {
            System.out.println("Capacity exceeded!\n");
        }
    }
    
    /**
     * Removes an element from the set.
     * 
     * @param element The element to remove
     * {@inheritDoc}
     */
    @Override
    public void remove(T element)
    {
        T[] temp = (T[]) new Object[cap];
        for(int i=0, iOfTemp = 0; i<used; i++, iOfTemp++)
        {
            if(elements[i] == element)
            {
                iOfTemp--;
                used--;
            } else {
                temp[iOfTemp] = elements[i];
            }
        }
        elements = temp;
    }
    /**
     * Returns the number of the elements in the set.
     * 
     * @return The size of the set.
     * {@inheritDoc}
     */
    @Override
    public int size()
    {
        return used;
    }

    /**
     * Iterator for iterating over elements in the set, implementing JavaIterator
     * @see GTUContainer.JavaIterator
     */
    public class JavaSetIterator implements JavaIterator<T>
    {
        /**
         * Current index in the set.
         */
        private int current = 0;
        /**
         * Checks if there is a next in the set.
         * 
         * @return {@code} true} if there is a next element, {@code false} otherwise
         */
        @Override
        public boolean hasNext(){
            if(current < used - 1) //Decrement by 1 for used variable is for index.
            {
                return true;
            } else {
                return false;
            }
        }
        /**
         * Retrieves the next element in the set.
         * 
         * @return The next element in the set.
         */
        @Override
        public T next()
        {
            if(hasNext())
            {
                return elements[++current];
            }
            return null;
        }
    }

    /**
     * Returns an iterator for iterating over elements in the set.
     * 
     * @return An iterator for the set.
     * {@inheritDoc}
     */
    @Override
    public JavaIterator<T> getIterator()
    {
        return new JavaSetIterator();
    }

    /**
     * Returns a string representation of the set.
     * 
     * @return A string representation of set.
     * @see java.lang.Object
     */
    @Override
    public String toString()
    {
        String stringArr = new String("[");
        for(int i=0; i<used - 1; i++)
        {
            stringArr += elements[i] + ", "; 
        }
        stringArr += elements[used - 1] + "]";

        return stringArr;
    }
    /**
     * Checks if this set is equal to another object.
     * 
     * @param obj The object to compare with.
     * @return {@code true} if the sets are equal, {@code false} otherwise.
     * @see java.lang.Object
     */
    @Override
    public boolean equals(Object obj)
    {
        if(this == obj) return true;
        if(obj == null || getClass() != obj.getClass()) return false;
        JavaSet<T> javaSet = (JavaSet<T>) obj;
        
        if(used == javaSet.used)
        {
            for(int i=0; i<used; i++)
            {
                if(elements[i] != javaSet.elements[i])
                    return false;
            }
        }

        return true;
    }
}