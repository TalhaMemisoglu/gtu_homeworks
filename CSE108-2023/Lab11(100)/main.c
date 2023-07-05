#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Order{
	int orderID;
	char customerName[20];
	char items[50];
	char orderTime[20];
	struct Order *next;
}Order;

typedef struct ExamPaper{
	char studentName[20];
	int StudentNumber;
	int Score;
	struct ExamPaper *next;
}ExamPaper;


Order *list_enqueue(Order  *queue,  char  *customerName,  int  orderID, char  *items,  char  *orderTime)
{
	Order *np;
	np = (Order*) malloc(sizeof(Order));
	np->next = queue;
	
	strcpy(np->customerName,customerName);
	strcpy(np->items,items);
	np->orderID = orderID;
	strcpy(np->orderTime,orderTime);
	
	return np;
}

Order *dequeue(Order * queue)
{
	Order *cp;
	cp=queue;
	
	if(cp->next==NULL)
	{
		queue=NULL;
		free(cp);
	}
	else
	{
		while(cp->next->next!=NULL) cp = cp->next;
		cp->next=NULL;
		free(cp->next);
	}
	
	return queue;
}

void display(Order *queue)
{
	while(queue != NULL)
	{
		printf("-Order ID:%d, Customer Name: %s, Items: %s\n",queue->orderID,queue->customerName,queue->items);
		queue=queue->next;
	}
	printf("\n");
}

void updateOrder(Order *queue, int orderID, char *newItems)
{
	Order *cp;
	cp = queue;
	
	while(cp != NULL)
	{
		if(cp->orderID == orderID)
		{
			strcpy(cp->items,newItems);
		}
		cp = cp->next;
	}
}

int isEmpty(ExamPaper * stack)
{
	int empty_status=1;
	
	if(stack == NULL)
	{
		empty_status = 0;
	}
	
	return empty_status;
}

ExamPaper *push(ExamPaper *stack,  char  *studentName, int studentNumber, int score)
{
	ExamPaper *np;
	np = (ExamPaper*) malloc(sizeof(ExamPaper));
	
	if(stack == NULL)
	{
		np->next = stack;
		strcpy(np->studentName,studentName);
		np->StudentNumber = studentNumber;
		np->Score = score;
		stack = np;
	}
	else
	{
		while(stack->next != NULL)
		{
			stack = stack->next;
		}
		strcpy(np->studentName,studentName);
		np->StudentNumber = studentNumber;
		np->Score = score;
		np->next = NULL;
		stack->next = np;
	}
	
	return stack;
}

ExamPaper *pop(ExamPaper * stack)
{
	ExamPaper *cp;
	cp=stack;
	
	if(cp->next==NULL)
	{
		stack=NULL;
		free(cp);
	}
	else
	{
		while(cp->next->next!=NULL) cp = cp->next;
		cp->next=NULL;
		free(cp->next);
	}
	
	return stack;
}

void display_exam(ExamPaper *stack)
{
	while(stack != NULL)
	{
		printf("-Student Name:%s, Student Number: %d, Score: %d\n",stack->studentName,stack->StudentNumber,stack->Score);
		stack=stack->next;
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	
	Order *queue;
	queue = NULL;
	
	int OrderID=101;
	char customer_name[] = "Talha";
	char items[30]= "Burger, Fries, Coke";
	char order_time[10] = "12.15";
	
	queue = list_enqueue(queue,customer_name,OrderID,items,order_time);
	display(queue);
	
	OrderID = 102;
	strcpy(customer_name,"Emre");
	strcpy(items,"Pizza, Fries,Coke");
	strcpy(order_time,"12.17");
	
	queue = list_enqueue(queue,customer_name,OrderID,items,order_time);
	display(queue);
	
	OrderID = 102;
	strcpy(items,"Burger, Fries,Coke");
	updateOrder(queue,OrderID,items);
	display(queue);
	
	queue = dequeue(queue);
	display(queue);
	
	/****************************************************************/
	
	ExamPaper *stack;
	stack = NULL;
	
	char studentName[] = "Talha"; 
	int studentNumber = 210;
	int score = 100;
	
	stack = push(stack,studentName,studentNumber,score);
	display_exam(stack);
	
	strcpy(studentName,"Emre");
	studentNumber = 212;
	score = 95;
	
	stack = push(stack,studentName,studentNumber,score);
	display_exam(stack);
	
	if(isEmpty(stack) != 0) printf("Stack is not empty\n\n");
	
	stack = pop(stack);
	display_exam(stack);
	return 0;
}
