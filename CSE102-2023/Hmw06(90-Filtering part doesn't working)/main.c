#include <stdio.h>
#include <stdlib.h>
#define FEAT_NUM 8
#define FEAT_NAME_LENGTH 20
#define COLUMNS_NUM 60
#define INPUT_OF_FEAT_LENGTH 20
#define NUMBER_OF_FILTER 4
#define PRODUCTS_NUM 50

void file_operations(void);
void adding_new_product(void);
void adding_new_feature(void);
void file_copy(char file1[15], char file2[15]);
void deleting_product(void);
void deleting_product(void);
void updating_product(void);
void adding_new_stock_entry(void);
void deleting_stock_entry(void);
void updating_stock_entry(void);
void search_operations(void);
void listing_all_product(void);
void filtering_products(void);
void brand_stock_control(void);
void checking_stock_status_operations(void);
void option1(void);
void option2(void);
void option3(void);
void stock_control(void);

int main() 
{
	
	int menu_selection;
	while(menu_selection != 5)
	{
		printf("Welcome operator, please select an option to continue:\n");
		printf("1- File Operations\n");
		printf("2- Query products\n");
		printf("3- Check stocks status\n");
		printf("4- Stock control by brand\n");
		printf("5- Export report\n");
		printf("Enter your selection:");
		scanf("%d",&menu_selection);
		switch(menu_selection)
		{
			case 1:
				file_operations();
				break;
			case 2:
				search_operations();
				break;
			case 3:
				checking_stock_status_operations();
				break;
			case 4:
				brand_stock_control();
				break;
			case 5:
				stock_control();
				break;
			default:
				printf("Invalid menu selection!");
		}
	}
		
	return 0;
}

void file_operations(void)
{
	int menu_selection;
	while(menu_selection!=8)
	{
		printf("\nWelcome to file operations menu, please select an option to continue:\n");
		printf("1- Add a new product\n");
		printf("2- Delete a product\n");
		printf("3- Update a product\n");
		printf("4- Add feature to products\n");
		printf("5- Add a new stock entry\n");
		printf("6- Delete a stock entry\n");
		printf("7- Update a stock entry\n");
		printf("8- Back to main menu\n");
		printf("Enter your selection:");
		scanf("%d",&menu_selection);
		switch(menu_selection)
		{
			case 1:
				adding_new_product();
				break;
			case 2:
				deleting_product();
				break;
			case 3:
				updating_product();
				break;
			case 4:
				adding_new_feature();
				break;
			case 5:
				adding_new_stock_entry();
				break;
			case 6:
				deleting_stock_entry();
				break;
			case 7:
				updating_stock_entry();
				break;
			case 8:
				printf("\n");
				break;
			default:
				printf("Invalid menu selection!");
		}
	}
}

void search_operations(void)
{
	int menu_selection;
	while(menu_selection != 3)
	{
		printf("\nWelcome to file operations menu, please select an option to continue:\n");
		printf("1-List all products\n");
		printf("2-Filter products\n");
		printf("3-Back to main menu\n");
		printf("Enter your selection:");
		scanf("%d",&menu_selection);
		switch(menu_selection)
		{
			case 1:
				listing_all_product();
				break;
			case 2:
				//filtering_products();
				break;
			case 3:
				printf("\n");
				break;
			default:
				printf("Invalid menu selection!");
		}
	}
}

void checking_stock_status_operations(void)
{
	int menu_selection;
	while(menu_selection != 3)
	{
		printf("\nWelcome to file operations menu, please select an option to continue:\n");
		printf("1-Option 1\n");
		printf("2-Option 2\n");
		printf("3-Option 3\n");
		printf("4-Back to main menu\n");
		printf("Enter your selection:");
		scanf("%d",&menu_selection);
		switch(menu_selection)
		{
			case 1:
				option1();
				break;
			case 2:
				option2();
				break;
			case 3:
				option3();
				break;
			case 4:
				printf("\n");
				break;
			default:
				printf("Invalid menu selection!");
		}
	}
}

void file_copy(char file1[15], char file2[15])//to copying operations
{
	FILE *fptr1;
	fptr1 = fopen(file1,"w");
	FILE *fptr2;
	fptr2 = fopen(file2,"r");
	
	char string[COLUMNS_NUM];
	while(fscanf(fptr2,"%s",string) != EOF)
	{
		fprintf(fptr1,"%s\n",string);
	}
	
	fclose(fptr1);
	fclose(fptr2);
}

void adding_new_product(void)
{
	FILE *fptr;
	fptr = fopen("products.txt","r+");
	char features[FEAT_NUM][FEAT_NAME_LENGTH];
	
	int c;
	int n=0,j=0;
	while((c=fgetc(fptr)) != '\n')//to determine which features are there
	{
		if(c == ',')
		{
			features[n][j] = '\0';
			n++;
			j=0;
		}
		else 
		{
			features[n][j] = c;
			j++;	
		}
	}
	features[n][j] = '\0';
	
	int control;
	int num;
	char string[30];
	do//to calculate how much pID in the file.
	{
		control = fscanf(fptr,"%d%s",&num,string);
	}
	while(control != EOF);
	fclose(fptr);
	char type;
	char name[15];
	char brand[15];
	float price;
	char input_features[INPUT_OF_FEAT_LENGTH];
	
	fptr = fopen("products.txt","a");
	char status;
	int i;
	while(status != 'n' && status != 'N')
	{
		num++;
		printf("Type: ");
		scanf(" %c",&type);
		printf("Name: ");
		scanf("%s",name);
		printf("Brand: ");
		scanf("%s",brand);
		printf("Price: ");
		scanf("%f",&price);
		fprintf(fptr,"%d,%c,%s,%s,%.1f\n",num,type,name,brand,price);
		
		for(i=5;i<=n;i++)//to adding user-defined features.
		{
			printf("%s:",features[i]);
			scanf("%s",input_features);
			fprintf(fptr,",%s",input_features);
		}
		printf("Continue to adding operation?(y/n): ");
		scanf(" %c",&status);
	}
	
	fclose(fptr);
}

void deleting_product(void)
{
	//transfer the new state to temporary and transfer it back to the products from there in this function.
	FILE *fptr1;
	fptr1 = fopen("products.txt","r");
	
	FILE *fptr2;
	fptr2 = fopen("temporary.txt","w");
	
	int user_num,num;
	printf("Enter the pID number to delete: ");
	scanf("%d",&user_num);
	
	char string[COLUMNS_NUM];
	fscanf(fptr1,"%s",string);
	fprintf(fptr2,"%s\n",string);
	while(fscanf(fptr1,"%d%s",&num,string) != EOF)
	{
		if(num == user_num)
		{
			while(fscanf(fptr1,"%d%s",&num,string) != EOF)
			{
				fprintf(fptr2,"%d%s\n",num-1,string);
			}
			break;
		}
		fprintf(fptr2,"%d%s\n",num,string);
	}
	
	fclose(fptr1);
	fclose(fptr2);
	
	file_copy("products.txt","temporary.txt");
}

void updating_product(void)
{
	//transfer the new state to temporary and transfer it back to the products from there in this function.
	FILE *fptr1;
	fptr1 = fopen("products.txt","r");
	
	FILE *fptr2;
	fptr2 = fopen("temporary.txt","w");
	
	char features[FEAT_NUM][FEAT_NAME_LENGTH];
	int c;
	int n=0,j=0;
	while((c=fgetc(fptr1)) != '\n')
	{
		if(c == ',')
		{
			features[n][j] = '\0';
			n++;
			j=0;
		}
		else 
		{
			features[n][j] = c;
			j++;	
		}
		fprintf(fptr2,"%c",c);
	}
	features[n][j] = '\0';
	fprintf(fptr2,"\n");
	
	int pID;
	printf("Enter pID number: ");
	scanf("%d",&pID);
	
	int feat_num,i;
	for(i=1;i<=n;i++)
	{
		printf("%d.%s\n",i,features[i]);
	}
	printf("Please choose that you want to update: ");
	scanf("%d",&feat_num);
	
	char new_name[FEAT_NAME_LENGTH];
	printf("Enter new input of %s: ",features[feat_num]);
	scanf("%s",new_name);
	
	int num;
	int cntr=0;
	char string[COLUMNS_NUM];
	while(fscanf(fptr1,"%d%s",&num,string) != EOF)
	{
		if(num==pID-1)
		{
			c = fgetc(fptr1);
			while((c = fgetc(fptr1)) != '\n')
			{
				if(c == ',')
				{
					cntr++;
					if(cntr==feat_num)
					{
						fprintf(fptr2,",%s",new_name);
						do
						{
							c=fgetc(fptr1);
						}
						while(c!= ',' && c!='\n');
					}
				}
				fprintf(fptr2,"%c",c);
			}
			fprintf(fptr2,"\n",c);
		}
		else
		{
			fprintf(fptr2,"%d%s\n",num,string);
		}
	}
	
	fclose(fptr1);
	fclose(fptr2);
	
	file_copy("products.txt","temporary.txt");
}

void adding_new_feature(void)//to adding new feature in the end.
{
	char string[COLUMNS_NUM],new_feature[FEAT_NAME_LENGTH];
	printf("Enter the name of the new feature:");
	scanf("%s",new_feature);
	
	FILE *fptr1;
	fptr1 = fopen("products.txt","r");
	
	FILE *fptr2;
	fptr2 = fopen("temporary.txt","w");
	
	fscanf(fptr1,"%s",string);
	fprintf(fptr2,"%s,%s\n",string,new_feature);
	
	while(fscanf(fptr1,"%s",string) != EOF)
	{
		fprintf(fptr2,"%s,NULL\n",string);
	}
	
	fclose(fptr1);
	fclose(fptr2);
	
	file_copy("products.txt","temporary.txt");
}

void adding_new_stock_entry(void)
{
	//basically it is the same as adding new product.
	FILE *fptr;
	fptr = fopen("stocks.txt","r+");
	
	int control;
	int num;
	char string[30];
	fscanf(fptr,"%s",string);
	do
	{
		control = fscanf(fptr,"%d%s",&num,string);
	}
	while(control != EOF);
	fclose(fptr);
	
	fptr = fopen("stocks.txt","a");
	int pID;
	char branch[FEAT_NAME_LENGTH];
	int current_stock;
	char status;
	while(status != 'n' && status != 'N')
	{
		num++;
		printf("pID: ");
		scanf("%d",&pID);
		printf("Branch: ");
		scanf("%s",branch);
		printf("Current Stock: ");
		scanf("%d",&current_stock);
		fprintf(fptr,"%d,%d,%s,%d\n",num,pID,branch,current_stock);
		printf("Continue to adding operation?(y/n): ");
		scanf(" %c",&status);
	}
	fclose(fptr);	
}

void deleting_stock_entry(void)
{
	//basically it is the same as deleting product.
	
	FILE *fptr1;
	fptr1 = fopen("stocks.txt","r");
	
	FILE *fptr2;
	fptr2 = fopen("temporary.txt","w");
	
	int user_num,num;
	printf("Enter the sID number to delete: ");
	scanf("%d",&user_num);
	
	char string[COLUMNS_NUM];
	fscanf(fptr1,"%s",string);
	fprintf(fptr2,"%s\n",string);
	while(fscanf(fptr1,"%d%s",&num,string) != EOF)
	{
		if(num == user_num)
		{
			while(fscanf(fptr1,"%d%s",&num,string) != EOF)
			{
				fprintf(fptr2,"%d%s\n",num-1,string);
			}
			break;
		}
		fprintf(fptr2,"%d%s\n",num,string);
	}
	
	fclose(fptr1);
	fclose(fptr2);
	
	file_copy("stocks.txt","temporary.txt");
}

void updating_stock_entry(void)
{
	//basically it is the same as updating product.
	
	FILE *fptr1;
	fptr1 = fopen("stocks.txt","r");
	
	FILE *fptr2;
	fptr2 = fopen("temporary.txt","w");
	
	int sID;
	printf("Enter sID number: ");
	scanf("%d",&sID);
	
	char feat[3][15]={"pID","branch","current_stock"};
	int feat_num;
	printf("1.pID\n");
	printf("2.Branch\n");
	printf("3.Current Stock\n");
	printf("Please choose that you want to update: ");
	scanf("%d",&feat_num);
	
	char new_name[FEAT_NAME_LENGTH];
	printf("Enter new input of %s: ",feat[feat_num-1]);
	scanf("%s",new_name);
	
	int num;
	int c,cntr=0;
	char string[COLUMNS_NUM];
	fscanf(fptr1,"%s",string);
	fprintf(fptr2,"%s",string);
	while(fscanf(fptr1,"%d%s",&num,string) != EOF)
	{
		if(num==sID-1)
		{
			c = fgetc(fptr1);
			while((c = fgetc(fptr1)) != '\n')
			{
				if(c == ',')
				{
					cntr++;
					if(cntr==feat_num)
					{
						fprintf(fptr2,",%s",new_name);
						do
						{
							c=fgetc(fptr1);
						}
						while(c!= ',' && c!='\n');
					}
				}
				fprintf(fptr2,"%c",c);
			}
			fprintf(fptr2,"\n",c);
		}
		else
		{
			fprintf(fptr2,"%d%s\n",num,string);
		}
	}
	
	fclose(fptr1);
	fclose(fptr2);
	
	file_copy("stocks.txt","temporary.txt");
}

/*******************************************************************************************************/

void listing_all_product(void)
{
	//It listing all products.
	FILE *fptr;
	fptr = fopen("products.txt","r");
	
	char string[COLUMNS_NUM];
	while (fscanf(fptr,"%s",string) != EOF )
	{
		printf("%s\n",string);	
	}
	
	fclose(fptr);
}

void filtering_products(void)//it doesn't work.
{
	/*FILE *fptr;
	fptr = fopen("temporary.txt","w+");
	
	char string[COLUMNS_NUM];
	printf("Enter your filtering that you want to:");
	scanf("%s",string);
	fprintf(fptr,"%s",string);
	fclose(fptr);
	
	fptr = fopen("temporary.txt","r");
	char feat[FEAT_NUM][FEAT_NAME_LENGTH];
	char feat_name[FEAT_NUM][FEAT_NAME_LENGTH];
	int c,n=0,j=0;
	while((c=fgetc(fptr)) != EOF)
	{
		if(c==',')
		{
			feat[n][j]='\0';
			j=0;
			c=fgetc(fptr);
			while(1)
			{
				if(c == ',' || c == EOF)
				{
					feat_name[n][j]='\0';
					break;
				}
				else
				{
					feat_name[n][j]=c;
					j++;
					c=fgetc(fptr);
				}
			}
			n++;
			j=0;
		}
		else
		{
			feat[n][j]=c;
			j++;
		}
	}
	feat[n][j]='\0';
	fclose(fptr); 
	
	fptr = fopen("products.txt","r");
	char features_in_file[FEAT_NUM][FEAT_NAME_LENGTH];
	
	int k=0;
	c=0,j=0;
	while((c=fgetc(fptr)) != '\n')
	{
		if(c == ',')
		{
			features_in_file[k][j] = '\0';
			k++;
			j=0;
		}
		else 
		{
			features_in_file[k][j] = c;
			j++;	
		}
	}
	features_in_file[k][j] = '\0';
	
	int i,z=0,y=0,order[NUMBER_OF_FILTER][;
	for(i=0;i<NUMBER_OF_FILTER;i++){
		order[i]=0;
	}
	for(i=0;i<=n;i++)
	{
		for(j=0;j<=k;j++)
		{
			if(strcmp(feat[i],features_in_file[j])==0)
			{
				order[z][=j;
				order
				z++;
			}
		}
	}
	order[z]=-1;
	
	int price_exist=0,min_value,max_value;
	for(i=0;order[i]!=(-1);i++)
	{
		if(order[i]==4)
		{
			printf("Min Value: ");
			scanf("%f",&min_value);
			printf("Max Value: ");
			scanf("%f",&max_value);
			price_exist=1;				
			break;
		}
	}

	int pID,price;
	char type,brand[20],name[20],feature[k-4][FEAT_NAME_LENGTH];
	int i,filter;
	char string[50];
	fscanf(fptr,"%s",string);
	while(fscanf(fptr,"%d,%c,",&pID,&type) != EOF)
	{
		i=0;
		while((c=fgetc(fptr)) != ',')
		{
			name[i] = c;
			i++;
		}
		name[i] = '\0';
		i=0;
		while((c=fgetc(fptr)) != ',')
		{
			brand[i] = c;
			i++;
		}
		brand[i] = '\0';
		fscanf(fptr,"%f",&price);
		
		for(i=5;i<=k;i++)
		{
			c=fgetc(fptr);
			for(j=0;c != ',';j++)
			{
				feature[i-5][j] = c;
				c=fgetc(fptr);
			}
			feature[i-5][j] = '\0';
		}
	
		for(i=0;i<=k;i++)
		{
			for(j=0;j<=n;j++)
			{
				if()
			}
		}
		if(features_in_file[i])
		i=0;
		while(branch[i] != '\0' && string[i] != '\0')
		{
			if(branch[i] != string[i]) filter=0;
			i++;
		}
		if(filter==1 && stock==0)
		{
			printf("%d,%d,%s,%d\n",sID,pID,branch,stock);
		}
	}
	
	printf("%d",filtered_products[0]);
	
	fclose(fptr);*/
}

/*******************************************************************************************************/

void option1(void)
{
	//to scan for branch and pID
	int sID,pID,pID_user;
	char branch[20];
	printf("pID: ");
	scanf("%d",&pID_user);
	printf("Branch: ");
	scanf("%s",branch);
	
	//to filtering by branch that given and pID that given.
	FILE *fptr = fopen("stocks.txt","r");
	char string[50];
	int stock,i,c,filter;
	fscanf(fptr,"%s",string);
	while(fscanf(fptr,"%d,%d,",&sID,&pID) != EOF)
	{
		i=0;
		while((c=fgetc(fptr)) != ',')
		{
			string[i] = c;
			i++;
		}
		string[i] = '\0';
		fscanf(fptr,"%d",&stock);
		filter=1;
		i=0;
		while(branch[i] != '\0' && string[i] != '\0')
		{
			if(branch[i] != string[i]) filter=0;
			i++;
		}
		if(pID_user == pID && filter==1)
		{
			printf("Current Stock:%d\n",stock);
		}
	}
	fclose(fptr);
}

void option2(void)
{
	//to scan for branch
	int sID,pID;
	char branch[20];
	printf("Branch: ");
	scanf("%s",branch);
	
	//to filtering by branch that given.
	FILE *fptr = fopen("stocks.txt","r");
	char string[50];
	int stock,i,c,filter;
	fscanf(fptr,"%s",string);
	while(fscanf(fptr,"%d,%d,",&sID,&pID) != EOF)
	{
		i=0;
		while((c=fgetc(fptr)) != ',')
		{
			string[i] = c;
			i++;
		}
		branch[i] = '\0';
		fscanf(fptr,"%d",&stock);
		filter=1;
		i=0;
		while(branch[i] != '\0' && string[i] != '\0')
		{
			if(branch[i] != string[i]) filter=0;
			i++;
		}
		if(filter==1)
		{	
			printf("%d,%d,%s,%d\n",sID,pID,branch,stock);
		}
	}
	fclose(fptr);
}

void option3(void)
{
	//to scan for branch
	int sID,pID;
	char branch[20];
	printf("Branch: ");
	scanf("%s",branch);
	
	//to filtering products which is out of stock and print sID,pID,branch,stock.
	FILE *fptr = fopen("stocks.txt","r");
	char string[50];
	int stock,i,c,filter;
	fscanf(fptr,"%s",string);
	while(fscanf(fptr,"%d,%d,",&sID,&pID) != EOF)
	{
		i=0;
		while((c=fgetc(fptr)) != ',')
		{
			string[i] = c;
			i++;
		}
		branch[i] = '\0';
		fscanf(fptr,"%d",&stock);
		filter=1;
		i=0;
		while(branch[i] != '\0' && string[i] != '\0')
		{
			if(branch[i] != string[i]) filter=0;
			i++;
		}
		if(filter==1 && stock==0)
		{
			printf("%d,%d,%s,%d\n",sID,pID,branch,stock);
		}
	}
	fclose(fptr);
}

/*******************************************************************************************************/

void brand_stock_control(void)
{
	//to list all products of a given brand with its pID, its price, and its current stock 
	FILE *fptr1 = fopen("products.txt","r");
	
	//to scan for brand.
	char brand[20];
	printf("Enter the Brand: ");
	scanf("%s",brand);

	int filter=1;
	int i=0;
	char c;
	int pID,cntr;
	char string[30],cr_brand[20];
	float arr[PRODUCTS_NUM][1];
	for(i=0;i<PRODUCTS_NUM;i++)
	{
		arr[i][0]=-1;
	}
	fscanf(fptr1,"%s",string);
	while(fscanf(fptr1,"%d",&pID) != EOF)
	{
		cntr=0;
		filter=1;
		while((c=fgetc(fptr1))!='\n' && c != EOF)
		{
			if(c==',')
			{
				cntr++;
				if(cntr==3)
				{
					i=0;
					while((c=fgetc(fptr1))!=',')
					{
						cr_brand[i] = c;
						i++;
					}
					cr_brand[i]='\0';
					i=0;
					while(brand[i]!='\0' && cr_brand[i]!='\0')
					{
						if(brand[i]!=cr_brand[i])
						{
							filter=0;
						}
						i++;
					}
					if(filter==1)
					{
						fscanf(fptr1,"%f",&arr[pID][0]);
					}
				}
			}
		}
	}
	FILE *fptr2;
	
	int sID,current_stock;
	for(i=0;i<PRODUCTS_NUM;i++)
	{
		if(arr[i][0] != (-1))
		{
			fptr2 = fopen("stocks.txt","r");
			fscanf(fptr2,"%s",string);
			while(fscanf(fptr2,"%d,%d,",&sID,&pID) != EOF)
			{
				do
				{
					c=fgetc(fptr2);
				}
				while(c != ',');
				fscanf(fptr2,"%d",&current_stock);
				if(pID==i)
				{
					printf("pID:%d, price:%.1f, current Stock:%d\n",i,arr[i][0],current_stock);
				}
			}			
			fclose(fptr2);	
		}	
	}
	fclose(fptr1);
}

/*******************************************************************************************************/

void stock_control(void)
{
	//to print  pID of a product, its minimum stock value, its maximum stock value, 
	//and its median number of stock in all the branches.Then print to report.txt.
	
	FILE *fptr = fopen("stocks.txt","r");
	FILE *fptr2 = fopen("report.txt","w");
	
	int num,control;
	int sID,pID,pID_in_file=0;
	char string[30];
	fscanf(fptr,"%s",string);
	while(fscanf(fptr,"%d,%d",&sID,&num) != EOF)
	{
		if(num>pID)
		{
			pID_in_file=num;
		}
		fscanf(fptr,"%s",string);
	}
	fclose(fptr);
	
	char c,branch[30];
	int stock,arr[pID_in_file+1][10],i,j,k=0;
	for(i=1;i<=pID_in_file;i++)
	{
		fptr= fopen("stocks.txt","r");
		fscanf(fptr,"%s",string);
		k=0;
		while(fscanf(fptr,"%d,%d,",&sID,&pID) != EOF)
		{
			j=0;
			while((c=fgetc(fptr)) != ',')
			{
				branch[j] = c;
				j++;
			}
			branch[j] = '\0';
			fscanf(fptr,"%d",&stock);
			if(i==pID)
			{
				arr[pID][k]=stock;
				k++;
			}
		}
		arr[i][k]=-1;
		fclose(fptr);
	}
	
	int cntr[pID_in_file+1];
	for(i=1;i<=pID_in_file;i++)
	{
		cntr[i]=0;
	}
	int max_val[pID_in_file+1],min_val[pID_in_file+1];
	for(i=1;i<=pID_in_file;i++)
	{
		max_val[i]=0;
		for(j=0;arr[i][j]!=(-1);j++)
		{
			if(arr[i][j]>max_val[i])
			{
				max_val[i]=arr[i][j];
			}
			cntr[i]++;
		}
	}
	
	for(i=1;i<=pID_in_file;i++)
	{
		min_val[i]=arr[i][0];
		for(j=0;arr[i][j]!=(-1);j++)
		{
			if(arr[i][j]<min_val[i])
			{
				min_val[i]=arr[i][j];
			}
		}
	}
	
	float arr2[pID_in_file+1][10];
	for(i=1;i<=pID_in_file;i++)
	{
		for(j=0;arr[i][j]!=(-1);j++)
		{
			arr2[i][j]=arr[i][j];
		}
	}
	

	float median[pID_in_file];
	int m;
	for(i=1;i<=pID_in_file;i++)
	{
		if((cntr[i]%2)==1)
		{
			m=cntr[i]/2;
			median[i]=arr[i][m];	
		}
		else if((cntr[i]%2)==0)
		{
			m=cntr[i]/2;
			m--;
			median[i]=(((arr[i][m]) + (arr[i][m+1]))/2);
		}
	}

	fprintf(fptr2,"pID,Max Value,Min Value,Median\n");
	for(i=1;i<=pID_in_file;i++)
	{
		fprintf(fptr2,"%d,%d,%d,%.2f\n",i,max_val[i],min_val[i],median[i]);	
	}
	
	fclose(fptr2);
}

