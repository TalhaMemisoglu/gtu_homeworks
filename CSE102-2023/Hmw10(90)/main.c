#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BORROW 3

typedef struct ll_book{
	char *ISBN;
	char *title;
	char *author;
	int publication_year;
	char *reserved_status;
	struct ll_book *next;
}ll_book;

typedef struct ll_student{
	char *name;
	char *ID;
	char *borrowedBooks[MAX_BORROW];
	struct ll_student *next;
}ll_student;

ll_book *insert_at_begin(ll_book *books, char *ISBN, char *title, char *author, int publication_year, char *reserved_status);
ll_book *insert_at_end(ll_book *books, char *ISBN, char *title, char *author, int publication_year, char *reserved_status);
ll_student *initializeStudentList(int method);
ll_student *insert_at_begin_std(ll_student *student, char *ID, char *name, char **borrowedBooks);
ll_student *insert_at_end_std(ll_student *student, char *ID, char *name, char **borrowedBooks);
void print_list(ll_book *books);
void print_list_std(ll_student *student);
ll_book *initializeBookList(int method);
ll_book *addBook(ll_book *books, char *ISBN, char *title, char *author, int publication_year, int method);
ll_book *deleteBook(ll_book *books, char *ISBN);
ll_book *updateBook(ll_book *books, char *ISBN);
ll_book *selectionSort(ll_book *books, int sortChoice);
void filterAndSortBooks(ll_book *books, int filterChoice, int sortChoice, int method);
ll_book *reverseBookList(ll_book *books);
void searchBooks(ll_book *books, int searchChoice, char* searchValue);
void StudentsBorrowedBooksList(ll_book *books, ll_student *student, char *ID);
ll_student *borrow_std(ll_student *student, char *ISBN, char *ID, int *control);
ll_book *borrow_bk(ll_book *books, char *ISBN, int *control);
ll_student *return_std(ll_student *student, char *ISBN, char *ID, int *control);
ll_book *return_bk(ll_book *books, char *ISBN, int *control);
void save_changes(ll_book *books);
void save_changes_std(ll_student *student);

int main(int argc, char *argv[]) {
	
	int method;
	printf("Please choose the method that you want to store books (0:FIFO 1:LIFO): ");
	scanf("%d",&method);
	
	ll_book *books;
	ll_student *student;
	printf("Initializing...\n");
	books = initializeBookList(method);
	student = initializeStudentList(method);
	printf("Initialized.\n\n");
	
	printf("****** Books List ******\n");
	print_list(books);
	printf("****** Students List ******\n");
	print_list_std(student);
	
	int menu=0;
	char *string,*string2,*ISBN,*title,*author;
	string = (char*) malloc(50*sizeof(char));//to take input from user.
	string2 = (char*) malloc(50*sizeof(char));//to take input from user.
	int publicationYear;
	int control;
	
	int filterChoice,sortChoice,searchChoice;
	while(menu != 11)
	{
		printf("************ MENU ************\n");
		printf("1-Add Book\n");
		printf("2-Delete Book\n");
		printf("3-Update Book Information\n");
		printf("4-Filter And Sort Book\n");
		printf("5-Reverse The Book List\n");
		printf("6-Search Books\n");
		printf("7-Display student's borrowed books\n");
		printf("8-Borrow book\n");
		printf("9-Return book\n");
		printf("10-Display lists\n");
		printf("11-Exit\n");
		printf("Type the menu option you would like to choose: ");
		scanf("%d",&menu);
		getchar();
		switch(menu)
		{
			case 1:
				printf("Enter the ISBN of the book: ");
				gets(string);
				ISBN = (char*) malloc(sizeof(char)*strlen(string));
				strcpy(ISBN,string);
				
				printf("Enter the title of the book: ");
				gets(string);
				title = (char*) malloc(sizeof(char)*strlen(string));
				strcpy(title,string);
				
				printf("Enter the author of the book: ");
				gets(string);
				author = (char*) malloc(sizeof(char)*strlen(string));
				strcpy(author,string);
				
				printf("Enter the publication year of the book: ");
				scanf("%d",&publicationYear);
				books = addBook(books,ISBN,title,author,publicationYear,method);
				break;
			case 2:
				printf("Enter the ISBN of the book that you want to delete: ");
				gets(string);
				books = deleteBook(books,string);
				break;
			case 3:
				printf("Enter the ISBN of the book that you want to update: ");
				gets(string);
				books = updateBook(books,string);
				break;
			case 4:
				printf("1-Filter by author\n");
				printf("2-Filter by publication year\n");
				printf("Type the menu option you would like to choose: ");
				scanf("%d",&filterChoice);
				printf("1-Sort by title\n");
				printf("2-Sort by author\n");
				printf("3-Sort by publication year\n");
				printf("Type the menu option you would like to choose: ");
				scanf("%d",&sortChoice);
				filterAndSortBooks(books,filterChoice,sortChoice,method);
				break;
			case 5:
				books = reverseBookList(books);
				print_list(books);
				break;
			case 6:
				printf("1-Search by ISBN number\n");
				printf("2-Search by author\n");
				printf("3-Search by title\n");
				printf("Type the menu option you would like to choose: ");
				scanf("%d",&searchChoice);
				getchar();
				printf("Enter the filter: ");
				gets(string);
				searchBooks(books,searchChoice,string);
				break;
			case 7:
				printf("Enter the student's ID: ");
				gets(string);
				StudentsBorrowedBooksList(books,student,string);
				break;
			case 8:
				printf("Enther the ISBN of the book: ");
				gets(string);
				printf("Enter the student's ID: ");
				gets(string2);
				control = 1;//To verify whether everything is functioning correctly
				books = borrow_bk(books,string,&control);
				if(control == 1)//If book is not found, don't continue.
				{
					student = borrow_std(student,string,string2,&control);
					if(control == 0)//if student is not found, take back changes in the linked list of books.
					{
						books = return_bk(books,string,&control);
					}
				}
				break;
			case 9:
				printf("Enther the ISBN of the book: ");
				gets(string);
				printf("Enter the student's ID: ");
				gets(string2);
				control = 1;//To verify whether everything is functioning correctly.
				books = return_bk(books,string,&control);
				if(control == 1)//If book is not found, don't continue.
				{
					student = return_std(student,string,string2,&control);
					if(control == 0)//if student is not found, take back changes in the linked list of books.
					{
						books = borrow_bk(books,string,&control);
					}
				}
				break;
			case 10:
				printf("****** Books list ******\n");
				print_list(books);
				printf("****** Student list ******\n");
				print_list_std(student);
				break;
			case 11:
				break;
			default:
				printf("Invalid typing!\n");
				printf("Please try again.\n\n");
		}
	}
	
	char choose = 'n';
	printf("Do you want to save changes?(y/n): ");
	scanf(" %c",&choose);
	if(choose == 'y' || choose == 'Y')
	{
		printf("Saving...\n");
		save_changes(books);
		save_changes_std(student);
		printf("Saved.\n");
	}
	free(books);
	return 0;
}

ll_book *insert_at_begin(ll_book *books, char *ISBN, char *title, char *author, int publication_year, char *reserved_status)
{
	ll_book *np;
	np = (ll_book*) malloc(sizeof(ll_book));
	
	//data copying.
	np->next = books;
	np->ISBN = ISBN;
	np->title = title;
	np->author = author;
	np->publication_year = publication_year;
	np->reserved_status = reserved_status;
	
	return np;
}

ll_book *insert_at_end(ll_book *books, char *ISBN, char *title, char *author, int publication_year, char *reserved_status)
{
	ll_book *np,*cp;
	np = (ll_book*) malloc(sizeof(ll_book));
	
	//data copying
	np->ISBN = ISBN;
	np->title = title;
	np->author = author;
	np->publication_year = publication_year;
	np->reserved_status = reserved_status;
	np->next = NULL;
	
	if(books == NULL)
	{
		books = np;
	}
	else
	{
		cp = books;
		while(cp->next != NULL)
		{
			cp = cp->next;
		}
		cp->next = np;
	}
	
	return books;
}

ll_student *insert_at_begin_std(ll_student *student, char *ID, char *name, char **borrowedBooks)
{
	ll_student *np;
	np = (ll_student*) malloc(sizeof(ll_student));
	
	//data copying
	np->next = student;
	np->ID = ID;
	np->name = name;
	int i;
	for(i=0;i<MAX_BORROW;i++)//to take all information of borrowed books according to max numbers of books that can be borrowed.
	{
		np->borrowedBooks[i] = borrowedBooks[i];
	}
	
	return np;
}

ll_student *insert_at_end_std(ll_student *student, char *ID, char *name, char **borrowedBooks)
{
	ll_student *np,*cp;
	np = (ll_student*) malloc(sizeof(ll_student));
	
	//data copying
	np->ID = ID;
	np->name = name;
	int i;
	for(i=0;i<MAX_BORROW;i++)//to take all information of borrowed books according to max numbers of books that can be borrowed.
	{
		np->borrowedBooks[i] = borrowedBooks[i];
	}
	np->next = NULL;
	
	if(student == NULL)
	{
		student = np;
	}
	else
	{
		cp = student;
		while(cp->next != NULL)
		{
			cp = cp->next;
		}
		cp->next = np;
	}
	
	return student;
}

void print_list(ll_book *books)
{
	while(books != NULL)
	{
		printf("ISBN: %s\n",books->ISBN);
		printf("Title: %s\n",books->title);
		printf("Author: %s\n",books->author);
		printf("Publication Year: %d\n",books->publication_year);
		printf("Reserved Status: %s\n\n",books->reserved_status);
		books = books->next;
	}
}

void print_list_std(ll_student *student)
{
	int i;
	while(student != NULL)
	{
		printf("ID:%s\n",student->ID);
		printf("Name:%s\n",student->name);
		for(i=0; i<MAX_BORROW; i++)
		{
			if(strlen(student->borrowedBooks[i])>1)//print all ISBN of books that was borrowed by this student.
			{
				printf("%d. Borrowed Book: %s\n", i+1, student->borrowedBooks[i]);
			}
		}
		printf("\n");
		student = student->next;
	}
}

ll_book *initializeBookList(int method)//to take information from the file and store in linked list.
{
	ll_book *books;
	books = NULL;
	
	FILE *fptr = fopen("BookList.txt","r");
	
	char *string;
	string = (char*) malloc(50*sizeof(char));
	int publicationYear;
	
	char *ISBN,*title,*author,*reserved_status;
	
	if(method == 0)//FIFO
	{
		while(fgets(string,50,fptr) != NULL)
		{
			ISBN = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(ISBN,string);
			
			fgets(string,50,fptr);
			title = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(title,string);
			
			fgets(string,50,fptr);
			author = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(author,string);
			
			fscanf(fptr,"%d\n",&publicationYear);
			
			fgets(string,50,fptr);
			reserved_status = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(reserved_status,string);
			
			books = insert_at_end(books,ISBN,title,author,publicationYear,reserved_status);
		}
	}
	else//LIFO
	{
		while(fgets(string,50,fptr) != NULL)
		{
			ISBN = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(ISBN,string);
			
			fgets(string,50,fptr);
			title = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(title,string);
			
			fgets(string,50,fptr);
			author = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(author,string);
			
			fscanf(fptr,"%d\n",&publicationYear);
			
			fgets(string,50,fptr);
			reserved_status = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(reserved_status,string);
			
			books = insert_at_begin(books,ISBN,title,author,publicationYear,reserved_status);
		}
	}
	
	fclose(fptr);
	return books;
}

ll_student *initializeStudentList(int method)//to take information from the file and store in linked list.
{
	ll_student *student;
	student = NULL;
	
	FILE *fptr = fopen("StudentList.txt","r");
	
	char *string;
	string = (char*) malloc(50*sizeof(char));
	
	char *borrowedBooks[MAX_BORROW],*ID,*name;
	int i;
	
	if(method == 0)//FIFO
	{
		while(fgets(string,50,fptr) != NULL)
		{
			ID = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(ID,string);
			
			fgets(string,50,fptr);
			name = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(name,string);
			
			for(i=0;i<MAX_BORROW;i++)
			{
				fgets(string,50,fptr);
				borrowedBooks[i] = (char*) malloc(strlen(string)*sizeof(char));
				string[strlen(string)-1] = '\0';
				strcpy(borrowedBooks[i],string);
			}
			
			student = insert_at_end_std(student,ID,name,borrowedBooks);
		}
	}
	else//LIFO
	{
		while(fgets(string,50,fptr) != NULL)
		{
			ID = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(ID,string);
			
			fgets(string,50,fptr);
			name = (char*) malloc(strlen(string)*sizeof(char));
			string[strlen(string)-1] = '\0';
			strcpy(name,string);
			
			for(i=0;i<MAX_BORROW;i++)
			{
				fgets(string,50,fptr);
				borrowedBooks[i] = (char*) malloc(strlen(string)*sizeof(char));
				string[strlen(string)-1] = '\0';
				strcpy(borrowedBooks[i],string);
			}
			
			student = insert_at_begin_std(student,ID,name,borrowedBooks);
		}
	}
	
	fclose(fptr);
	return student;	
}

ll_book *addBook(ll_book *books, char *ISBN, char *title, char *author, int publication_year, int method)
{
	char *reserved_status;
	reserved_status = (char*) malloc(sizeof(char)*12);
	strcpy(reserved_status,"Not Borrowed");
	
	if(method == 0)//FIFO
	{
		books = insert_at_end(books,ISBN,title,author,publication_year,reserved_status);
	}
	else//LIFO
	{
		books = insert_at_begin(books,ISBN,title,author,publication_year,reserved_status);
	}
	
	return books;
}

ll_book *deleteBook(ll_book *books, char *ISBN)
{
	ll_book *cp,*pp;
	cp = books;
	pp = books;
	
	while(cp != NULL)
	{
		if(strcmp(cp->ISBN,ISBN)==0)
		{
			break;
		}
		pp = cp;
		cp = cp->next;
	}
	if(cp != NULL)
	{
		if(cp != pp)
		{
			pp->next = cp->next;
		}
		else
		{
			books = cp->next;
		}
		
		//Firstly, free memory of those that inside of the struct,then free memory of struct. 
		free(cp->ISBN);
		free(cp->title);
		free(cp->author);
		free(cp->reserved_status);
		free(cp);
	}
	else printf("Not Found.\n\n");
	
	return books;
}

ll_book *updateBook(ll_book *books, char *ISBN)
{
	ll_book *cp;
	cp = books;
	int choose,n_publication;
	char *string;
	string = (char*) malloc(50*sizeof(string));
	
	while(cp != NULL)
	{
		if(strcmp(cp->ISBN,ISBN)==0)
		{
			printf("The book that you want to update was found.\n");
			printf("1-Title\n");
			printf("2-Author\n"),
			printf("3-Publication Year\n");
			printf("Type the menu option you would like to choose: ");
			scanf("%d",&choose);
			getchar();
			switch(choose)
			{
				case 1:
					printf("Enter new title: ");
					gets(string);
					cp->title = (char*) realloc(cp->title,sizeof(char)*strlen(string));//firstly update size of memory.
					strcpy(cp->title,string);//And then copy updated version.
					break;
				case 2:
					printf("Enter new author: ");
					gets(string);
					cp->author = (char*) realloc(cp->author,sizeof(char)*strlen(string));//firstly update size of memory.
					strcpy(cp->author,string);//And then copy updated version.
					break;
				case 3:
					printf("Enter new publication year: ");//firstly update size of memory.
					scanf("%d",&n_publication);
					cp->publication_year = n_publication;//And then copy updated version.
					break;
				default:
					printf("Invalid Typing.\n");
			}
			break;		
		}
		cp = cp->next;
	}
	if(cp == NULL) printf("Not Found.\n\n");

	return books;
}

ll_book *selectionSort(ll_book *books, int sortChoice)//recursive function
{
	ll_book *spp,*pp,*cp,*sp;
	sp=books;
	spp=books;
	cp=books;
	pp=books;
	
	switch(sortChoice)
	{
		case 1://title
			while(cp!=NULL)
			{
				if(strcmp(cp->title,sp->title)<0)
				{
					sp = cp;
					spp = pp;
				}
				pp = cp;
				cp = cp->next;
			}
			if(sp!=NULL)
			{
				if(sp != spp)
				{
					spp->next = sp->next;
					sp->next = books;
					books = sp;
				}
			}
			if(sp!=NULL)
			{
				books->next = selectionSort(books->next,1);//After smallest one is put the beginning of the list, Send the next one.
			}
			break;
		case 2://author
			while(cp!=NULL)
			{
				if(strcmp(cp->author,sp->author)<0)
				{
					sp = cp;
					spp = pp;
				}
				pp = cp;
				cp = cp->next;
			}
			if(sp!=NULL)
			{
				if(sp != spp)
				{
					spp->next = sp->next;
					sp->next = books;
					books = sp;
				}
			}
			if(sp!=NULL)
			{
				books->next = selectionSort(books->next,2);//After smallest one is put the beginning of the list, Send the next one.
			}
			break;
		case 3://publication year
			while(cp!=NULL)
			{
				if(cp->publication_year < sp->publication_year)
				{
					sp = cp;
					spp = pp;
				}
				pp = cp;
				cp = cp->next;
			}
			if(sp!=NULL)
			{
				if(sp != spp)
				{
					spp->next = sp->next;
					sp->next = books;
					books = sp;
				}
			}
			if(sp!=NULL)
			{
				books->next = selectionSort(books->next,1);//After smallest one is put the beginning of the list, Send the next one.
			}
			break;
	}
	return books;
}

void filterAndSortBooks(ll_book *books, int filterChoice, int sortChoice, int method)
{
	//Firstly filter that wanted and store in the filtered list, and then sort them and store in filtered list. 
	
	ll_book *filteredList,*cp;
	filteredList = NULL;
	cp = books;
	
	char *filter;
	filter = (char*) malloc(sizeof(char)*50);
	int filterNum;
	
	switch(filterChoice)
	{
		case 1:
			printf("Enter the author that you would like to filter: ");
			getchar();
			gets(filter);
			
			while(cp != NULL)
			{
				if(strcmp(filter,cp->author)==0)
				{
					if(method == 0)
					{
						filteredList = insert_at_end(filteredList,cp->ISBN,cp->title,cp->author,cp->publication_year,cp->reserved_status);
					}
					else
					{
						filteredList = insert_at_begin(filteredList,cp->ISBN,cp->title,cp->author,cp->publication_year,cp->reserved_status);
					}
				}
				cp = cp->next;
			}
			if(filteredList == NULL) printf("Not Found.\n");
		break;
		case 2:
			printf("Enter the publication year that you would like to filter: ");
			scanf("%d",&filterNum);
			
			while(cp != NULL)
			{
				if(filterNum == cp->publication_year)
				{
					if(method == 0)
					{
						filteredList = insert_at_end(filteredList,cp->ISBN,cp->title,cp->author,cp->publication_year,cp->reserved_status);
					}
					else
					{
						filteredList = insert_at_begin(filteredList,cp->ISBN,cp->title,cp->author,cp->publication_year,cp->reserved_status);
					}
				}
				cp = cp->next;
			}
			if(filteredList == NULL) printf("Not Found.\n");
		break;
		default:
			printf("Invalid Filter Selection!\n");
	}

	switch(sortChoice)
	{
		case 1:
			filteredList = selectionSort(filteredList,1);
			break;
		case 2:
			filteredList = selectionSort(filteredList,2);
			break;
		case 3:
			filteredList = selectionSort(filteredList,3);
		default:
			printf("Invalid Sorting Selection!\n");
	}
	
	printf("\n");
	print_list(filteredList);
	
	free(filter);
}

ll_book *reverseBookList(ll_book *books)
{
	ll_book *cp;
	cp = books;
	if(cp->next != NULL && cp != NULL)
	{
		while(cp->next->next !=NULL)
		{
			cp = cp->next;
		}
		cp->next->next = books;
		books = cp->next;
		cp->next = NULL;
		
		books->next = reverseBookList(books->next);//After the one at end is put the beginning of the list, Send the next one to function.
	}
	
	return books;
}

void searchBooks(ll_book *books, int searchChoice, char* searchValue)
{
	ll_book *cp;
	cp = books;
	int cntr;
	switch(searchChoice)
	{
		case 1://ISBN
			while(cp != NULL)
			{
				if(strcmp(searchValue,cp->ISBN)==0)//if wanted book is found, print it.
				{
					printf("\nISBN: %s\n",cp->ISBN);
					printf("Title: %s\n",cp->title);
					printf("Author: %s\n",cp->author);
					printf("Publication Year: %d\n",cp->publication_year);
					printf("Reserved Status: %s\n",cp->reserved_status);
					cntr++;
				}
				cp = cp->next;
			}
			break;
		case 2://Author
			while(cp != NULL)
			{
				if(strcmp(searchValue,cp->author)==0)//if wanted book is found, print it.
				{
					printf("\nISBN: %s\n",cp->ISBN);
					printf("Title: %s\n",cp->title);
					printf("Author: %s\n",cp->author);
					printf("Publication Year: %d\n",cp->publication_year);
					printf("Reserved Status: %s\n",cp->reserved_status);
					cntr++;
				}
				cp = cp->next;
			}
			break;
		case 3://Title
			while(cp != NULL)
			{
				if(strcmp(searchValue,cp->title)==0)//if wanted book is found, print it.
				{
					printf("\nISBN: %s\n",cp->ISBN);
					printf("Title: %s\n",cp->title);
					printf("Author: %s\n",cp->author);
					printf("Publication Year: %d\n",cp->publication_year);
					printf("Reserved Status: %s\n",cp->reserved_status);
					cntr++;
				}
				cp = cp->next;
			}
			break;
		default:
			printf("Invalid Typing.\n");
		
	}
	printf("\n");
	
	if(cntr == 0) printf("Not Found.\n");
}

void StudentsBorrowedBooksList(ll_book *books, ll_student *student, char *ID)
{	
	ll_student *cp;
	cp = student;
	ll_book *cpb;
	int i,cntr=0;
	
	printf("\n");
	while(cp != NULL)
	{
		if(strcmp(cp->ID,ID) == 0)
		{
			for(i=0;i<MAX_BORROW;i++)
			{
				if(strlen(cp->borrowedBooks[i])>1)//if ISBN of book is 0, length of that string is 1.
				{
					cpb = books;
					while(cpb != NULL)
					{
						if(strcmp(cpb->ISBN,cp->borrowedBooks[i]) == 0)
						{
							printf("ISBN: %s\n",cpb->ISBN);
							printf("Title: %s\n",cpb->title);
							printf("Author: %s\n",cpb->author);
							printf("Publication Year: %d\n",cpb->publication_year);
							printf("Reserved Status: %s\n\n",cpb->reserved_status);
							cntr++;
							break;
						}
						cpb = cpb->next;
					}
				}
			}	
			break;
		}
		cp = cp->next;
	}
	if(cntr == 0) printf("This student have not any book borrowed.\n\n");
}

ll_student *borrow_std(ll_student *student, char *ISBN, char *ID, int *control)
{
	ll_student *cp;
	cp = student;
	int i,cntr1=0,cntr2=0;
	
	while(cp != NULL)
	{
		if(strcmp(cp->ID,ID)==0)
		{
			for(i=0;i<MAX_BORROW;i++)
			{
				if(strlen(cp->borrowedBooks[i]) == 1)//if ISBN of book is 0, length of that string is 1.
				{
					strcpy(cp->borrowedBooks[i],ISBN);
					break;
				}
				else cntr1++;
			}
			if(cntr1 == MAX_BORROW) printf("This student isn't allowed to borrow more books.\n");
			cntr2++;
			break;
		}
		cp = cp->next;
	}
	if(cntr2 == 0)
	{
		printf("Student not found!\n");
		*control = 0;//to control whether student is found(0:Not Found 1:Found)
	}
	
	return student;
}

ll_book *borrow_bk(ll_book *books, char *ISBN, int *control)
{
	ll_book *cp;
	cp = books;
	int cntr=0;
	
	char *status;
	status = (char*) malloc(sizeof(char)*8);
	strcpy(status,"Borrowed");
	
	while(cp != NULL)
	{
		if(strcmp(cp->ISBN,ISBN)==0)
		{
			cp->reserved_status = status;
			cntr++;
			break;	
		}
		cp = cp->next;
	}
	if(cntr == 0)
	{
		*control = 0;//to control whether student is found(0:Not Found 1:Found)
		printf("Book not found!\n");
	}
	
	return books;
}

ll_student *return_std(ll_student *student, char *ISBN, char *ID, int *control)
{
	ll_student *cp;
	cp = student;
	int i,cntr1=0,cntr2=0;
	
	while(cp != NULL)
	{
		if(strcmp(cp->ID,ID)==0)
		{
			for(i=0;i<MAX_BORROW;i++)
			{
				if(strlen(cp->borrowedBooks[i]) != 1)//if ISBN of book is not 0, length of that string is not 1.
				{
					if(strcmp(cp->borrowedBooks[i],ISBN)==0)
					{
						strcpy(cp->borrowedBooks[i],"0");
						break;
					}
				}
				else cntr1++;
			}
			if(cntr1 == MAX_BORROW) printf("This student didn't borrow this book.\n");
			cntr2++;
			break;
		}
		cp = cp->next;
	}
	if(cntr2 == 0)
	{
		*control=0;//to control whether student is found(0:Not Found 1:Found)
		printf("Student not found!\n");
	}
	
	return student;
}

ll_book *return_bk(ll_book *books, char *ISBN, int *control)
{
	ll_book *cp;
	cp = books;
	int cntr=0;
	
	char *status;
	status = (char*) malloc(sizeof(char)*12);
	strcpy(status,"Not Borrowed");
	
	while(cp != NULL)
	{
		if(strcmp(cp->ISBN,ISBN)==0)
		{
			cp->reserved_status = status;
			cntr++;
			break;	
		}
		cp = cp->next;
	}
	if(cntr == 0)
	{
		*control=0;//to control whether student is found(0:Not Found 1:Found)
		printf("Book not found!\n");
	}

	return books;
}

void save_changes(ll_book *books)//Save changes in linked list of books.
{
	FILE *fptr;
	fptr = fopen("BookList.txt","w");
		
	ll_book *cp;
	cp = books;
	
	while(cp != NULL)
	{
		fprintf(fptr,"%s\n",cp->ISBN);
		fprintf(fptr,"%s\n",cp->title);
		fprintf(fptr,"%s\n",cp->author);
		fprintf(fptr,"%d\n",cp->publication_year);
		fprintf(fptr,"%s\n",cp->reserved_status);
		cp = cp->next;
	}
	
	fclose(fptr);
}

void save_changes_std(ll_student *student)//Save changes in linked list of student.
{
	FILE *fptr;
	fptr = fopen("StudentList.txt","w");
		
	ll_student *cp;
	cp = student;
	int i;
	
	while(cp != NULL)
	{
		printf("ID:%s\n",student->ID);
		printf("Name:%s\n",student->name);
		for(i=0;i<MAX_BORROW;i++)
		{
			if(strlen(student->borrowedBooks[i])>1)
			{
				printf("%d. Borrowed Book: %s\n",i+1,student->borrowedBooks[i]);
			}
		}
		cp = cp->next;
	}
	
	fclose(fptr);
}
