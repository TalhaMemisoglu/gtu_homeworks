package Hmw07;

public class BinarySearchTree<E extends Comparable<E>>
             extends BinaryTree<E> implements SearchTree<E> {

    /** Return value from the public add method. */
    protected boolean addReturn;

    /** Return value from the public delete method. */
    protected E deleteReturn;

    public boolean contains(E target){
        if(find(root, target) != null)
            return true;
        return false;
    }
    
    public E find(E target){
        return find(root, target);
    }

    /** Iterator find method.
    *@param localRoot The local subtree's root
    *@param target The object being sought
    *return The object, if found, otherwise null
    */
    private E find(Node<E> Root, E target){
        Node<E> currNode = Root;
        E targetData = null;
        while(currNode != null){
            int compResult = target.compareTo(currNode.data);
            if(compResult == 0){
                targetData = currNode.data;
                break;
            } else if(compResult < 0) {
                currNode = currNode.left;
            } else {
                currNode = currNode.right;
            }
        }
        return targetData;
    }

    public boolean add(E item) {
        root = add(root, item);
        return addReturn;
    }

    /** Recursive add method.
    *post: The data field addReturn is set true if the item is added to
    *the tree, false if the item is already in the tree.
    *@param localRoot The local root of the subtree
    *@param item The object to be inserted
    *@return The new local root that now contains the
    *inserted item
    */
    private Node<E> add(Node<E> localRoot, E item){
        if(localRoot == null){
            addReturn = true;
            return new Node<>(item);
        } else if(item.compareTo(localRoot.data) == 0) {
            addReturn = false;
            return localRoot;
        } else if(item.compareTo(localRoot.data) < 0) {
            localRoot.left = add(localRoot.left, item);
            return localRoot;
        } else {
            localRoot.right = add(localRoot.right, item);
            return localRoot;
        }
    }

    public boolean remove(E target){
        root = delete(root, target);
        if(deleteReturn != null)
            return true;
        return false;
    }

    public E delete(E target){
        root = delete(root, target);
        return deleteReturn;
    }

    private Node<E> delete(Node<E> localRoot, E item) {
        if(localRoot == null){
            deleteReturn = null;
            return localRoot;
        }
        int compResult = item.compareTo(localRoot.data);
        if(compResult < 0) {
            localRoot.left = delete(localRoot.left, item);
            return localRoot;
        }
        else if(compResult > 0) {
            localRoot.right = delete(localRoot.right, item);
            return localRoot;
        } else {
            deleteReturn = localRoot.data;
            if(localRoot.left == null) {
                // If there is no left child, return right child
                // which can also be null.
                return localRoot.right;
            }
            else if(localRoot.right == null){
                 // If there is no right child, return left child.
                return localRoot.left;
            } else {
                if(localRoot.left.right == null){
                    // The left child has no right child.
                    // Replace the data with the data in the
                    // left child.
                    localRoot.data = localRoot.left.data;
                    // Replace the left child with its left child
                    localRoot.left = localRoot.left.left;
                    return localRoot;
                } else {
                    // Search for the inorder predecessor (ip) and
                    // replace deleted node's data with ip.
                    localRoot.data = findLargestChild(localRoot.left);
                    return localRoot;
                }
            }
        }
    }

    /** Find the node that is the
    *inorder predecessor and replace it
    *with its left child (if any).
    *post: The inorder predecessor is removed from the tree.
    *@param parent The parent of possible inorder
    *              predecessor (ip)
    *@return The data in the ip
    */
    private E findLargestChild(Node<E> parent) {
        if(parent.right.right == null){
            E returnValue = parent.right.data;
            parent.right = parent.right.left;
            return returnValue;
        } else {
            return findLargestChild(parent.right);
        }
    }
}
