#include <stdio.h>
#include "util.h"

void printfunction(int firstnumber, int secondnumber)
{
	printf("First number is:%d\n",firstnumber);
	printf("Second number is:%d\n",secondnumber);
}

void gcd_of_two_integers(int num1, int num2)
{	
	int temp;
	
	if(num1<num2) temp=num1,num1=num2,num2=temp;

	while(num1%num2!=0)
	{
		temp=num1%num2;
		num1=num2;
		num2=temp;
	}
	
	printf("GCD of them:%d\n",num2);
}

void sum_of_two_numbers(int num1, int num2)
{
	
	printfunction(num1,num2);
	printf("Result:\n");
	printf("%11d\n%11d\n",num1,num2);
	printf("     +\n     ------\n");
	printf("%11d\n",num1+num2);
}

void mltp_of_two_numbers(int num1, int num2)
{
	printfunction(num1,num2);
	int num1_clone;
	int digit_number=0;
	num1_clone=num1;
	while(num1_clone!=0) /*to determine how much zero I will display in case num2 or ones digit of num2 is equal to zero*/
	{
		num1_clone=num1_clone/10;		
		digit_number++;
	}
	
	int ones_digit,tens_digit,hundreds_digit;
	ones_digit=(num2%100)%10;
	tens_digit=(num2/10)%10;
	hundreds_digit=num2/100;
	
	printf("Result:\n");
	printf("%12d\n%12d\n",num1,num2);
	printf("     *\n");
	printf("     -------\n");
	
	if(num2==0 || ones_digit==0){
		if(digit_number==1) printf("           0\n");
		if(digit_number==2) printf("          00\n");
		if(digit_number==3) printf("         000\n");	
	}
	if(ones_digit>0)    printf("%12d\n",num1*ones_digit);
	
	if(tens_digit==0){
		if(digit_number==1) printf("          0\n");
		if(digit_number==2) printf("         00\n");
		if(digit_number==3) printf("        000\n");	
	}
	if(num2>=10 && tens_digit>0)  printf("%11d\n",num1*tens_digit);
	
	if(num2>=100) printf("%10d\n",num1*hundreds_digit);
	
	printf("     +\n");
	printf("     -------\n");
	printf("%12d\n",num1*num2);
}

void calculation_of_range(int num)
{
	if(num>0 && num<=5)  printf("The integer you entered is less than or equal to 5\n");
	if(num>5 && num<=10) printf("The integer you entered is greater than 5\n");
	if(num<0 || num>10)  printf("Invalid input\n");
}
