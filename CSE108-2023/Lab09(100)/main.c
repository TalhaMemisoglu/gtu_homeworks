#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char name[20];
	char color[20];
	float price;
	int serial_number;
	int quantity;
}furniture_db;

void see_all_furniture(void);
void add_new_furniture(void);
void remove_furniture(void);
void search_furniture(void);
void name_search(furniture_db furniture[],char *user_name);
void color_search(furniture_db furniture[],char *user_color);

int main(int argc, char *argv[]) {
	
	int choice=0;
	while(choice!=5)
	{
		printf("Welcome to database of furniture shop\n");
		printf("1-See all furnitures\n");
		printf("2-Add a new furniture\n");
		printf("3-Remove furniture\n");
		printf("4-Search furniture\n");
		printf("5-Exit\n");
		printf(">");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				see_all_furniture();
				break;
			case 2:
				add_new_furniture();
				break;
			case 3:
				remove_furniture();
				break;
			case 4:
				search_furniture();
				break;
			case 5:
				printf("Program Terminated..\n");
			default:
				printf("Invalid Typing\n");
		}
	}
	return 0;
}

void see_all_furniture(void)
{
	FILE *fptr;
	fptr=fopen("furniture_database.txt","r");
	char feature[20];
	furniture_db furniture;
	while(fscanf(fptr,"%s %s",feature,furniture.name) != EOF)
	{
		printf("%s %s\n",feature,furniture.name);
		fscanf(fptr,"%s %s",feature,furniture.color);
		printf("%s %s\n",feature,furniture.color);
		
		fscanf(fptr,"%s %f",feature,&furniture.price);
		printf("%s %.2f\n",feature,furniture.price);
		
		fscanf(fptr,"%s %d",feature,&furniture.serial_number);
		printf("%s %d\n",feature,furniture.serial_number);
		
		fscanf(fptr,"%s %d",feature,&furniture.quantity);
		printf("%s %d\n\n",feature,furniture.quantity);
	}
	
	fclose(fptr);
}

void add_new_furniture(void)
{
	FILE *fptr;
	fptr=fopen("furniture_database.txt","a");
	
	furniture_db furniture;
	printf("Please enter the properties of new furniture(Name, color, price, serial number, quantity):");
	scanf("%s %s %f %d %d",furniture.name,furniture.color,&furniture.price,&furniture.serial_number,&furniture.quantity);
	
	printf("Following furniture is added to database:\n");
	fprintf(fptr,"\nName: %s\n",furniture.name);
	printf("\nName: %s\n",furniture.name);
	fprintf(fptr,"Color: %s\n",furniture.color);
	printf("Color: %s\n",furniture.color);
	fprintf(fptr,"Price: %f\n",furniture.price);
	printf("Price: %f\n",furniture.price);
	fprintf(fptr,"Serial_Number: %d\n",furniture.serial_number);
	printf("Serial_Number: %d\n",furniture.serial_number);
	fprintf(fptr,"Quantity: %d\n",furniture.quantity);
	printf("Quantity: %d\n\n",furniture.quantity);
	
	fclose(fptr);
}

void file_copy(char *file_name1, char *file_name2)
{
	FILE *fptr;
	fptr=fopen(file_name1,"w");
	FILE *fptr2;
	fptr2=fopen(file_name2,"r");
	
	char feature[20];
	furniture_db furniture;
	
	while(fscanf(fptr2,"%s %s",feature,furniture.name) != EOF)
	{
		fprintf(fptr,"%s %s\n",feature,furniture.name);
		fscanf(fptr2,"%s %s",feature,furniture.color);
		fprintf(fptr,"%s %s\n",feature,furniture.color);
		
		fscanf(fptr2,"%s %f",feature,&furniture.price);
		fprintf(fptr,"%s %.2f\n",feature,furniture.price);
		
		fscanf(fptr2,"%s %d",feature,&furniture.serial_number);
		fprintf(fptr,"%s %d\n",feature,furniture.serial_number);
		
		fscanf(fptr2,"%s %d",feature,&furniture.quantity);
		fprintf(fptr,"%s %d\n\n",feature,furniture.quantity);
	}
	
	fclose(fptr);
	fclose(fptr2);
}

void remove_furniture(void)
{
	FILE *fptr;
	fptr=fopen("furniture_database.txt","r");
	FILE *fptr2;
	fptr2=fopen("temp.txt","w");
	
	char feature[20];
	furniture_db furniture;
	
	int removed,i=0;
	printf("Enter the index of product you want to remove:");
	scanf("%d",&removed);
	while(fscanf(fptr,"%s %s",feature,furniture.name) != EOF)
	{
		if(removed==i) printf("\nFollowing furniture is removed from the database:\n");
		if(removed!=i) fprintf(fptr2,"%s %s\n",feature,furniture.name);
		if(removed==i) printf("%s %s\n",feature,furniture.name);
		
		fscanf(fptr,"%s %s",feature,furniture.color);
		if(removed!=i) fprintf(fptr2,"%s %s\n",feature,furniture.color);
		if(removed==i) printf("%s %s\n",feature,furniture.color);
		
		fscanf(fptr,"%s %f",feature,&furniture.price);
		if(removed!=i) fprintf(fptr2,"%s %.2f\n",feature,furniture.price);
		if(removed==i) printf("%s %.2f\n",feature,furniture.price);
		
		fscanf(fptr,"%s %d",feature,&furniture.serial_number);
		if(removed!=i) fprintf(fptr2,"%s %d\n",feature,furniture.serial_number);
		if(removed==i) printf("%s %d\n",feature,furniture.serial_number);
		
		fscanf(fptr,"%s %d",feature,&furniture.quantity);
		if(removed!=i) fprintf(fptr2,"%s %d\n\n",feature,furniture.quantity);
		if(removed==i) printf("%s %d\n\n",feature,furniture.quantity);
		i++;
	}
	
	fclose(fptr);
	fclose(fptr2);
	
	file_copy("furniture_database.txt","temp.txt");
}

void search_furniture(void)
{
	FILE *fptr;
	fptr = fopen("furniture_database.txt","r");
	
	char feature[20];
	furniture_db furniture[20];
	int i=0;
	while(fscanf(fptr,"%s %s",feature,furniture[i].name) != EOF)
	{
		fscanf(fptr,"%s %s",feature,furniture[i].color);
		fscanf(fptr,"%s %f",feature,&furniture[i].price);
		fscanf(fptr,"%s %d",feature,&furniture[i].serial_number);
		fscanf(fptr,"%s %d",feature,&furniture[i].quantity);
		i++;
	}
	
	int property;
	char name[20],color[20];
	printf("Select a property to make search:\n\n");
	printf("1-Name\n");
	printf("2-Color\n");
	scanf("%d",&property);
	switch(property)
	{
		case 1:
			printf("Enter the name of product:");
			scanf("%s",name);
			name_search(furniture,name);
			break;
		case 2:
			printf("Enter the color of product:");
			scanf("%s",color);
			color_search(furniture,color);
			break;
		default:
			printf("Invalid Typing!");		
	}
	
	fclose(fptr);
}

void name_search(furniture_db furniture[],char *user_name)
{
	if(strcmp(furniture[0].name,user_name)==0)
	{
		printf("Name: %s\n",furniture[0].name);
		printf("Color: %s\n",furniture[0].color);
		printf("Price: %.2f\n",furniture[0].price);
		printf("Serial_Number: %d\n",furniture[0].serial_number);
		printf("Quantity: %d\n",furniture[0].quantity);	
	}
	else
	{
		name_search(&furniture[1],user_name);
	}	
}

void color_search(furniture_db furniture[],char *user_color)
{
	if(strcmp(furniture[0].color,user_color)==0)
	{
		printf("Name: %s\n",furniture[0].name);
		printf("Color: %s\n",furniture[0].color);
		printf("Price: %.2f\n",furniture[0].price);
		printf("Serial_Number: %d\n",furniture[0].serial_number);
		printf("Quantity: %d\n",furniture[0].quantity);	
	}
	else
	{
		color_search(&furniture[1],user_color);
	}	
}
