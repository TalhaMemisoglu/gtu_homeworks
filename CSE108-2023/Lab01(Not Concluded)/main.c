#include <stdio.h>
#include <stdlib.h>

void fizz_buzz(void);
void X_Y_Z(void);
void calculation_of_salary(void);

int main(int argc, char *argv[]) {
	
	fizz_buzz();
	X_Y_Z();
	calculation_of_salary();		
	return 0;
}

void fizz_buzz(void)
{
	int num;
	printf("Enter your an integer value between 1 and 100:");
	scanf("%d",&num);
		
	if(num%3==0)
	{
		printf("Fizz");
	}
	if(num>=5 && num<=50)
	{
		printf("Buzz");	
	}
	printf("\n");
}

void X_Y_Z(void)
{
	float numx,numy,numz;
	printf("Enter X decimal number:");
	scanf("%f",&numx);
	printf("Enter Y decimal number:");
	scanf("%f",&numy);
	
	if(numx>numy){
		numz=(numx/numy)+(numx/(numx+numy));
		printf("%f\n",numz);
	}
	else{
		numz=(numx/numy)+(numy/(numx+numy));
		printf("%f\n",numz);
	}
}

void calculation_of_salary(void)
{
	int age,experience;
	printf("Enter your age:");
	scanf("%d",&age);
	printf("Enter your years of experience:");
	scanf("%d",&experience);
	
	if(age<=20 && experience>=0 && experience<=10) printf("Your salary is 10.000\n");
	if(age<=20 && experience>10) printf("Error\n");
	if(age>20 && age<=50 && experience>=0 && experience<=10) printf("Your salary is 15.000\n");
	if(age>20 && age<=50 && experience>10) printf("Your salary is 20.000\n");
	if(age>50 && experience>=0 && experience<=10) printf("Your salary is 20.000\n");
	if(age>50 && experience>10) printf("Your salary is 25.000\n");
}
