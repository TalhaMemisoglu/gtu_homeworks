#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	part1();
	part2();
	part3();
	
	return 0;
}

float celsius_to_fahrenheit(float value)
{
	return ((value*9)/5)+32;
}

float fahrenheit_to_celsius(float value)
{
	return ((value-32)*5)/9;
}

void part1(void)
{
	int choice;
	float value;
	
	while(choice!=3)
	{
		printf("Temperature Conversion Menu\n");
		printf("1.Convert Celsius to Fahrenheit\n");
		printf("2.Convert Fahrenheit to Celsius\n");
		printf("3.Quit\n");
		printf("Enter your choice(1-3):");
		scanf("%d",&choice);
	
		switch(choice)
		{
			case 1:
				printf("Enter the temperature value to convert:");
				scanf("%f",&value);
				printf("%.2f Celsius = %.2f Fahrenheit\n\n",value,celsius_to_fahrenheit(value));
				break;
			case 2:
				printf("Enter the temperature value to convert:");
				scanf("%f",&value);
				printf("%.2f Fahrenheit = %.2f Celsius\n\n",value,fahrenheit_to_celsius(value));
				break;
			case 3:
				printf("Quitting...");
				break;
			default:
				printf("Entered invalid key!");
		}	
	}
}

int digit_counting(int num)
{
	int count=0;
	
	while(num !=0)
	{
		count++;
		num /=10;
	}
	
	return count;
}


void part2(void)
{
	int number,exit_status=0;
	
	while(exit_status!=1)
	{
		printf("Enter a number (3,4or 5):");
		scanf("%d",&number);
	
		if(digit_counting(number)<3 || digit_counting(number)>5)
		{
		 	printf("Invalid input. Please enter a 3, 4 or 5 digit integer.\n");
		 	exit_status=0;
		}
		else
		{
			exit_status=1;	
		}	
	}
	
	int digit,reverse_num=0;
	
	while(number!=0)
	{
		digit = number%10;
		reverse_num = reverse_num*10+digit;
		number /=10;
	}
	
	printf("Reversed number:%d",reverse_num);
}

long long to_binary(int n) 
{
  long long bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

return bin;
}

void part3(void)
{
    int o;
    int num;
    while (1)
    {
        printf("Menu:\n");
        printf("1. Convert a number to decimal, binary, octal, and hexadecimal\n");
        printf("2. Quit\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &o);
        switch (o)
        {
            case 1:
                printf("Enter a number: ");
                scanf("%d", &num);
                printf("Decimal equivalent: %d\n", num);
                printf("Binary equivalent: %lld\n", to_binary(num));
                printf("Octal equivalent: %o\n", num);
                printf("Hexadecimal equivalent: %x\n", num); 
                break;
            case 2:
                return ;
		}
	}
}
