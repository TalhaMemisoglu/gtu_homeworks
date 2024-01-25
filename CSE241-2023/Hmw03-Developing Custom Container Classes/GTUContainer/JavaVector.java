package GTUContainer;
/**
 * A generic vector implementation in Java, implementing the JavaContainer interface.
 * @see GTUContainer.JavaContainer
 * 
 * @param <T> The type of elements in the vector.
 */
public class JavaVector<T> implements JavaContainer<T>{
    /**
     * Default capacity for the vector.
     */
    private static final int DEFAULT_CAP = 10;
    /**
     * Array to store elements.
     */
    private T[] elements;
    /**
     * Capacity of the vector.
     */
    private int cap;
    /**
     * Number of used elements in the vector.
     */
    private int used;

    /**
     * Constructs a vector with the default capacity.
     */
    public JavaVector()
    {
        elements = (T[]) new Object[DEFAULT_CAP];
        cap = DEFAULT_CAP;
        used = 0;
    }
    /**
     * Constructs a vector with the specified capacity.
     * 
     * @param capacity
     */
    public JavaVector(int capacity)
    {
        if(capacity > 0)
        {
            elements = (T[]) new Object[capacity];
            cap = capacity;
            used = 0;
        }
    }
    /**
     * Adds an element to the vector if there is enough space.
     * 
     * @param element The element to be added
     * {@inheritDoc}
     */
    @Override
    public void add(T element)
    {
        if(used < cap)
        {
            elements[used++] = element;
        }else{
            System.out.println("Capacity exceeded!\n");
        }
    }
    /**
     * Removes one time the specified element from the vector.
     * 
     * @param element The element to be removed.
     * {@inheritDoc}
     */
    @Override
    public void remove(T element)
    {
        T[] temp = (T[]) new Object[cap];
        boolean found = false;
        for(int i=0, iOfTemp = 0; i<used; i++, iOfTemp++)
        {
            if(elements[i] == element && !found)
            {
                iOfTemp--;
                found = true;
            } else {
                temp[iOfTemp] = elements[i];
            }
        }
        used--;
        elements = temp;
    }
    /**
     * Returns the number of elements in the vector.
     * 
     * @return The number of elements in the vector.
     * {@inheritDoc}
     */
    @Override
    public int size()
    {
        return used;
    }

    /**
     * Iterator for iterating over elements in the vector, implementing JavaIterator.
     * @see GTUContainer.JavaIterator
     */
    public class JavaVectorIterator implements JavaIterator<T>
    {
        /**
         * Current index in the vector.
         */
        private int current = 0;
        /**
         * Checks if there is a next element in the iterator.
         * 
         * @return {@code true} if there is a next element, {@code false} otherwise.
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
         * Retrieves the next element int the iterator.
         * 
         * @return The next element in the iterator.
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
     * Returns an iterator for iterating over elements in the vector.
     * 
     * @return An iterator for the vector.
     * {@inheritDoc}
     */
    @Override
    public JavaIterator<T> getIterator()
    {
        return new JavaVectorIterator();
    }
    /**
     * Returns a string representation of the vector.
     * 
     * @return A string representation of the vector.
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
     * Checks if this vector is equal to another object.
     * 
     * @param obj The object to compare with.
     * @return {@code true} if the vectors are equal, {@code false} otherwise.
     *
     */
    @Override
    public boolean equals(Object obj)
    {
        if(this == obj) return true;
        if(obj == null || getClass() != obj.getClass()) return false;
        JavaVector<T> javaVector = (JavaVector<T>) obj;
        
        if(used == javaVector.used)
        {
            for(int i=0; i<used; i++)
            {
                if(elements[i] != javaVector.elements[i])
                    return false;
            }
        }
        return true;
    }
}
