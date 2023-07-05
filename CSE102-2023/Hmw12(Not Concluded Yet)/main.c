#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node {
	void* data;
	struct Node* next;
}Node;

typedef struct Asset1{
	char type[20];
	int ivals[1];
	double svals[1];
}Asset1;

typedef struct Asset2{
	char type[20];
	double svals[2];
	int ivals[2];
} Asset2;

typedef struct Asset3{
	char type[20];
	char string1[50];
	char string2[50];
} Asset3;

typedef struct Asset4{
	char type[20];
	double value1;
	float value2;
	double value3;
} Asset4;

double randDouble();
void insert(struct Node **head);
void printLinkedList(struct Node *head);
void fillLinkedList(struct Node **head);
void serializeLinkedList(struct Node *head);
void insertAtEnd(struct Node **head, void *asset);
void deserializeLinkedList(struct Node **head);

int main(int argc, char *argv[]) {

	struct Node *head = NULL;
	fillLinkedList(&head);
	printf("Generated Linked List:\n");
	printLinkedList(head);
	printf("\n");
	serializeLinkedList(head);
	printf("Serialized Linked List and saved to 'file.bin'.");
	printf("\n\n");
	
	struct Node *newHead = NULL;
	deserializeLinkedList(&newHead);
	printLinkedList(newHead);
	
	return 0;
}

double randDouble()//Random double number generator.
{
    double range = 10; 
    double div = RAND_MAX / 10;
    return 0 + (rand() / div);
}

void insert(struct Node **head)//selection random asset and filling random number in assets
{	
	Node *np;
	np = (Node*) malloc(sizeof(Node));
	np->next = *head;
	
	int i;
	int asset_type = rand()%4;
	if(asset_type == 0){
		Asset1 *data;
		data = (Asset1*) malloc(sizeof(Asset1));
		strcpy(data->type,"Asset1");
		data->ivals[0] = rand();
		data->svals[0] = randDouble();
		np->data = data;
	}
	else if(asset_type == 1){
		Asset2 *data;
		data = (Asset2*) malloc(sizeof(Asset2));
		strcpy(data->type,"Asset2");
		data->svals[0] = randDouble();
		data->svals[1] = randDouble();
		data->ivals[0] = rand();
		data->ivals[1] = rand();
		np->data = data;
	}
	else if(asset_type == 2){
		Asset3 *data;
		data = (Asset3*) malloc(sizeof(Asset3));
		strcpy(data->type,"Asset3");
		for(i = 0; i < 10; i++){
    		data->string1[i] = '65' + rand()%58;
    	}
    	data->string1[i] = '\0';
    	for(i = 0; i < 10; i++){
    		data->string2[i] = '65' + rand()%58;
    	}
    	data->string2[i] = '\0';
    	np->data = data;
	}
	else{
		Asset4 *data;
		data = (Asset4*) malloc(sizeof(Asset4));
		strcpy(data->type,"Asset4");
    	data->value1 = randDouble();
    	data->value2 = randDouble();
    	data->value3 = randDouble();
    	np->data = data;
	}
	*head = np;
}

void printLinkedList(struct Node *head)//printing function.
{
	Node *current = head;
	while(current != NULL)
	{
		void *asset = current->data;
		if (strcmp(((Asset1*)asset)->type, "Asset1") == 0) {
            printf("Asset Type: %s ",((Asset1*)asset)->type);
            printf("Ivals: %d ",((Asset1*)asset)->ivals[0]);
            printf("Svals: %lf\n",((Asset1*)asset)->svals[0]);
        } else if (strcmp(((Asset2*)asset)->type, "Asset2") == 0) {
            printf("Asset Type: %s ",((Asset2*)asset)->type);
            printf("Svals: %lf %lf ",((Asset2*)asset)->svals[0],((Asset2*)asset)->svals[1]);
            printf("Ivals: %d %d\n",((Asset2*)asset)->ivals[0],((Asset2*)asset)->ivals[1]);
        } else if (strcmp(((Asset3*)asset)->type, "Asset3") == 0) {
            printf("Asset Type: %s ",((Asset3*)asset)->type);
            printf("String1: %s ",((Asset3*)asset)->string1);
            printf("String2: %s\n",((Asset3*)asset)->string2);
        } else if (strcmp(((Asset4*)asset)->type, "Asset4") == 0) {
            printf("Asset Type: %s ",((Asset4*)asset)->type);
            printf("Value1: %lf ",((Asset4*)asset)->value1);
            printf("Value2: %lf ",((Asset4*)asset)->value2);
            printf("Value3: %lf\n",((Asset4*)asset)->value3);
        }
		current = current->next;
	}
}

void fillLinkedList(struct Node **head)
{
	srand(time(0));
	int num_of_asset = rand()%11;//to determine how many asset are there.
	num_of_asset += 10;
	
	int i;
	for(i=0;i<num_of_asset;i++){
		insert(head);
	}
}

void serializeLinkedList(struct Node *head)//writing inside of assets to binary file.
{
	FILE *fptr = fopen("file.bin","wb");
	
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Node* current = head;
	while (current != NULL) {
        void* asset = current->data;
        if (strcmp(((Asset1*)asset)->type, "Asset1") == 0) {
            fwrite(&((Asset1*)asset)->type,sizeof(((Asset1*)asset)->type),1,fptr);
			fwrite(&((Asset1*)asset)->ivals[0],sizeof(((Asset1*)asset)->ivals[0]),1,fptr);
			fwrite(&((Asset1*)asset)->svals[0],sizeof(((Asset1*)asset)->svals[0]),1,fptr);
        } else if (strcmp(((Asset2*)asset)->type, "Asset2") == 0) {
            fwrite(&((Asset2*)asset)->type,sizeof(((Asset2*)asset)->type),1,fptr);
			fwrite(&((Asset2*)asset)->svals[0],sizeof(((Asset2*)asset)->svals[0]),1,fptr);
			fwrite(&((Asset2*)asset)->svals[1],sizeof(((Asset2*)asset)->svals[1]),1,fptr);
			fwrite(&((Asset2*)asset)->ivals[0],sizeof(((Asset2*)asset)->ivals[0]),1,fptr);
			fwrite(&((Asset2*)asset)->ivals[1],sizeof(((Asset2*)asset)->ivals[1]),1,fptr);
        } else if (strcmp(((Asset3*)asset)->type, "Asset3") == 0) {
            fwrite(&((Asset3*)asset)->type,sizeof(((Asset3*)asset)->type),1,fptr);
            fwrite(&((Asset3*)asset)->string1,sizeof(((Asset3*)asset)->string1),1,fptr);
            fwrite(&((Asset3*)asset)->string2,sizeof(((Asset3*)asset)->string2),1,fptr);
        } else if (strcmp(((Asset4*)asset)->type, "Asset4") == 0) {
            fwrite(&((Asset4*)asset)->type,sizeof(((Asset4*)asset)->type),1,fptr);
            fwrite(&((Asset4*)asset)->value1,sizeof(((Asset4*)asset)->value1),1,fptr);
            fwrite(&((Asset4*)asset)->value2,sizeof(((Asset4*)asset)->value2),1,fptr);
            fwrite(&((Asset4*)asset)->value3,sizeof(((Asset4*)asset)->value3),1,fptr);
        }
        current = current->next;
    }
	
	fclose(fptr);
}

void insertAtEnd(struct Node **head, void *asset)//inserting end of the linked list.
{
	Node *np = malloc(sizeof(Node));
	np->data = asset;
	np->next = NULL;
	
	Node *cp;
	if(*head == NULL)
	{	
		*head = np;
	}
	else
	{
		cp = *head;
		while(cp->next != NULL)
		{
			cp = cp->next;
		}	
		cp->next = np;
	}
}

void deserializeLinkedList(struct Node **head)//reading from binary file and filling linked list.
{
	FILE *fptr = fopen("file.bin","rb");
	
	if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
   	char cType[20];
    while (fread(cType, sizeof(char), 20, fptr) != 0) 
	{
		Node *np = malloc(sizeof(Node));
		if(strcmp(cType, "Asset1") == 0){
			Asset1 *asset;
			asset = (Asset1*) malloc(sizeof(Asset1));
			strcpy(asset->type,cType);
			fread(&((Asset1*)asset)->ivals[0],sizeof(((Asset1*)asset)->ivals[0]),1,fptr);
			fread(&((Asset1*)asset)->svals[0],sizeof(((Asset1*)asset)->svals[0]),1,fptr);
			
			insertAtEnd(head,asset);
		}
		else if(strcmp(cType, "Asset2") == 0){
			Asset2 *asset;
			asset = (Asset2*) malloc(sizeof(Asset2));
			strcpy(asset->type,cType);
			fread(&((Asset2*)asset)->svals[0],sizeof(((Asset2*)asset)->svals[0]),1,fptr);
			fread(&((Asset2*)asset)->svals[1],sizeof(((Asset2*)asset)->svals[1]),1,fptr);
			fread(&((Asset2*)asset)->ivals[0],sizeof(((Asset2*)asset)->ivals[0]),1,fptr);
			fread(&((Asset2*)asset)->ivals[1],sizeof(((Asset2*)asset)->ivals[1]),1,fptr);
			
			insertAtEnd(head,asset);
		}
		else if(strcmp(cType, "Asset3") == 0){
			Asset3 *asset;
			asset = (Asset3*) malloc(sizeof(Asset3));
			strcpy(asset->type,cType);
            fread(&((Asset3*)asset)->string1,sizeof(((Asset3*)asset)->string1),1,fptr);
            fread(&((Asset3*)asset)->string2,sizeof(((Asset3*)asset)->string2),1,fptr);
           	
			insertAtEnd(head,asset);
		}
		else if(strcmp(cType, "Asset4") == 0){
			Asset4 *asset;
			asset = (Asset4*) malloc(sizeof(Asset4));
			strcpy(asset->type,cType);
            fread(&((Asset4*)asset)->value1,sizeof(((Asset4*)asset)->value1),1,fptr);
            fread(&((Asset4*)asset)->value2,sizeof(((Asset4*)asset)->value2),1,fptr);
            fread(&((Asset4*)asset)->value3,sizeof(((Asset4*)asset)->value3),1,fptr);
			
			insertAtEnd(head,asset);
		}
    }

    fclose(fptr);
}
