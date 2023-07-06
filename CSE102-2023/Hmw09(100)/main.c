#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	char type;
	int value;
}block;

typedef struct{
	int row;
	int col;
}pos_of_snake;

block*** init_board();
void draw_board(block ***board, pos_of_snake *snake, int length);
int check_status(block ***board, pos_of_snake *snake, int length, char move);
void play(block ***board);
pos_of_snake* move(pos_of_snake *snake, int length, char move);
void update(block ***board, pos_of_snake *snake, int length);

int main(int argc, char *argv[]) {
	
	block ***board;
	board = init_board();
	play(board);
	return 0;
}

block*** init_board()
{
	block ***board = (block***) malloc(10 * sizeof(block));
	
	int i,j;
	for(i=0;i<10;i++)
	{
		board[i] = (block**) malloc(10 * sizeof(block));
		for(j=0;j<10;j++)
		{
			board[i][j] = (block*) malloc(sizeof(block));
		}
	}
	
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			board[i][j][0].type = 'e';//initialize with empty.
			board[i][j][0].value = 0;
		}
	}
	
	srand(time(0));
	int rand_row,rand_col,cntr=0;

	while(1)//for obstacles.
	{
		rand_row=rand()%10;
		rand_col=rand()%10;
		if(board[rand_row][rand_col][0].type=='e' && rand_row != 0 && rand_col != 0)
		{
			board[rand_row][rand_col][0].type = 'p';
			board[rand_row][rand_col][0].value = 1;
			cntr++;
			if(cntr==3) break;
		}
	}
	
	while(1)//for baits.
	{
		rand_row=rand()%10;
		rand_col=rand()%10;
		if(board[rand_row][rand_col][0].type=='e' && rand_row != 0 && rand_col != 0)
		{
			board[rand_row][rand_col][0].type = 'b';
			board[rand_row][rand_col][0].value = 0;
			break;
		}
	}
	
	return board;
}

void draw_board(block ***board, pos_of_snake *snake, int length)
{
	int i,j,k;
	int flag;	
	printf("  ");
	for(i=0;i<10;i++) printf("- ");
	printf("\n");
	
	for(i=0;i<10;i++)
	{
		printf("| ");
		for(j=0;j<10;j++)
		{
			flag=0;
			for(k=0;k<length;k++)//If there is snake, print firstly.
			{
				if(i == snake[k].row && j == snake[k].col)
				{
					flag=1;
					if(k==0) printf("O ");
					else printf("X ");
				}
			}
			if(flag==0)//to print obstacles and baits.
			{
				if(board[i][j][0].type == 'e') printf("  ");
				else if(board[i][j][0].type == 'o'){
					k=0;
					while(board[i][j][k].type != 'p'){
						k++;
					}
					printf("%d ",board[i][j][k].value);
				}
				else if(board[i][j][0].type == 'p') printf("%d ",board[i][j][0].value);
				else if(board[i][j][0].type == 'b') printf(". ");
			}
		}
		printf("|\n");
	}
	
	printf("  ");
	for(i=0;i<10;i++) printf("- ");
	printf("\n");
}

int check_status(block ***board, pos_of_snake *snake, int length, char move)
{
	int i,k;
	
	if(snake[0].col == 0 && move == 'a' || move == 'A') return 1;
	else if(snake[0].col == 9 && move =='d' || move == 'D') return 1;
	else if(snake[0].row == 0 && move == 'w' || move == 'W') return 1;
	else if(snake[0].row == 9 && move == 's' || move == 'S') return 1;
	
	switch(move)
	{
		case 'w':
		case 'W':
			if(board[snake[0].row-1][snake[0].col][0].type == 'o'){
				k=0;
				while(board[snake[0].row-1][snake[0].col][k].type!='p'){
					k++;
				}
				if(length<=board[snake[0].row-1][snake[0].col][k].value) return 1;
			}
			else if(board[snake[0].row-1][snake[0].col][0].type == 'p'){
				if(length<=board[snake[0].row-1][snake[0].col][0].value) return 1;
			}
			else if(length>1)
			{
				if(snake[0].row-1 == snake[1].row && snake[0].col == snake[1].col){
					printf("You can't move there!\n");
					return 2;
				}
			}
			for(i=1;i<length;i++)
			{
				if(snake[i].col == snake[0].col && snake[i].row == snake[0].row-1){
					return 1;
				}
			}
			break;
		case 'a':
		case 'A':
			if(board[snake[0].row][snake[0].col-1][0].type == 'o'){
				k=0;
				while(board[snake[0].row][snake[0].col-1][k].type!='p'){
					k++;
				}
				if(length<=board[snake[0].row][snake[0].col-1][k].value) return 1;
			}
			else if(board[snake[0].row][snake[0].col-1][0].type == 'p'){
				if(length<=board[snake[0].row][snake[0].col-1][0].value) return 1;
			}
			else if(length>1)
			{
				if(snake[0].row == snake[1].row && snake[0].col-1 == snake[1].col){
					printf("You can't move there!\n");
					return 2;
				}
			}
			for(i=1;i<length;i++)
			{
				if(snake[i].col == snake[0].col-1 && snake[i].row == snake[0].row){
					printf("here");
					return 1;
				}
			}
			break;
		case 's':
		case 'S':
			if(board[snake[0].row+1][snake[0].col][0].type == 'o'){
				k=0;
				while(board[snake[0].row+1][snake[0].col][k].type!='p'){
					k++;
				}
				if(length<=board[snake[0].row+1][snake[0].col][k].value) return 1;
			}
			else if(board[snake[0].row+1][snake[0].col][0].type == 'p'){
				if(length<=board[snake[0].row+1][snake[0].col][0].value) return 1;
			}
			else if(length>1)
			{
				if(snake[0].row+1 == snake[1].row && snake[0].col == snake[1].col){
					printf("You can't move there!\n");
					return 2;
				}
			}
			for(i=1;i<length;i++)
			{
				if(snake[i].col == snake[0].col && snake[i].row == snake[0].row+1){
					printf("here");
					return 1;
				}
			}
			break;
		case 'd':
		case 'D':
			if(board[snake[0].row][snake[0].col+1][0].type == 'o'){
				k=0;
				while(board[snake[0].row][snake[0].col+1][k].type!='p'){
					k++;
				}
				if(length<=board[snake[0].row][snake[0].col+1][k].value) return 1;
			}
			else if(board[snake[0].row][snake[0].col+1][0].type == 'p'){
				if(length<=board[snake[0].row][snake[0].col+1][0].value) return 1;
			}
			else if(length>1)
			{
				if(snake[0].row == snake[1].row && snake[0].col+1 == snake[1].col){
					printf("You can't move there!\n");
					return 2;
				}
			}
			for(i=1;i<length;i++)
			{
				if(snake[i].col == snake[0].col+1 && snake[i].row == snake[0].row){
				printf("here");
				return 1;
				}
			}
			break;
	}
	
	return 0;
}

void play(block ***board)
{
	pos_of_snake *snake;
	pos_of_snake *prev;
	snake = (pos_of_snake*) malloc(sizeof(pos_of_snake));
	snake[0].col=0;
	snake[0].row=0;
	int length = 1;
	
	int flag;
	int rand_row,rand_col;
	
	int i;
	int cntr = 0;
	
	char user_move;
	int game_status=0;
	while(game_status != 1)
	{
		user_move = 'n';
		draw_board(board,snake,length);
		while(user_move != 'w' && user_move != 'a' && user_move != 's' && user_move != 'd' && user_move != 'W' && user_move != 'A' && user_move != 'S' && user_move != 'D')
		{
			printf("Enter your move (w,a,s,d): ");
			scanf(" %c",&user_move);
			if(user_move != 'w' && user_move != 'a' && user_move != 's' && user_move != 'd' && user_move != 'W' && user_move != 'A' && user_move != 'S' && user_move != 'D') 
				printf("Invalid typing!\nTry again!\n");
		}
		game_status = check_status(board,snake,length,user_move);
		if(game_status != 1)
		{
			prev = move(snake,length,user_move);
			if(board[snake[0].row][snake[0].col][0].type == 'b')//if head of snake encounter the bait. 
			{
				(length)++;
				snake = (pos_of_snake*) realloc(snake,(length)*sizeof(pos_of_snake));
				snake[(length)-1].col = prev->col;
				snake[(length)-1].row = prev->row;
				board[snake[0].row][snake[0].col][0].type = 'e';
				
				flag=0;
				while(flag==0)//to find empty space.
				{
					rand_row = rand()%10;
					rand_col = rand()%10;
					flag=1;
					for(i=0;i<length;i++)
					{
						if(rand_col == snake[i].col && rand_row == snake[i].row)
						{
							flag=0;
							break;
						}
					}
					if(board[rand_row][rand_col][0].type != 'e') flag=0;
				}
				board[rand_row][rand_col][0].type = 'b';
			}
			else if(board[snake[0].row][snake[0].col][0].type == 'o' || board[snake[0].row][snake[0].col][0].type == 'p')//if head of snake encounter the obstacles.
			{
				board[snake[0].row][snake[0].col] = (block*) realloc(board[snake[0].row][snake[0].col],sizeof(block));
				board[snake[0].row][snake[0].col][0].type = 'e';
				board[snake[0].row][snake[0].col][0].value = 0;
				
				flag=0;
				while(flag==0)//to find empty space.
				{
					rand_row = rand()%10;
					rand_col = rand()%10;
					flag=1;
					for(i=0;i<length;i++)
					{
						if(rand_col == snake[i].col && rand_row == snake[i].row)
						{
							flag=0;
							break;
						}
					}
					if(board[rand_row][rand_col][0].type != 'e') flag=0;
				}
				board[rand_row][rand_col][0].type = 'p';
				board[rand_row][rand_col][0].value = 1;
			}
		}
		if(game_status!=2) cntr++;
		if(cntr%5 == 0 && cntr != 0) update(board, snake, length);
	}
	printf("Game Over\n");
	printf("Length of your snake: %d\n",length);
}

pos_of_snake* move(pos_of_snake *snake, int length, char move)
{
	pos_of_snake temp;
	pos_of_snake *prev;
	prev = (pos_of_snake*) malloc(sizeof(pos_of_snake));
	int i,k;
	int rand_row,rand_col;
	int flag;
	
	switch(move)
	{
		case 'w':
		case 'W':
			if(length>1)
			{
				if(snake[0].row-1 == snake[1].row && snake[0].col == snake[1].col){
					printf("Try again!\n");
					break;
				}
			}
			prev->col=snake[0].col;
			prev->row=snake[0].row;
			snake[0].row=snake[0].row-1;
			for(i=1;i<(length);i++)
			{
				temp.col = snake[i].col;
				temp.row = snake[i].row;
				snake[i].col = prev->col;
				snake[i].row = prev->row;
				prev->col = temp.col;
				prev->row = temp.row;
			}
			break;
		case 'a':
		case 'A':
			if(length>1)
			{
				if(snake[0].row == snake[1].row && snake[0].col-1 == snake[1].col){
					printf("Try again!\n");
					break;
				}
			}
			prev->col=snake[0].col;
			prev->row=snake[0].row;
			snake[0].col=snake[0].col-1;
			for(i=1;i<(length);i++)
			{
				temp.col = snake[i].col;
				temp.row = snake[i].row;
				snake[i].col = prev->col;
				snake[i].row = prev->row;
				prev->col = temp.col;
				prev->row = temp.row;
			}
			
			break;
		case 's':
		case 'S':
			if(length>1)
			{
				if(snake[0].row+1 == snake[1].row && snake[0].col == snake[1].col){
					printf("Try again!\n");
					break;
				}
			}
			prev->col=snake[0].col;
			prev->row=snake[0].row;
			snake[0].row=snake[0].row+1;
			for(i=1;i<(length);i++)
			{
				temp.col = snake[i].col;
				temp.row = snake[i].row;
				snake[i].col = prev->col;
				snake[i].row = prev->row;
				prev->col = temp.col;
				prev->row = temp.row;
			}
			
			break;
		case 'd':
		case 'D':
			if(length>1)
			{
				if(snake[0].row == snake[1].row && snake[0].col+1 == snake[1].col){
					printf("Try again!\n");
					break;
				}
			}
			prev->col=snake[0].col;
			prev->row=snake[0].row;
			snake[0].col=snake[0].col+1;
			for(i=1;i<(length);i++)
			{
				temp.col = snake[i].col;
				temp.row = snake[i].row;
				snake[i].col = prev->col;
				snake[i].row = prev->row;
				prev->col = temp.col;
				prev->row = temp.row;
			}
			break;
	}
	
	return prev;
}

void update(block ***board, pos_of_snake *snake, int length)
{
	int flag=0;
	int rand_col,rand_row;
	int i,height;
	while(flag==0)//to find obstacles.
	{
		rand_row = rand()%10;
		rand_col = rand()%10;
		flag=1;
		for(i=0;i<length;i++)
		{
			if(rand_col == snake[i].col && rand_row == snake[i].row)
			{
				flag=0;
				break;
			}
		}
		if(board[rand_row][rand_col][0].type == 'b' || board[rand_row][rand_col][0].type == 'e') flag=0;
	}
	for(i=0;board[rand_row][rand_col][i].type == 'o';i++);
	height = i+1;
	
	if(height != 9)//to increase height of the obstacle.
	{
		board[rand_row][rand_col] = (block*) realloc(board[rand_row][rand_col],(height+1)*sizeof(block));
		for(i=0;i<height;i++)
		{
			board[rand_row][rand_col][i].type = 'o';
			board[rand_row][rand_col][i].value = i+1;
		}
		board[rand_row][rand_col][i].type = 'p';
		board[rand_row][rand_col][i].value = i+1;
	}
}
