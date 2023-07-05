#include <stdio.h>
#include <stdlib.h>

typedef struct BST{
	struct Node* root;	
}BST;

typedef struct Node{
	double Value;
	struct Node* left;
	struct Node* right;
}Node;

struct Node* createNode(double value);
struct Node* insert(struct Node* root, double value);
void printTree(struct Node* root);
struct BST *generateBST(const char *filename);
struct Node* searchNode(struct Node* root, double value);
void addNode(struct BST* bst, double value);

int main(int argc, char *argv[]) 
{
	char filename[] = "input.txt";
	BST *mytree;
	mytree = generateBST(filename);
	
	printf("Number of Nodes: %d\n",countNodes(mytree->root));
	printf("Number of leafs: %d\n",countLeafNodes(mytree->root));
	printf("Max depth: %d\n",getMaxDepth(mytree->root));
	printTree(mytree->root);
	
	double wanted_value = 45;
	struct Node* wantedNode = (struct Node*)malloc(sizeof(struct Node));
	wantedNode = searchNode(mytree->root,wanted_value);
	printf("\nWanted Node: %lf\n",wantedNode->Value);
	
	double new_value = 19;
	addNode(mytree,new_value);
	
	return 0;
}

struct Node* createNode(double value) 
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->Value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, double value) 
{
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->Value) {
        root->left = insert(root->left, value);
    } else if (value > root->Value) {
        root->right = insert(root->right, value);
    }
    return root;
}

struct BST *generateBST(const char *filename)
{
	FILE *fptr;
	fptr = fopen(filename,"r");
	
	if (fptr == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

	struct Node* root = NULL;
    double value;
    while (fscanf(fptr, "%lf", &value) != EOF) 
	{
        root = insert(root, value);
    }
	struct BST *btree;
	btree = (BST*) malloc(sizeof(BST));
	btree->root = root;
    
	fclose(fptr);
	return btree;
}

void addNode(struct BST* bst, double value) 
{
    struct Node* newNode = createNode(value);
    if (bst->root == NULL) 
	{
        bst->root = newNode;
    } 
	else 
	{
        struct Node* current = bst->root;
        while (1) 
		{
            if (value < current->Value) 
			{
                if (current->left == NULL) 
				{
                    current->left = newNode;
                    break;
                }
                current = current->left;
            } 
			else if (value > current->Value) 
			{
                if (current->right == NULL) 
				{
                    current->right = newNode;
                    break;
                }
                current = current->right;
            } 
			else 
			{
                free(newNode);
                break;
            }
        }
    }
    
    printTree(bst->root);
}

struct Node* searchNode(struct Node* root, double value) 
{
    if (root == NULL || root->Value == value) {
        return root;
    }
    if (value < root->Value) {
        return searchNode(root->left, value);
    }
    return searchNode(root->right, value);
}

int countNodes(struct Node* root) 
{
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeafNodes(struct Node* root) 
{
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    return countLeafNodes(root->left) + countLeafNodes(root->right);
}

int getMaxDepth(struct Node* root) 
{
    if (root == NULL) {
        return 0;
    }
    int leftDepth = getMaxDepth(root->left);
    int rightDepth = getMaxDepth(root->right);
    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}

void printTree(struct Node* root) 
{
    if (root != NULL) {
        printTree(root->left);
        printf("%.2lf ", root->Value);
        printTree(root->right);
    }
}
