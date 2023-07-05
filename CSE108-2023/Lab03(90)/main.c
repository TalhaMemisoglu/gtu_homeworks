#include <stdio.h>

void part1(void);
void part2(void);
void part3(void);


int main() {
	
	part3();
	part1();
	part2();
	
	return 0;
}

void part1(void)
{
	int num1,num2,num3;
	
	printf("Enter 3 integers: ");
	scanf("%d %d %d",&num1,&num2,&num3);
	
	if(num1>num2){
		if(num1>num3){
			if(num2>num3){
				printf("Median number is %d\n",num2);
			}
			else{
				printf("Median number is %d\n",num3);
			}
		}
	}
	if(num2>num1){
		if(num2>num3){
			if(num3>num1){
				printf("Median number is %d\n",num3);
			}
			else{
				printf("Median number is %d\n",num1);
			}
		}
	}
	if(num3>num2){
		if(num3>num1){
			if(num2>num1){
				printf("Median number is %d\n",num2);
			}
			else{
				printf("Median number is %d\n",num1);
			}
		}
	}
}

void part2(void)
{
	float grade1,grade2,grade3,average;
	
	printf("Enter your scores: ");
	scanf("%f %f %f",&grade1,&grade2,&grade3);
	
	int letter_grade;
	
	average=(grade1+grade2+grade3)/3;
		
	if(average>=90 && average<=100) letter_grade=1;
	if(average>=80 && average<90) 	letter_grade=2;
	if(average>=70 && average<80) 	letter_grade=3;
	if(average>=60 && average<70) 	letter_grade=4;
	if(average>=50 && average<60) 	letter_grade=5;
	if(average>=40 && average<50) 	letter_grade=6;
	if(average>=0 && average<40) 	letter_grade=7;
	
	switch(letter_grade)
	{
		case 1:
			printf("Your letter grades are A+ with an %.2f average\n",average);
			break;
		case 2:
			printf("Your letter grades are A with an %.2f average\n",average);
			break;
		case 3:
			printf("Your letter grades are B+ with an %.2f average\n",average);
			break;
		case 4:
			printf("Your letter grades are B with an %.2f average\n",average);
			break;
		case 5:
			printf("Your letter grades are C with an %.2f average\n",average);
			break;
		case 6:
			printf("Your letter grades are D with an %.2f average\n",average);
			break;
		case 7:
			printf("Your letter grades are F with an %.2f average\n",average);
			break;
		default:
			printf("ERROR\n");
	}
	
}

void part3(void)
{
	char operation;
	int num1,num2,result;

	printf("Enter an arithmetic operation(+,-,*,/): ");
	scanf("%c",&operation);
	printf("Enter two numbers: ");
	scanf("%d %d",&num1,&num2);
	
	switch(operation)
	{
		case '+':
			printf("Result: ");
			scanf("%d",&result);
			if(result==num1+num2) printf("Bravo,correct answer!\n"); 
			else printf("Wrong answer, try again\n");
			break;
		case '-':
			printf("Result: ");
			scanf("%d",&result);
			if(result==num1-num2) printf("Bravo,correct answer!\n"); 
			else printf("Wrong answer, try again\n");
			break;
		case '*':
			printf("Result: ");
			scanf("%d",&result);
			if(result==num1*num2) printf("Bravo,correct answer!\n"); 
			else printf("Wrong answer, try again\n");
			break;
		case '/':
			printf("Result: ");
			scanf("%d",&result);
			if(result==num1/num2) printf("Bravo,correct answer!\n"); 
			else printf("Wrong answer, try again\n");
			break;	
		default:
			printf("ERROR");
	}
}
