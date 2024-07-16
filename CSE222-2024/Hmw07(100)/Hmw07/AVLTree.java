package Hmw07;

public class AVLTree<E extends Comparable<E>> extends BinarySearchTreeWithRotate<E>{
    private static class AVLNode<E> extends Node<E> {
        public static final int LEFT_HEAVY = -1;
        public static final int BALANCED = 0;
        public static final int RIGHT_HEAVY = 1;
        private int balance;

        public AVLNode(E item){
            super(item);
            balance = BALANCED;
        }

        public String toString(){
            return super.toString() + ": "+ balance;
        }
    }
    private boolean increase;
    private boolean decrease;

    /**
     * Rebalances the AVL tree rooted at localRoot after a left subtree insertion.
     * 
     * @param localRoot The root of the AVL subtree to rebalance.
     * @return The new root of the AVL subtree after rebalancing.
     */
    private AVLNode<E> rebalanceLeft(AVLNode<E> localRoot){
        // Get the left child of the localRoot.
        AVLNode<E> leftChild = (AVLNode<E>) localRoot.left;
        // Left-Right case: leftChild is right heavy
        if(leftChild.balance > AVLNode.BALANCED){ //Left-Right case
            // Get the right child of the leftChild.
            AVLNode<E> leftRightChild = (AVLNode<E>) leftChild.right;
            if(leftRightChild != null){
                // Left-Right-Left case: leftRightChild is left heavy
                if(leftRightChild.balance < AVLNode.BALANCED){ //Left-Right-Left
                    leftChild.balance = AVLNode.BALANCED;
                    leftRightChild.balance = AVLNode.BALANCED;
                    localRoot.balance = AVLNode.RIGHT_HEAVY;
                } else { // Left-Right-Right case: leftRightChild is right heavy or balanced
                    leftChild.balance = AVLNode.LEFT_HEAVY;
                    leftRightChild.balance = AVLNode.BALANCED;
                    localRoot.balance = AVLNode.BALANCED;
                }
            }
            // Perform left rotation on leftChild.
            localRoot.left = rotateLeft(leftChild);
        } else { // Left-Left case: leftChild is left heavy or balanced
            // Both leftChild and localRoot will be balanced after rotation.
            leftChild.balance = AVLNode.BALANCED;
            localRoot.balance = AVLNode.BALANCED;
        }
        // Perform right rotation on localRoot and return the new root.
        return (AVLNode<E>) rotateRight(localRoot);
    }
    /**
     * Rebalances the AVL tree rooted at localRoot after a right subtree insertion.
     * 
     * @param localRoot The root of the AVL subtree to rebalance.
     * @return The new root of the AVL subtree after rebalancing.
     */
    private AVLNode<E> rebalanceRight(AVLNode<E> localRoot){
        // Get the right child of the localRoot.
        AVLNode<E> rightChild = (AVLNode<E>) localRoot.right;
        // Right-Left case: rightChild is left heavy
        if(rightChild.balance < AVLNode.BALANCED){ 
            // Get the left child of the rightChild
            AVLNode<E> rightLeftChild = (AVLNode<E>) rightChild.left;
            if(rightLeftChild != null){
                // Right-Left-Right case: rightLeftChild is right heavy or balanced
                if(rightLeftChild.balance > AVLNode.BALANCED){ //Right-Left-Right
                    rightChild.balance = AVLNode.BALANCED;
                    rightLeftChild.balance = AVLNode.BALANCED;
                    localRoot.balance = AVLNode.LEFT_HEAVY;
                } else { // Right-Left-Left case: rightLeftChild is left heavy
                    rightChild.balance = AVLNode.RIGHT_HEAVY;
                    rightLeftChild.balance = AVLNode.BALANCED;
                    localRoot.balance = AVLNode.BALANCED;
                }
            }
            // Perform right rotation on rightChild.
            localRoot.right = rotateRight(rightChild);
        } else { // Right-Right case: rightChild is right heavy or balanced
            // Both rightChild and localRoot will be balanced after rotation.
            rightChild.balance = AVLNode.BALANCED;
            localRoot.balance = AVLNode.BALANCED;
        }
        // Perform left rotation on localRoot and return the new root.
        return (AVLNode<E>) rotateLeft(localRoot);
    }
     /**
     * Decrements the balance factor of the given AVL node and checks if it becomes balanced.
     * 
     * @param node The AVL node whose balance factor is to be decremented.
     */
    private void decrementBalance(AVLNode<E> node){
        // Decrement the balance factor of the node.
        node.balance--;
        // If the balance factor becomes balanced, set increase and decrease flags accordingly.
        if(node.balance == AVLNode.BALANCED){
            increase = false;
            decrease = false;
        }
    }
    /**
     * Increments the balance factor of the given AVL node and checks if it becomes balanced.
     * 
     * @param node The AVL node whose balance factor is to be incremented.
     */
    private void incrementBalance(AVLNode<E> node){
        // Increment the balance factor of the node.
        node.balance++;
        // If the balance factor becomes balanced, set increase and decrease flags accordingly.
        if(node.balance == AVLNode.BALANCED){
            increase = false;
            decrease = false;
        }
    }
    /**
    * Adds an item to the AVL tree rooted at localRoot.
    * 
    * @param localRoot The root of the AVL subtree to which the item is added.
    * @param item The item to add to the AVL tree.
    * @return The root of the AVL subtree after adding the item.
    */
    private AVLNode<E> add(AVLNode<E> localRoot, E item){
        // If the subtree is empty, create a new node with the item.
        if(localRoot == null){
            addReturn = true;
            increase = true;
            return new AVLNode<E>(item);
        }
        // If the item is already in the tree, update flags accordingly.
        if(item.compareTo(localRoot.data) == 0){
            increase = false;
            addReturn = false;
        } else if(item.compareTo(localRoot.data) < 0){
            // If the item is less than the current node's data, add it to the left subtree.
            localRoot.left = add((AVLNode<E>) localRoot.left, item);
            // Check if the balance factor of the current node is out of balance after insertion.
            if(increase){
                decrementBalance(localRoot);
                // If the tree becomes left-heavy after insertion, perform left rebalancing.
                if(localRoot.balance < AVLNode.LEFT_HEAVY){
                    increase = false;
                    return rebalanceLeft(localRoot);
                }
            }
        } else if(item.compareTo(localRoot.data) > 0) {
            // If the item is greater than the current node's data, add it to the right subtree.
            localRoot.right = add((AVLNode<E>) localRoot.right, item);
            // Check if the balance factor of the current node is out of balance after insertion.
            if(increase){
                incrementBalance(localRoot);
                // If the tree becomes right-heavy after insertion, perform right rebalancing.
                if(localRoot.balance > AVLNode.RIGHT_HEAVY){
                    increase = false;
                    return rebalanceRight(localRoot);
                }
            }
        }
        // Return the modified subtree root.
        return localRoot;
    }
    /**
     * Adds an item to the AVL tree.
     * 
     * @param item The item to add to the AVL tree.
     * @return true if the item was successfully added, false otherwise.
     */
    @Override
    public boolean add(E item){
        increase = false;
        root = add((AVLNode<E>) root, item);
        return addReturn;
    }
    /**
    * Finds the largest child of the given parent node in the AVL tree.
    * 
    * @param parent The parent node whose largest child is to be found.
    * @return The data of the largest child of the given parent node.
    */
    private E findLargestChild(AVLNode<E> parent) {
        // If the right child of the parent node has no right child, it is the largest child.
        if(parent.right.right == null){
             // Save the data of the right child.
            E returnValue = parent.right.data;
            // Remove the right child from the parent by updating the reference.
            parent.right = parent.right.left;
            // Return the data of the removed node.
            return returnValue;
        } else {
            // If the right child of the parent has a right child, recursively find the largest child.
            return this.findLargestChild((AVLNode<E>) parent.right);
        }
    }
    /**
     * Deletes an item from the AVL tree rooted at localRoot.
     * 
     * @param localRoot The root of the AVL subtree from which to delete the item.
     * @param item The item to delete from the AVL tree.
     * @return The root of the AVL subtree after deleting the item.
     */
    private AVLNode<E> delete(AVLNode<E> localRoot, E item) {
        // If the subtree is empty, return null.
        if(localRoot == null){
            deleteReturn = null;
            decrease = false;
            return localRoot;
        }
        // If the item is found at the current node.
        if(item.compareTo(localRoot.data) == 0){
            // Set deleteReturn to the data of the node to be deleted.
            deleteReturn = localRoot.data;
            // Node with no left child.
            if(localRoot.left == null) {
                // Return the right child (which can be null).
                return (AVLNode<E>)localRoot.right;
            }
            // Node with no right child.
            else if(localRoot.right == null){
                decrease = true;
                // If there is no right child, return left child.
                return (AVLNode<E>)localRoot.left;
            } else {// Node with two children: Get the in-order predecessor (largest in the left subtree)
                if(localRoot.left.right == null){
                    decrease = true;
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
                    localRoot.data = findLargestChild((AVLNode<E>)localRoot.left);
                    localRoot.left = delete((AVLNode<E>) localRoot.left, localRoot.data);
                    // If decrease flag is set, increment the balance and rebalance if necessary.
                    if(decrease){
                        incrementBalance(localRoot);
                        if(localRoot.balance < AVLNode.BALANCED){
                            decrease = false;
                            return rebalanceLeft(localRoot);
                        }
                    }
                    return localRoot;
                }
            }
        } else if(item.compareTo(localRoot.data) < 0){
            //item < data
            localRoot.left = delete((AVLNode<E>) localRoot.left, item);
            // If decrease flag is set, increment the balance and rebalance if necessary.
            if(decrease){
                incrementBalance(localRoot);
                if(localRoot.balance < AVLNode.BALANCED){
                    decrease = false;
                    return rebalanceLeft(localRoot);
                }
            }
        } else if(item.compareTo(localRoot.data) > 0) {
            //item > data
            localRoot.right = delete((AVLNode<E>) localRoot.right, item);
            // If decrease flag is set, decrement the balance and rebalance if necessary.
            if(decrease){
                decrementBalance(localRoot);
                if(localRoot.balance > AVLNode.BALANCED){
                    decrease = false;
                    return rebalanceRight(localRoot);
                }
            }
        }
        return localRoot;
    }
    /**
     * Deletes an item from the AVL tree.
     * 
     * @param item The item to delete from the AVL tree.
     * @return The deleted item, or null if the item is not found.
     */
    @Override
    public E delete(E item) {
        root = delete((AVLNode<E>) root, item);
        return deleteReturn;
    }
    /**
     * Performs an in-order traversal of the AVL tree rooted at the given node.
     * 
     * @param node The root of the AVL subtree to traverse.
     */
    private void inOrderTraversal(AVLNode<E> node) {
        if (node != null) {
            inOrderTraversal((AVLNode<E>)node.left); // Traverse left subtree
            System.out.println(node.data); // Visit root
            inOrderTraversal((AVLNode<E>)node.right); // Traverse right subtree
        }
    }
    /**
    * Performs an in-order traversal of the AVL tree.
    */
    public void inOrderTraversal() {
        inOrderTraversal((AVLNode<E>)root); // Start traversal from root
    }
    /**
     * Performs a post-order traversal of the AVL tree rooted at the given node.
     * 
     * @param node The root of the AVL subtree to traverse.
     */
    private void postOrderTraversal(AVLNode<E> node){
        if(node!=null){
            postOrderTraversal((AVLNode<E>)node.left); // Traverse left subtree
            postOrderTraversal((AVLNode<E>)node.right); // Traverse right subtree
            System.out.println(node.data); // Visit root
        }
    }
    /**
    * Performs a post-order traversal of the AVL tree.
    */
    public void postOrderTraversal(){
        postOrderTraversal((AVLNode<E>)root); // Start traversal from root
    }
    /**
     * Performs a pre-order traversal of the AVL tree rooted at the given node.
     * 
     * @param node The root of the AVL subtree to traverse.
     */
    private void preOrderTraversal(AVLNode<E> node){
        if(node!=null){
            System.out.println(node.data); // Visit root
            preOrderTraversal((AVLNode<E>)node.left); // Traverse left subtree
            preOrderTraversal((AVLNode<E>)node.right); // Traverse right subtree
        }
    }
    /**
     * Performs a pre-order traversal of the AVL tree.
     */
    public void preOrderTraversal(){
        preOrderTraversal((AVLNode<E>)root); // Start traversal from root
    }
}