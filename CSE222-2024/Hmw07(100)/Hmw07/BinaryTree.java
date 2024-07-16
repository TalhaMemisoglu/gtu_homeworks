package Hmw07;

import java.io.Serializable;

public class BinaryTree<E> implements Serializable {
    protected static class Node<E> implements Serializable{
        public E data;
        public Node<E> left;
        public Node<E> right;

        public Node(E data){
            this.data = data;
            left = null;
            right = null;
        }

        public String toString(){
            return data.toString();
        }
    }

    protected Node<E> root;

    public BinaryTree() {
        root = null;
    }

    protected BinaryTree(Node<E> root){
        this.root = root;
    }

    public BinaryTree(E data, BinaryTree<E> lefTree, BinaryTree<E> rightTree){
        root = new Node<>(data);
        if(lefTree != null){
            root.left = lefTree.root;
        } else {
            root.left = null;
        }
        if(rightTree != null) {
            root.right = rightTree.root;
        } else {
            root.right = null;
        }
    }
    
    public E getData(){
        return root.data;
    }

    public BinaryTree<E> getLeftSubtree(){
        if(root != null && root.left != null){
            return new BinaryTree<>(root.left);
        }
        else{
            return null;
        }
    }

    public BinaryTree<E> getRightSubtree() {
        if(root != null && root.right != null){
            return new BinaryTree<>(root.right);
        } else {
            return null;
        }
    }

    /** Determine whether this tree is a leaf.
     @return true if the root has no children
    */
    public boolean isLeaf() {
        return (root.left == null && root.right == null);
    }

    /** Converts a sub‐tree to a string.
     Performs a preorder traversal.
    @param node The local root
    @param depth The depth
    @param sb The StringBuilder to save the output
    */
    private void toString(Node<E> node, int depth, StringBuilder sb){
        for(int i = 1; i <depth; i++){
            sb.append("  ");
        }
        if(node == null){
            sb.append("null\n");
        } else {
            sb.append(node.toString());
            sb.append("\n");
            toString(node.left, depth + 1, sb);
            toString(node.right, depth + 1, sb);
        }
    }
    
    public String toString() {
        StringBuilder sb = new StringBuilder();
        toString(root, 1, sb);
        return sb.toString();
    }
}
