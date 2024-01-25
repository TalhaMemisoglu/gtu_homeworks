package GTUContainer;

/**
 * A generic container interface in Java for holding elements of type T.
 * 
 * @param <T> The type of elements that uses in functions
 */
public interface JavaContainer<T> {
    /**
     * Adds element to the container.
     * @param element
     */
    void add(T element);
    /**
     * Removes the specified element from the container.
     * @param element
     */
    void remove(T element);
    /**
     * Returns the number of elements in the container.
     * @return The number of elements in the container.
     */
    int size();
    /**
     * Returns an iterator for iterating over elements in the container.
     * 
     * @return An iterator for the container.
     */
    JavaIterator<T> getIterator();
}
