#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void part1(void);
char conversion_function(char character);
void determining_of_rank_and_existing_function(char character, int *rank, int *existing_status);
void printing_function(int arr[]);

int color_generator(int color1, int color2);
void colorMixer(int color1, int color2, int (*f)(int color1, int color2));
void part2(void);

void X_O_X_printing_function(const char arr[][3]);
int control_function(const char arr[][3],char *winning_player);
void part3(void);

int main(int argc, char *argv[]) {
	
	part1();
	part2();
	part3();
	return 0;
}

void part1(void)
{
	printf("Enter the file name:");
	char file_name[30];
	scanf("%s",&file_name);//to take input for file name from the user.
	
	FILE *fp;
	fp = fopen(file_name,"r");
	int arr[26];
	
	int i;
	for(i=0;i<26;i++)
	{
		arr[i]=0;//In the beginning status, numbers of each letter is zero.
	}
	
	int rank,existing_status;
	char c;
	while((c= fgetc(fp)) != EOF)
	{
		c=conversion_function(c);
		determining_of_rank_and_existing_function(c, &rank, &existing_status);
		arr[rank] += existing_status;//to determine what letter is how many.
	}
	
	printing_function(arr);
}

char conversion_function(char character)//to convert capital letters to small letters.
{
	if(character>='A' && character<='Z')
	{
		int i;
		for(i=0;i<32;i++)
		{
			character++;
		}
	}
	
	return character;
}

void determining_of_rank_and_existing_function(char character, int *rank, int *existing_status)
{
	*rank=0,*existing_status=0;
	char iterator;
	
	for(iterator='a';iterator<='z';iterator++)
	{
		if(iterator==character) 
		{
			*existing_status=1;//if the character exist, let "existing_status" be 1.
			break;
		}	
		(*rank)++;//the rank of each letter is 1 less than its rank in the alphabet.
	}	
}

void printing_function(int arr[])
{
	int count=0;
	char iterator;
	printf("\nLetter Frequency:\n");
	for(iterator='A';iterator<='Z';iterator++)
	{
		printf("%c:%d\n",iterator,arr[count]);
		count++;
	}
	printf("\n");
}

//************************************************************************************************************//

typedef enum {RED,GREEN,BLUE,YELLOW,ORANGE}color;

int color_generator(int color1, int color2)
{
	//0=RED, 1=GREEN, 2=BLUE, 3=YELLOW, 4=ORANGE in array given below.
	float color_values[5][3]={{1,0,0},{0,1,0},{0,0,1},{0.5,0.5,0},{0.5,0.4,0.2}};
	float z1,z2,z3;
	
	//to calculate values of new color.
	z1=(color_values[color1][0]+color_values[color2][0])/2;
	z2=(color_values[color1][1]+color_values[color2][1])/2;
	z3=(color_values[color1][2]+color_values[color2][2])/2;
	
	float d1,d2;
	int minimum_distance=0,number_of_new_color=0;
	int i;
	for(i=1;i<5;i++)//to determine which euclidean distance between new color and each color is closest one.
	{
		//to calculate euclidean distance between new color and each color.
		d1=sqrt(pow(z1-color_values[minimum_distance][0],2)+pow(z2-color_values[minimum_distance][1],2)+pow(z3-color_values[minimum_distance][2],2));
		d2=sqrt(pow(z1-color_values[i][0],2)+pow(z2-color_values[i][1],2)+pow(z3-color_values[i][2],2)); 
		if(d2<d1)
		{
			minimum_distance=i;
		}
	}
	number_of_new_color=minimum_distance;
	
	return number_of_new_color;
}

void colorMixer(int color1, int color2, int (*f)(int color1, int color2))
{
	int  new_color=f(color1,color2);
	switch(new_color)
	{
		case RED:
			printf("Mixed Color: RED [1, 0, 0]\n\n");
			break;
		case GREEN:
			printf("Mixed Color: GREEN [0, 1, 0]\n\n");
			break;
		case BLUE:
			printf("Mixed Color: BLUE [0, 0, 1]\n\n");
			break;
		case YELLOW:
			printf("Mixed Color: YELLOW [0.5, 0.5, 0]\n\n");
			break;
		case ORANGE:
			printf("Mixed Color: ORANGE [0.5, 0.4, 0.2]\n\n");
			break;
		default:
			printf("ERROR");
	}
}

void part2(void)
{
	char color1,color2;
	printf("Enter Color 1 (r,g,b,y,o): ");
	scanf(" %c",&color1);
	printf("Enter Color 2 (r,g,b,y,o): ");
	scanf(" %c",&color2);

	switch(color1)
	{
		case 'r':
			switch(color2)
			{
				case 'r':
					colorMixer(RED,RED,&color_generator);
					break;					
				case 'g':
					colorMixer(RED,GREEN,&color_generator);
					break;
				case 'b':
					colorMixer(RED,BLUE,&color_generator);
					break;
				case 'y':
					colorMixer(RED,YELLOW,&color_generator);
					break;
				case 'o':
					colorMixer(RED,ORANGE,&color_generator);
					break;
				default:
					printf("Entered invalid type for color2");
			}
			break;
		case 'g':
			switch(color2)
			{
				case 'r':
					colorMixer(GREEN,RED,&color_generator);
					break;					
				case 'g':
					colorMixer(GREEN,GREEN,&color_generator);
					break;
				case 'b':
					colorMixer(GREEN,BLUE,&color_generator);
					break;
				case 'y':
					colorMixer(GREEN,YELLOW,&color_generator);
					break;
				case 'o':
					colorMixer(GREEN,ORANGE,&color_generator);
					break;
				default:
					printf("Entered invalid type for color2");
			}
			break;
		case 'b':
			switch(color2)
			{
				case 'r':
					colorMixer(BLUE,RED,&color_generator);
					break;					
				case 'g':
					colorMixer(BLUE,GREEN,&color_generator);
					break;
				case 'b':
					colorMixer(BLUE,BLUE,&color_generator);
					break;
				case 'y':
					colorMixer(BLUE,YELLOW,&color_generator);
					break;
				case 'o':
					colorMixer(BLUE,ORANGE,&color_generator);
					break;
				default:
					printf("Entered invalid type for color2");
			}
			break;
		case 'y':
			switch(color2)
			{
				case 'r':
					colorMixer(YELLOW,RED,&color_generator);
					break;					
				case 'g':
					colorMixer(YELLOW,RED,&color_generator);
					break;
				case 'b':
					colorMixer(YELLOW,RED,&color_generator);
					break;
				case 'y':
					colorMixer(YELLOW,RED,&color_generator);
					break;
				case 'o':
					colorMixer(YELLOW,RED,&color_generator);
					break;
				default:
					printf("Entered invalid type for color2");
			}
			break;
		case 'o':
			switch(color2)
			{
				case 'r':
					colorMixer(ORANGE,RED,&color_generator);
					break;					
				case 'g':
					colorMixer(ORANGE,GREEN,&color_generator);
					break;
				case 'b':
					colorMixer(ORANGE,BLUE,&color_generator);
					break;
				case 'y':
					colorMixer(ORANGE,YELLOW,&color_generator);
					break;
				case 'o':
					colorMixer(ORANGE,ORANGE,&color_generator);
					break;
				default:
					printf("Entered invalid type for color2");
			}
			break;
		default:
			printf("Entered invalid type for color1");	
	}
}

//************************************************************************************************************//

void X_O_X_printing_function(const char arr[][3])
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			printf("%c ",arr[i][j]);
		}
		printf("\n");
	}
}

int control_function(const char arr[][3],char *winning_player)
{
	int game_status=0,numbers_of_spaces=0;
	int i,j;
	for(i=0;i<3;i++)//checking for win status in line.
	{
		if(arr[i][0]==arr[i][1] && arr[i][1]==arr[i][2])
		{
			if(arr[i][0] != '_')
			{
				*winning_player=arr[i][0];
				game_status=1;
				break;
			}
		}
	}
	for(i=0;i<3;i++)//checking for win status in column.
	{
		if(arr[0][i]==arr[1][i] && arr[1][i]==arr[2][i])
		{
			if(arr[0][i]!='_')
			{
			*winning_player=arr[0][i];
			game_status=1;
			break;
			}
		}
	}
	
	if(game_status!=1)//check if no win status found.
	{
		if(arr[0][2]==arr[1][1] && arr[1][1]==arr[2][0])//checking for win status in cross line.
		{
			if(arr[0][2]!='_')
			{
			*winning_player=arr[1][1];
			game_status=1;
			}
		}
		else if(arr[0][0]==arr[1][1] && arr[1][1]==arr[2][2])//checking for win status in cross line.
		{
			if(arr[0][0]!='_')
			{
			*winning_player=arr[1][1];
			game_status=1;
			}
		}
	}
	
	if(game_status!=1)
	{
		for(i=0;i<3;i++)//to calculate how many space in the game. 
		{
			for(j=0;j<3;j++)
			{
				if(arr[i][j]=='_') numbers_of_spaces++;			
			}
		}
		
		if(numbers_of_spaces==0)//if there is no space in the game, winning player is 'N'obody.
		{
		*winning_player='N';
		game_status=1;
		}
	}
	
	return game_status;
}

void part3(void)
{
	int choice,winning_numbers_of_player_1=0,winning_numbers_of_player_2=0;
	while(1)
	{
		printf("1.New Game\n");
		printf("2.Exit\n");
		printf("Enter your choice: ");
		scanf("%d",&choice);
		if(choice==2) break;
		
		int i,j;
		char arr[3][3];
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				arr[i][j]='_';//to fill the board with '_'.
			}
		}
		
		int game_status=0,row,col;
		char winning_player;
		while(game_status!=1)
		{
			while(1)//to take input again until user type validly.
			{
			printf("Player 1(X), enter your move (row, col): ");
			scanf("%d %d",&row,&col);
			if(row>=0 && row<=2 && col>=0 && col<=2)//so as not to cross limits.
			{
				if(arr[row][col]=='_'){//so as not to overwrite.
					arr[row][col]='X'; 
					break;
				} 
				else printf("Invalid typing!\nTry again!\n");
			}
			else printf("Invalid typing!\nTry again!\n");
			}
			X_O_X_printing_function(arr);
			game_status=control_function(arr,&winning_player);
			
			if(game_status!=1)
			{
				while(1)//to take input again until user type validly.
				{
				printf("Player 2(O), enter your move (row, col): ");
				scanf("%d %d",&row,&col);
				if(row>=0 && row<=2 && col>=0 && col<=2)//so as not to cross limits.
				{
					if(arr[row][col]=='_'){//so as not to overwrite.
						arr[row][col]='O';
						break;
					} 	 
					else printf("Invalid typing!\nTry again!\n");
				}
				else printf("Invalid typing!\nTry again!\n");
				}
				X_O_X_printing_function(arr);
				game_status=control_function(arr,&winning_player);
			}
		}
		
		switch(winning_player)
		{
			case 'X':
				winning_numbers_of_player_1++;//to calculate numbers of winning.
				printf("Player 1 (X) wins!\n");
				break;
			case 'O':
				winning_numbers_of_player_2++;//to calculate numbers of winning.
				printf("Player 2 (O) wins!\n");
				break;
			case 'N':
				printf("Nobody wins!\n");
				break;
			default:
				printf("ERROR\n");
		}
	}
	printf("Win Status:\n");
	printf("Player 1: %d\n",winning_numbers_of_player_1);
	printf("Player 2: %d\n",winning_numbers_of_player_2);
}
