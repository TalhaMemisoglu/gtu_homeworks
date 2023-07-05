#include <stdio.h>
#include "util.h"

int main()
{
	int num1,num2;
	printf("Enter two integers to find GCD of them: ");
	scanf("%d %d",&num1,&num2);
	gcd_of_two_integers(num1,num2);
	printf("Enter two numbers that you want to sum: ");
	scanf("%d %d",&num1,&num2);
	sum_of_two_numbers(num1,num2);
	printf("Enter two numbers that you want to multiply: ");
	scanf("%d %d",&num1,&num2);
	mltp_of_two_numbers(num1,num2);
	printf("Enter an integer between 1 and 10: ");
	scanf("%d",&num1);
	calculation_of_range(num1);
	return 0;	
}
