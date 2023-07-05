#include <stdio.h>
#define MAX_NUMBER 50

void part1(void);
void part2(void);
void labeling_array(int arr[]);
void printing_function(int arr[],char larr[]);

int main() 
{
	part1();
	part2();
	return 0;
}

void part1(void)
{
	int num[MAX_NUMBER];
	int i=0;
	
	printf("%d. num:",i+1);
	scanf("%d",&num[i]);
	while(num[i]!=(-100))
	{
		i++;
		printf("%d. num:",i+1);
		scanf("%d",&num[i]);
	}

	labeling_array(num);
}

void labeling_array(int arr[])
{
	char larr[MAX_NUMBER];
	int i=0;
	
	while(arr[i]!=(-100))
	{
		if(arr[i]%2==0) 
		{
			larr[i]='e';	
		}
		else 
		{
			larr[i]='o';	
		}
		i++;
	}
	
	printing_function(arr,larr);
}

void printing_function(int arr[],char larr[])
{
	int i=0;
	
	while(arr[i]!=(-100))
	{
		printf("%d %c\n",arr[i],larr[i]);
		i++;
	}
}

void part2(void)
{
	FILE *fpt;
	fpt=fopen("text.txt","r");
	
	int row;
	fscanf(fpt,"%d",&row);
	
	int age[row];
	char occ[row];
	float salary[row];
	char ft[row];
	int i;
	for(i=0;i<row;i++)
	{
		fscanf(fpt,"%d %c %f %c",&age[i],&occ[i],&salary[i],&ft[i]);
	}
	
	char ch;
	printf("\nPlease select a team:");
	scanf(" %c",&ch);
	
	float num=0,sum=0;
	for(i=0;i<row;i++)
	{
		if(ft[i]== ch)
		{
			num++;
			sum += salary[i];
		}
	}
	
	if(sum>0)
	{
		printf("Average salaries of fans of %c: %.1f ",ch,sum/num);
	}
	else
	{
		printf("There are no fans of %c in the database!",ch);
	}
	
	fclose(fpt);
}


