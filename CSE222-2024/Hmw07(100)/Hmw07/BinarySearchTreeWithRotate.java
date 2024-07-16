package Hmw07;

public class BinarySearchTreeWithRotate<E extends Comparable<E>> extends BinarySearchTree<E>{
    /**
     * Performs a right rotation on the given node.
     * 
     * @param node The node to perform the rotation on.
     * @return The new root of the subtree after rotation.
     */
    protected Node<E> rotateRight(Node<E> node){
        Node<E> temp = node.left;
        node.left = temp.right;
        temp.right = node;
        return temp;
    }

    /**
     * Performs a left rotation on the given node.
     * 
     * @param node The node to perform the rotation on.
     * @return The new root of the subtree after rotation.
     */
    protected Node<E> rotateLeft(Node<E> node){
        Node<E> temp = node.right;
        node.right = temp.left;
        temp.left = node;
        return temp;
    }
}
