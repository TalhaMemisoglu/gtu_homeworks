package GTUContainer;
/**
 * An iterator interface in Java for iterating over elements of type T.
 * 
 * @param <T> The type of elements in the iterator.
 */
public interface JavaIterator <T> {
    /*
     * Checks if there is a next element in the iterator.
     */
    boolean hasNext();
    /**
     *  Retrieves the next element in the iterator.
     * 
     * @return The next element in the iterator.
     */
    T next();
}
