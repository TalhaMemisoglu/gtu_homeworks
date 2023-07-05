#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int game_arr[9];
}puzzle;

void generate_board(puzzle *board);
int movement(puzzle *board, int num, char move);
void print_function(puzzle *board);
void user_gameplay();
char auto_finish(int position_of_0);
void pc_gameplay();
void best_score();

int main(int argc, char *argv[]) {
	
	int choice=0;
	while(choice!=4)
	{
		printf("Welcome to the 8-Puzzle Game!\n");
		printf("Please select an option:\n");
		printf("1. Play game as a user\n");
		printf("2. Finish the game with PC\n");
		printf("3. Show the best score\n");
		printf("4. Exit\n");
		printf("> ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				user_gameplay();
				break;
			case 2:
				pc_gameplay();
				break;
			case 3:
				best_score();
				break;
			case 4:
				printf("Program Terminated.");
				break;
			default:
				printf("Invalid typing.\n");
				printf("Please try again!\n");
		}
	}
	return 0;
}

void generate_board(puzzle *board)
{
	int i,j;
	for(i=0;i<8;i++)//to generate initial board.
	{
		board->game_arr[i]=i+1;
	}
	board->game_arr[8]=0;
	
	int rand_num,solvable_status=0,inversion_cntr;
	int position_of_0=8;
	srand(time(0));
	while(solvable_status==0)
	{
		for(i=0;i<20;i++)//to mix the board.
		{
			rand_num=rand()%9;
			board->game_arr[position_of_0]=board->game_arr[rand_num];
			board->game_arr[rand_num]=0;
			position_of_0=rand_num;
		}
		
		inversion_cntr=0;
		for(i=0;i<9;i++)//to check the board whether it is solvable.
		{
			for(j=i+1;j<9;j++)
			{
				if(board->game_arr[i]>board->game_arr[j] && board->game_arr[j]!=0) inversion_cntr++;
			}
		}
		if(inversion_cntr%2==0) solvable_status=1;//if inversion is odd,board cannot be solve.
	}
}

int movement(puzzle *board, int num, char move)
{	
	int i,position_of_num;
	for(i=0;i<9;i++)//to find given num in the board.
	{
		if(board->game_arr[i]==num){
			position_of_num=i;
			break;
		}
	}
	int position_of_0;
	for(i=0;i<9;i++)//to find 0(gap) in the board.
	{
		if(board->game_arr[i]==0){
			position_of_0=i;
			break;
		}
	}
	
	switch(move)
	{
		case 'R':
		case 'r':
			if(position_of_num!=2 && position_of_num!=5 && position_of_num!=8)//to check border states.
			{
				if(position_of_num+1==position_of_0){//check if the destination is free.
					board->game_arr[position_of_0]=board->game_arr[position_of_num];
					board->game_arr[position_of_num]=0;
				}
				else{
					printf("%d-%c is an illegal move! Make a legal move!\n",num,move);	
				}
			}
			else{
				printf("%d-%c is an illegal move! Make a legal move!\n",num,move);
			}
			break;
		case 'L':
		case 'l':
			if(position_of_num!=0 && position_of_num!=3 && position_of_num!=6)//to check border states.
			{
				if(position_of_num-1==position_of_0){//check if the destination is free.
					board->game_arr[position_of_0]=board->game_arr[position_of_num];
					board->game_arr[position_of_num]=0;
				}
				else{
					printf("%d-%c is an illegal move! Make a legal move!\n",num,move);	
				}
			}
			else{
				printf("%d-%c is an illegal move! Make a legal move!\n",num,move);
			}
			break;
		case 'U':
		case 'u':
			if(position_of_num!=0 && position_of_num!=1 && position_of_num!=2)//to check border states.
			{
				if(position_of_num-3==position_of_0){//check if the destination is free.
					board->game_arr[position_of_0]=board->game_arr[position_of_num];
					board->game_arr[position_of_num]=0;
				}
				else{
					printf("%d-%c is an illegal move! Make a legal move!\n",num,move);	
				}
			}
			else{
				printf("%d-%c is an illegal move! Make a legal move!\n",num,move);
			}
			break;
		case 'D':
		case 'd':
			if(position_of_num!=6 && position_of_num!=7 && position_of_num!=8)//to check border states.
			{
				if(position_of_num+3==position_of_0){//check if the destination is free.
					board->game_arr[position_of_0]=board->game_arr[position_of_num];
					board->game_arr[position_of_num]=0;
				}
				else{
					printf("%d-%c is an illegal move! Make a legal move!\n",num,move);	
				}
			}
			else{
				printf("%d-%c is an illegal move! Make a legal move!\n",num,move);
			}
			break;		
	}
	
	int flag=1;
	for(i=0;i<8;i++)//to check whether game is won.
	{
		if(board->game_arr[i]!=i+1){
			flag = 0;
			break;
		}	
	}
	
	return flag;
}

void print_function(puzzle *board)
{
	FILE *fptr;
	fptr = fopen("gameplay.txt","a");
	int i;
	for(i=0;i<9;i++)//to print to gameplay.txt file.
	{
		if(board->game_arr[i]==0) fprintf(fptr,"_ "); else fprintf(fptr,"%d ",board->game_arr[i]);
		if(i==2 || i==5 || i==8) fprintf(fptr,"\n");
	}
	fprintf(fptr,"\n");
	
	for(i=0;i<9;i++)//to print to console.
	{
		if(board->game_arr[i]==0) printf("_ "); else printf("%d ",board->game_arr[i]);
		if(i==2 || i==5 || i==8) printf("\n");
	}
	
	fclose(fptr);
}

void user_gameplay()
{
	FILE *fptr;
	fptr = fopen("gameplay.txt","w");
	fclose(fptr);
	
	puzzle board;
	generate_board(&board);
	
	print_function(&board);
	int game_status=0,movement_cntr=0;
	int num;
	char move;
	while(game_status!=1)//if game status equal to 1,the game is finished.
	{
		while(1)
		{
			printf("Enter your move (number-direction, e.g., 2-R): ");
			scanf("%d-%c",&num,&move);
			if(num<9 && num>0) break; else printf("Invalid number.\nPlease try again.\n");
			if(move=='L' || move=='l' || move=='R' || move=='r' || move=='U' || move=='u' || move=='D' || move=='d') break; 
			else printf("Invalid movement!\nPlease try again.\n");
		}	
		game_status=movement(&board,num,move);
		print_function(&board);
		movement_cntr++;
	}
	printf("Congratulations! You finished the game.\n\n");
	printf("Total number of moves: %d",movement_cntr);
	
	int user_score=1000-10*movement_cntr;
	printf("Your Score: %d",user_score);//to declare user score.
	
	int best_score;
	fptr = fopen("best_score.txt","r");
	fscanf(fptr,"%d",&best_score);
	fclose(fptr);
	
	if(user_score>best_score)
	{
		fptr = fopen("best_score.txt","w");
		fprintf(fptr,"%d",user_score);
		fclose(fptr);	
	}
}

char auto_finish(int position_of_0)//recursive function for generating valid move.
{
	int rand_num=rand()%4;
	switch(position_of_0)//for every place. 
	{
		case 0:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return auto_finish(position_of_0);
			}
			else if(rand_num==2){
				return auto_finish(position_of_0);
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 1:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return auto_finish(position_of_0);
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 2:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return auto_finish(position_of_0);
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return auto_finish(position_of_0);
			}
			break;
		case 3:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return auto_finish(position_of_0);
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 4:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 5:
			if(rand_num==0){
				return 'D';
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return auto_finish(position_of_0);
			}
			break;
		case 6:
			if(rand_num==0){
				return auto_finish(position_of_0);
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return auto_finish(position_of_0);
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 7:
			if(rand_num==0){
				return auto_finish(position_of_0);
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return 'R';
			}
			break;
		case 8:
			if(rand_num==0){
				return auto_finish(position_of_0);
			}
			else if(rand_num==1){
				return 'U';
			}
			else if(rand_num==2){
				return 'L';
			}
			else if(rand_num==3){
				return auto_finish(position_of_0);
			}
			break;
	}
}

void pc_gameplay()
{
	FILE *fptr;
	fptr = fopen("gameplay.txt","w");
	
	puzzle board;
	generate_board(&board);
	int i;
	for(i=0;i<9;i++)//to print to console.
	{
		if(board.game_arr[i]==0) printf("_ "); else printf("%d ",board.game_arr[i]);
		if(i==2 || i==5 || i==8) printf("\n");
	}
	printf("\n");
	for(i=0;i<9;i++)//to print to the file.
	{
		if(board.game_arr[i]==0) fprintf(fptr,"_ "); else fprintf(fptr,"%d ",board.game_arr[i]);
		if(i==2 || i==5 || i==8) fprintf(fptr,"\n");
	}
	
	int position_of_0;
	for(i=0;i<9;i++)//to determine where 0 is.
	{
		if(board.game_arr[i]==0)
		{
			position_of_0=i;
			break;
		}
	}
	
	srand(time(0));
	char movement_of_zero;
	int num,movement_cntr=0;
	int game_status;
	while(movement_cntr<90000000)
	{
		movement_of_zero=auto_finish(position_of_0);
		switch(movement_of_zero)//to move.
		{
			case 'R':
				position_of_0 +=1;
				num=board.game_arr[position_of_0];
				game_status=movement(&board,num,'L');
				printf("Computer Move: %d-L\n",num);
				break;
			case 'L':
				position_of_0 -=1;
				num=board.game_arr[position_of_0];
				game_status=movement(&board,num,'R');
				printf("Computer Move: %d-R\n",num);
				break;
			case 'U':
				position_of_0 -=3;
				num=board.game_arr[position_of_0];
				game_status=movement(&board,num,'D');
				printf("Computer Move: %d-D\n",num);
				break;
			case 'D':
				position_of_0 +=3;
				num=board.game_arr[position_of_0];
				game_status=movement(&board,num,'U');
				printf("Computer Move: %d-U\n",num);
				break;
		}
		movement_cntr++;
		
		for(i=0;i<9;i++)//to print to console.
		{
			if(board.game_arr[i]==0) printf("_ "); else printf("%d ",board.game_arr[i]);
			if(i==2 || i==5 || i==8) printf("\n");
		}
		printf("\n");
		for(i=0;i<9;i++)//to print to the file.
		{
			if(board.game_arr[i]==0) fprintf(fptr,"_ "); else fprintf(fptr,"%d ",board.game_arr[i]);
			if(i==2 || i==5 || i==8) fprintf(fptr,"\n");
		}
		fprintf(fptr,"\n");
		
		if(game_status==1){//if the game is finished.
			printf("Computer finished the game.\n");
			printf("Total number of computer moves: %d\n",movement_cntr);
			break;
		}
	}
	
	fclose(fptr);
}

void best_score()
{
	FILE *fptr;
	fptr=fopen("best_score.txt","r");
	int best_score;
	
	fscanf(fptr,"%d",&best_score);
	printf("The best score is %d.\n",best_score);
	
	fclose(fptr);
}
