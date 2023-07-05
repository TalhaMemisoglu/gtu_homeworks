#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *part1(int size);
int *part2(int *arr1, int size);
float part3();

typedef struct{
	int ID;
	int age;
	float gpa;
}std;

int main(int argc, char *argv[]) {
	
	int *arr1;
	int size;
	printf("Enter the size of the array: ");
	scanf("%d",&size);
	arr1 = part1(size);
	int i,min_value=arr1[0];
	for(i=0;i<size;i++)
	{
		if(arr1[i]<min_value) min_value=arr1[i];
	}
	printf("Min of the array elements: %d\n\n",min_value);
	
	/********************************************************/
	
	printf("Part2\n");
	printf("Enter the size of the array: ");
	scanf("%d",&size);
	arr1 = part1(size);
	
	int *arr2;
	arr2 = part2(arr1,size);
	
	printf("First array: ");
	for(i=0;i<size;i++) printf("%d ",arr1[i]);
	printf("\n");
	
	printf("Second array (cumulative sum): ");
	for(i=0;i<size;i++) printf("%d ",arr2[i]);
	printf("\n\n");
	
	free(arr1);
	free(arr2);
	
	/********************************************************/
	
	printf("Part3\n");
	float sum=0;
	for(i=0;i<10000;i++)
	{
		sum += part3();
	}
	printf("Average GPA of 10.000 x 10.000 students: %.4f",sum/10000);
	return 0;
}

int *part1(int size)
{
	int *arr;
	arr = (int *) calloc(size,sizeof(int));
	
	int i;
	for(i=0;i<size;i++)
	{
		printf("%d: ",i+1);
		scanf("%d",&arr[i]);
	}
	
	return arr;
}

int *part2(int *arr1, int size)
{
	int *arr2;
	arr2 = (int *) calloc(size,sizeof(int));
	
	int sum=0;
	int i,j;
	for(i=0;i<size;i++)
	{
		sum += arr1[i];
		arr2[i]=sum;
	}
	
	return arr2;
}

float part3()
{
	std *students;
	students = (std*) calloc(10000,sizeof(std));
	
	float sum=0,average;
	float a=4.0;
	srand(time(0));
	int i;
	for(i=0;i<10000;i++)
	{
		students[i].ID = rand();
		students[i].age = rand()%60;
		students[i].gpa = ((float)rand()/(float)(RAND_MAX)) * a;
		sum += students[i].gpa;
	}
	average = sum/10000;
	free(students);
	return average;
}
