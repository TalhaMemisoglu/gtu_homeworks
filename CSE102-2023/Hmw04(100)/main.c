#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main_function_of_printing_of_receipt(void);
void rock_paper_scissors_game(void);

int main() 
{
	main_function_of_printing_of_receipt();
	rock_paper_scissors_game();
	return 0;
}
void printing_to_console(void)
{
	FILE *menu;
	menu = fopen("menu.txt","r");
	
	char c;
	int num=1;
	
	fseek(menu,SEEK_CUR+22,SEEK_CUR);//for skipping first line.
	while((c=fgetc(menu)) != EOF)
	{
		printf("%c",c);
		if(c == '\n') 
		{
			if(num>=10) printf("%d.",num); else printf("%d. ",num);//numbers which is bigger than 9 already occupy one space.So I deleted one space. 
			num++;
		}
	}
	
	fclose(menu);	
}

void printing_to_console2(void)
{
	FILE *receipt;
	receipt = fopen("receipt.txt","r");
	
	char c;
	while((c=fgetc(receipt)) != EOF)
	{
		printf("%c",c);
	}
	
	fclose(receipt);
}

float f(int selection, int numbers_of_serving)
{
	//In this function,function read from menu.txt and write to bill.txt.After that it calculates prices and returns that value.	
	
	FILE *file1;
	file1 = fopen("menu.txt","r");
	
	FILE *file2;
	file2 = fopen("receipt.txt","a");
	
	float price,wanted_price;
	int digitcounter=0,c,count=1;
	
	do
	{
	if((c=fgetc(file1)) == '\n')//for reading lines one by one.
	{
		if(count==selection)//if we are in line that user wants, go on.
		{
			if(numbers_of_serving>1) fprintf(file2," %d*",numbers_of_serving); else fprintf(file2," ");
		}
		while((c=fgetc(file1)) != ' ')//function reads value that written in menu.txt after space.
		{
			if(count==selection) 
			{
				fprintf(file2,"%c",c);
				digitcounter++;	
			}
		}
		if(count==selection)
		{
			int i,space;
			
			if(numbers_of_serving>1) space=29; else space=31;

			for(i=0;i<space-digitcounter;i++)//for entering enough space(not much not less.)
			{
				fprintf(file2," ");
			}
		}
		fscanf(file1,"%f",&price);
		if(count==selection) 
		{
			wanted_price=price;//wanted_price is price which is wanted price of dish.
			fprintf(file2,"%.2f\n",numbers_of_serving*wanted_price);
		}
		count++;
	}
	}
	while(c!=EOF);
	
	fclose(file1);
	fclose(file2);
	
	return numbers_of_serving*wanted_price;//That value will help us to calculate total price.
}

void set_of_receipt(void)
{
	//This function write beginning of receipt.txt
	FILE *file0;
	file0 = fopen("receipt.txt","w");
	
	fprintf(file0,"\n 210104004009    ");
	time_t t = time(NULL);
	fprintf(file0,"%s\n",ctime(&t));
	fprintf(file0,"-----------------------------------------\n");
	fprintf(file0,"\n Product                       Price(TL)\n\n");
	fprintf(file0,"-----------------------------------------\n\n");
	
	fclose(file0);	
}

void main_function_of_printing_of_receipt(void)
{	
	set_of_receipt();	
	printf("Yemek Listesi:");
	printing_to_console();
	printf("\n\n");
	int selection,numbers_of_serving=1;
	
	FILE *file;
	file = fopen("receipt.txt","a");
	
	float total=0;
	
	while(numbers_of_serving!=0)
	{
	printf("Please choose a product:");
	scanf("%d",&selection);
	
	printf("How many servings do you want?:");
	scanf("%d",&numbers_of_serving);
	if(numbers_of_serving!=0)//If user doesn't enter '0',calculate total price via code below.
	{
		total += f(selection,numbers_of_serving);
	}
	}
	
	fprintf(file," Total:                         %.2f\n",total);
	
	float student_discount=0,discount_over_150=0;
	
	char student;
	int set=1;
	while(set==1)
	{
		printf("Are you student?(Y/N): ");
		scanf(" %c",&student);
		switch(student)
		{
			case 'Y':
			case 'y':
				student_discount = (total*12.5)/100;
				fprintf(file," Student discount:             -%.2f\n",student_discount);
				set=0;
				break;
			case 'N':
			case 'n':
				set=0;
				break;
			default:
				printf("Please enter a valid answer!");		
		}
	}
	if(total>=150)
	{
		discount_over_150=(total*1)/10;
		fprintf(file," Discount over 150TL:          -%.2f\n",discount_over_150);
	}
	
	fprintf(file,"\n-----------------------------------------\n\n");
	fprintf(file," Price:                         %.2f\n",total-discount_over_150-student_discount);
	fprintf(file," Price + VAT:                   %.2f\n",((total-discount_over_150-student_discount)*118)/100);
	
	fclose(file);
	
	printing_to_console2();
}

void rock_paper_scissors_game(void)
{
	char want_to_play = 'Y';
	int movement_choice;
 	int movement_choice_of_game_bot=0; // to enter in first encounter to while loop which create random numbers.
 	int number_of_game=0,number_of_win=0;
	
	srand(time(NULL));
	
	while(want_to_play!='n' && want_to_play!='N')
	{
		printf("\n\nPlease make a choice!\n");
		printf("1: Stone, 2: Paper, 3: Scissors\n");
		printf("Enter your selection:");
		scanf("%d",&movement_choice);
		
		movement_choice_of_game_bot=rand()%10;
		while(movement_choice_of_game_bot>3 || movement_choice_of_game_bot<1)
		{
			movement_choice_of_game_bot=rand()%10;
		}

		switch(movement_choice)
		{
			case 1:
				printf("You chose Stone.");
				switch(movement_choice_of_game_bot)
				{
					case 1:
						printf("I chose Stone.It's a tie!\n");
						number_of_game++;
						break;
					case 2:
						printf("I chose Paper.I won!\n");
						number_of_game++;
						break;
					case 3:
						printf("I chose Scissors.You won!\n");
						number_of_game++;
						number_of_win++;
						break;
					default:
						printf("ERROR\n");
						printf("It was occurred error of choosing random selection!\n");		
				}
				break;
			case 2:
				printf("You chose Paper.");
				switch(movement_choice_of_game_bot)
				{
					case 1:
						printf("I chose Stone.You won!\n");
						number_of_game++;
						number_of_win++;
						break;
					case 2:
						printf("I chose Paper.It's tie!\n");
						number_of_game++;
						break;
					case 3:
						printf("I chose Scissors.I won!\n");
						number_of_game++;
						break;
					default:
						printf("ERROR\n");
						printf("It was occurred error of choosing random selection!\n");	
				}
				break;
			case 3:
				printf("You chose Scissors.");
				switch(movement_choice_of_game_bot)
				{
					case 1:
						printf("I chose Stone.I won!\n");
						number_of_game++;
						break;
					case 2:
						printf("I chose Paper.You won!\n");
						number_of_game++;
						number_of_win++;
						break;
					case 3:
						printf("I chose Scissors.It's tie!\n");
						number_of_game++;
						break;
					default:
						printf("ERROR\n");
						printf("It was occurred error of choosing random selection!\n");	
				}
				break;
			default:
				printf("ERROR\n");
				printf("Please chose valid selection!\n");
				printf("Redirecting to main menu...\n\n");
		}
		
		if(movement_choice<=3 && movement_choice>0) //If user chose invalid number,The program will keep going without asking whether user want to play again.
		{
			printf("Do you want to play again?(Y/N):");
			scanf(" %c",&want_to_play);
			printf("\n");
			while(want_to_play!= 'N' && want_to_play!= 'Y' && want_to_play!= 'n' && want_to_play!= 'y')
			{
				printf("Please chose only Y/N!\n");
				printf("Do you want to play again?(Y/N):");
				scanf(" %c",&want_to_play);
				printf("\n");
			}
		}
	}
	
	printf("GAME OVER!\n");
	printf("You won %d out of %d games.\n",number_of_win,number_of_game);
	
}
