#include <stdio.h>
#include <stdlib.h>

int main() 
{
	
	int menu_selection=2;
	
	while(1)
	{
	printf("Welcome to Shape Reader! Please make your choice to continue:\n");
	printf("1-) Generate a shape file\n");
	printf("2-) Read and draw a shape file\n");
	printf("3-) Terminate the program\n");
	while(menu_selection==2)
	{
		printf("Enter your choice:");
		scanf("%d",&menu_selection);
		if(menu_selection==2) printf("Please choose 1 firstly!\n");
	}
	
	if(menu_selection==3) break;
	
	print_function();
	
	char shape,c;
	int size;
	
	FILE *finput;
	finput=fopen("input.txt","r");
	
	FILE *foutput;
	foutput=fopen("output.txt","w");
	
	while(1)
	{
		fscanf(finput," %c,%d\n",&shape,&size);
		if(shape=='e') break;
		if(shape=='s') drawing_square(size);
		if(shape=='t') drawing_triangle(size);
	}
	
	break;
	}
	
	return 0;
}

void print_function(void)
{
	
	FILE *finput;
	finput=fopen("input.txt","w");
	
	char shape;
	int size;
	
	while(1)
	{
		printf("Please enter shape type:");
		scanf(" %c",&shape);
		fprintf(finput,"%c",shape);
		if(shape=='e') break;
		
		fprintf(finput,",");
		
		printf("Please enter size:");
		scanf(" %d",&size);
		fprintf(finput,"%d",size);
		
		fprintf(finput,"\n");	
	}
	
	fclose(finput);
}

void drawing_square(int size)
{
	FILE *foutput;
	foutput=fopen("output.txt","a");
	
	int i,j;
	
	for(i=0;i<size;i++)
	{
		fprintf(foutput,"* ");
	}
	
	fprintf(foutput,"\n");
	
	for(i=0;i<size-2;i++)
	{
		fprintf(foutput,"*");
		for(j=0;j<2*(size-2)+1;j++)
		{
			fprintf(foutput," ");
		}
		fprintf(foutput,"*\n");
	}
	
	for(i=0;i<size;i++)
	{
		fprintf(foutput,"* ");
	}
	
	fprintf(foutput,"\n");
	fclose(foutput);
}

void drawing_triangle(int size)
{
	FILE *foutput;
	foutput=fopen("output.txt","a");
	
	int i,j;
	
	for(i=1; i<=size; i++)
    {
        for(j=i; j<size; j++)
        {
            fprintf(foutput," ");
        }

        for(j=1; j<=(2*i-1); j++)
        {
            if(i==size || j==1 || j==(2*i-1))
            {
                fprintf(foutput,"*");
            }
            else
            {
                fprintf(foutput," ");
            }
        }
        fprintf(foutput,"\n");
    }
	fprintf(foutput,"\n");
	fclose(foutput);
}


