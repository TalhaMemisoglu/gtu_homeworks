#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void drawing_room_function(int Cx, int Cy,int Dx,int Dy, int size);
int gameplay_function(void);
int control_function(int Cx,int Cy,int Dx,int Dy,int size);

int main() 
{
	int want_to_play=1;
	int sum_of_wins=0,sum_of_number_of_games=0;
	
	printf("Welcome to the 2D puzzle game!\n");
	
	while(want_to_play!=0)
	{
		int menu_selection;
		
		printf("1. New Game\n");
		printf("2. Help\n");
		printf("3. Exit\n");
		printf("Enter your choice: ");
		scanf("%d",&menu_selection);
		
		switch(menu_selection)
		{
			case 1:
				sum_of_wins += gameplay_function();
				sum_of_number_of_games++;
				break;
			case 2:
				printf("The character is able to move one space in any of the four cardinal directions: up, down, left, and right. Diagonal moves are not allowed.\n");
				printf("The user will input their desired move using the following keys: 'a' for left, 'd' for right, 'w' for up, and 's' for down.\n");
				printf("Any control or selection with [a, d, w, s] characters will result in a missgrading.\n");
				printf("The game will prompt the user for a new move after each move is made.\n");
				printf("If you want to finish and quit your game.You should press 'q' in your keyboard.\n\n");
				break;
			case 3:
				want_to_play=0;
				break;
			default:
				printf("Entered invalid number.\n");
				printf("Redirecting to main menu...\n");
		}
	}
	
	printf("You won %d out of %d games\n",sum_of_wins,sum_of_number_of_games);
	printf("Goodbye :)\n");

	return 0;
}

void drawing_room_function(int Cx, int Cy,int Dx,int Dy, int size)
{
	int i,j,k;
	
	for(k=0;k<2*size+1;k++)
	{
		printf("-");	
	}
	printf("\n");
	
	for(i=size;i>=1;i--)
	{
		for(j=1;j<=size;j++)
		{
			printf("|");
			if (Cx==Dx && Cy==Dy && Cx==j && Cy==i && Dx==j && Dy==i) printf("*");
			else if(Cx==j && Cy==i) printf("C");
			else if(Dx==j && Dy==i) printf("D");
			else printf(" ");
		}
		printf("|\n");
	}
	
	for(k=0;k<2*size+1;k++)
	{
		printf("-");	
	}
	printf("\n");
}

int control_function(int Cx,int Cy,int Dx,int Dy,int size)
{
	int move_control;
	//States of control
	//0:There is no problem to move.
	//1:Winning state.
	//2:Warning state in case of situation that movement isn't allowed.
	
	if(Cy>size || Cy<=0)
	{
		printf("Warning!You can't move there.\n");
		printf("Please Try again!\n");
		move_control=2;
	}
	else if(Cx>size || Cx<=0)
	{
		printf("Warning!You can't move there.\n");
		printf("Please Try again!\n");
		move_control=2;
	}
	else if(Cx==Dx && Cy==Dy)
	{
		move_control=1;
	}
	else 
	{
		move_control=0;	
	}
	
	return move_control;
}

int gameplay_function(void)
{
	int Cx=11,Cy=11,Dx=11,Dy=11,size=0;
	int win_status;
	
	while(size<5 || size>10)
	{
		printf("Enter your game size(5 to 10):");
		scanf("%d",&size);
		if(size<5 || size>10) printf("Please, enter in the given range.\n");
	}
	
	srand(time(NULL));
	while(Cx==Dx && Cy==Dy)//In order to prevent winning the game without any effort :)
	{
		while(Cx>=size || Cx==0) Cx=rand()%10;	
		while(Cy>=size || Cy==0) Cy=rand()%10;	
		while(Dx>=size || Dx==0) Dx=rand()%10;
		while(Dy>=size || Dy==0) Dy=rand()%10;
	}
	
	int game_status=0;
	int move_counter=0;

	while(game_status==0)
	{
		char move;
		drawing_room_function(Cx,Cy,Dx,Dy,size);
		printf("Enter your move: ");
		scanf(" %c",&move);
		
		switch(move)
		{
			case 'w':
				if(control_function(Cx,Cy+1,Dx,Dy,size)==2){
					break;
				}
				else if(control_function(Cx,Cy+1,Dx,Dy,size)==0){
					Cy++; 
					move_counter++; 
					break;
				}
				else if(control_function(Cx,Cy+1,Dx,Dy,size)==1){
					Cy++;
					game_status=1;
					drawing_room_function(Cx,Cy,Dx,Dy,size); 
					move_counter++; 
					break;
				}
			case 'a':
				if(control_function(Cx-1,Cy,Dx,Dy,size)==2){
					break;
				}
				else if(control_function(Cx-1,Cy,Dx,Dy,size)==0){	
					Cx--; 
					move_counter++; 
					break;
				}
				else if(control_function(Cx-1,Cy,Dx,Dy,size)==1){
					Cx--;
					game_status=1;
					drawing_room_function(Cx,Cy,Dx,Dy,size); 
					move_counter++; 
					break;
				}
			case 's':
				if(control_function(Cx,Cy-1,Dx,Dy,size)==2){
					break;
				}
				else if(control_function(Cx,Cy-1,Dx,Dy,size)==0){
					Cy--; 
					move_counter++; 
					break;
				}
				else if(control_function(Cx,Cy-1,Dx,Dy,size)==1){
					Cy--;
					game_status=1; 
					drawing_room_function(Cx,Cy,Dx,Dy,size);
					move_counter++; 
					break;
				}
			case 'd':
				if(control_function(Cx+1,Cy,Dx,Dy,size)==2) 
				break;
				else if(control_function(Cx+1,Cy,Dx,Dy,size)==0){
					Cx++;
					move_counter++; 
					break;
				}
				else if(control_function(Cx+1,Cy,Dx,Dy,size)==1){
					Cx++;
					game_status=1;
					drawing_room_function(Cx,Cy,Dx,Dy,size); 
					move_counter++; 
					break;
				}
			case 'q':
				game_status=3;
				break;
			default:
				printf("Entered invalid key.\n");
				printf("Please try again or if you want to finish and quit the game please press 'q' in your keyboard.\n");
		}	
	}
	
	if(game_status==1)
	{
		printf("GAME OVER!\n");
		printf("You won!\n");
		printf("You made %d moves.\n\n",move_counter);
		win_status=1;
	}
	else if(game_status==3)
	{
		printf("GAME OVER!\n");
		printf("You made %d moves.\n\n",move_counter);
		win_status=0;
	}
	
	return win_status;
}


