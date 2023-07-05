#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define VECTOR_SIZE 200
#define MAX_WORD_SIZE 20
#define MAX_WORD_D 1001
#define MAX_WORD 100000
#define IGNORED_WORD_NUM 20 
#define INPUT_NUM 100
#define INPUT_SIZE 20
#define THRESHOLD 0

int seperator(const char *string, char user_words[][MAX_WORD_SIZE+25]);
int counter(const char *words[],char *user_word);
void string_cpy(char *w1, char *w2);
int length_arr(char *word);
int read_dict(const char * file_name, char dict[][MAX_WORD_SIZE]);
void purifying_function(char *words);
int  read_text(const char  * text_file, const char  * ignore_file, char *words[]);
double dissimilarity(char * w1, char * w2, char dict[][MAX_WORD_SIZE], float threshold, char *file_name );
int histogram(const char *words[],const int occurrences[], const char hist[][MAX_WORD_SIZE+5+20]);

int main(int argc, char *argv[]) {
	
	char file_name[]="dictionary.txt";
	char text_file[]="input.txt";
	char ignore_file[]="ignore.txt";
	
	char user_words[INPUT_NUM][MAX_WORD_SIZE+25];
	char closest_word[MAX_WORD_SIZE];
	char string[INPUT_NUM*INPUT_SIZE];
	printf("Enter word(s): ");//for taking input.
	gets(string);
	
	char *words[135000];
	read_text(text_file,ignore_file,words);//to take words from given text_file and fill words array with words in the text. 
	char dict[MAX_WORD_D][MAX_WORD_SIZE];
	read_dict(file_name,dict);//to take words from given file_name and fill dict array with words in the text.

	if(seperator(string,user_words)==1)//if there is ONE word to search after storing words in user_words array.
	{
		int num=counter(words,&user_words[0]);
		if(num>0)//if the word is in words array.	
		{
			printf("'%s' appears in '%s' %d times.",user_words,text_file,num);	
		}
		else
		{
			if(dissimilarity(user_words[0],closest_word,dict,THRESHOLD,file_name)==-1)//if dissimilarity is '-1',there isn't word which user typed ,in dictionary file. 
			{
				printf("'%s' doesn't appear in '%s'",user_words,text_file);
			}
			else if(counter(words,closest_word)==0)//if dissimilarity is '0',there is word which user typed but the word isn't in dictionary file.
			{
				printf("'%s' and it's closest word doesn't appear in '%s'",user_words,text_file);
			}
			else
			{
				printf("'%s' doesn't appear in '%s' but '%s' appears %d times.",user_words,text_file,closest_word,counter(words,closest_word));
			}
		}
	}
	else//if there are too many words to search after storing words in user_words array.
	{
		int i,j=0,occurrences[INPUT_NUM];
		char hist[INPUT_NUM+INPUT_NUM][MAX_WORD_SIZE+25];
		for(i=0;user_words[i][0]!='-';i++)//to store words in hist array with name and in occurences array with occurences values.
		{
			occurrences[j]=counter(words,user_words[i]);
			string_cpy(hist[j],user_words[i]);
			if(occurrences[j]==0)
			{
				if(dissimilarity(user_words[i],closest_word,dict,THRESHOLD,file_name)==-1)
				{
					occurrences[j]=-1;//to print 'NO MATCHÝNG'.
				}
				else if(counter(words,closest_word)==0)
				{
					occurrences[j]=-1;//to print 'NO MATCHÝNG'.
				}
				else//if occurences value of current word is 0,next occurences value is gonna be occurences value of closest word in dict.
				{
					j++;
					string_cpy(hist[j],closest_word);
					occurrences[j]=counter(words,closest_word);
				}
			}
			j++;
		}
		hist[j][0]='-';
		histogram(words,occurrences,hist);
	}
		
		
	return 0;
}

int seperator(const char *string, char user_words[][MAX_WORD_SIZE+25])
{
	//to fill user_words array with words input.
	FILE *fptr = fopen("user_words.txt","w");
	fputs(string,fptr);
	fclose(fptr);
	
	fptr = fopen("user_words.txt","r");
	
	int i=0;
	while(fscanf(fptr,"%s",user_words[i]) != EOF)
	{
		i++;
	}
	user_words[i][0]='-';
	fclose(fptr);
	return i;
}

int counter(const char *words[],char *user_word)
{	
	//to determine how many words in text file.
	if(user_word[0]>=65 && user_word[0]<=90)
	{
		user_word[0] += ('a'-'A');
	}
	
	int i,cntr=0;
	for(i=0;(words+i)[0]!='-';i++)
	{
		if(strcmp((words+i),user_word)==0) cntr++;
	}
	
	return cntr;
}

void string_cpy(char *w1, char *w2)
{
	//for copying processings.
	int i=0;
	while(w2[i] != '\0')
	{
		w1[i]=w2[i];
		i++;
	}
	w1[i] = '\0';	
}

int length_arr(char *word)
{
	//to determine length of array.
	int i,length=0;
	for(i=0;word[i]!='\0';i++)
	{
		length++;	
	}
	
	return length;	
}

int read_dict(const char * file_name, char dict[][MAX_WORD_SIZE])
{
	//for reading and fill dict array with dictionary words.
	FILE *fptr;
	fptr = fopen(file_name,"r");
	
	char c;
	while(c != '\n')
	{
		c=fgetc(fptr);//to jump first line.
	}
	
	int i=0,word_cntr=0;
	while(fscanf(fptr,"%s",dict[i]) != EOF)
	{
		fseek(fptr,SEEK_CUR+(8*VECTOR_SIZE-1),SEEK_CUR);//to jump vectors.
		i++;
		word_cntr++;
	}
	dict[i][0]='-';
	fclose(fptr);
	
	return word_cntr;
}

void purifying_function(char *words)
{	
	//function for purifying words from punctions.
	char c='d';
	char string1[50],string2[50],string3[50];
	int length=length_arr(words);
	words[length]='-';
	words[length+1]= '\0';
	string_cpy(string1,words);

	int i=0;
	while(c != '-')
	{
		sscanf(string1,"%c%s",&c,string2);
		if(c!=',' && c!='.' && c!=':' && c!=';' && c!='?' && c!='(' && c!=')' && c!='"')
		{
			string3[i]=c;
			i++;
		}
		string_cpy(string1,string2);
	}
	string3[i-1]='\0';
	string_cpy(words,string3);
}

int  read_text(const char  * text_file, const char  * ignore_file, char *words[])
{
	FILE *fptr = fopen(text_file,"r");
	FILE *fptr2 = fopen(ignore_file,"r");
	
	char ignored_word[IGNORED_WORD_NUM][MAX_WORD_SIZE];
	int i=0;
	while(fscanf(fptr2,"%s",ignored_word[i]) != EOF)//to fill ignored_words array with ignored words.
	{
		i++;
	}
	int length=i;
	
	for(i=0; i<135000; i++)
	{
		words[i] = (char*) malloc(MAX_WORD_SIZE*sizeof(char));
	}
	
	int j,word_cntr=0;
	i=0;
	while(fscanf(fptr,"%s",words+i) != EOF)
	{
		int flag=0;
		purifying_function((words+i));//to purify words from punctions.
		for(j=0;j<length;j++)
		{
			if(strcmp((words+i),ignored_word[j]) == 0)
			{
				flag=1;//if word is ignored words,then flag is 1. 
				break;
			}
		}
		if(flag !=1)//to skip ignored words.
		{
			i++;
			word_cntr++;
		}
	}
	(words+i)[0]='-';//sentinel value.

	fclose(fptr);
	fclose(fptr2);
	
	return word_cntr;
}

double dissimilarity(char * w1, char * w2, char dict[][MAX_WORD_SIZE], float threshold, char *file_name )
{
	int i,cntr=0;
	for(i=0;dict[i][0]!='-';i++)
	{
		if(strcmp(dict[i],w1)==0) cntr++;
	}
	if(cntr==0)
	{
		return -1;//if word isn't in dictionary,return -1.
	}

	//finding w1 process.
	int num=0;
	while(strcmp(w1,dict[num])!=0)//to determine what rank of given word.
	{
		num++;
	}
	FILE *fptr;
	fptr = fopen(file_name,"r");
	
	char c;
	while(c != '\n')//to skip first line.
	{
		c=fgetc(fptr);
	}
	
	//to find vectors and name of given word.
	char words[MAX_WORD_SIZE];
	for(i=0;i<num;i++)
	{
		fscanf(fptr,"%s ",words);
		fseek(fptr,SEEK_CUR+(8*VECTOR_SIZE-1),SEEK_CUR);
	}
	fscanf(fptr,"%s",words);
	double w1_vector[VECTOR_SIZE];
	for(i=0;i<VECTOR_SIZE;i++)
	{
		fscanf(fptr,"%lf ",&w1_vector[i]);
	}
	fclose(fptr);
	/***************************************/
	
	//finding w2 process.
	fptr = fopen(file_name,"r");
	fscanf(fptr,"%s %s",words,words);//to skip first line.
	double w2_vector[VECTOR_SIZE],sum=0,biggest_sum=0;
	while(fscanf(fptr,"%s ",words) != EOF)
	{
		for(i=0;i<VECTOR_SIZE;i++)
		{
			fscanf(fptr,"%lf ",&w2_vector[i]);
			sum += ((w1_vector[i]) - (w2_vector[i]))*((w1_vector[i])-(w2_vector[i]));//to calculate vectors difference between w1 and w2.
		}
		if(strcmp(w1,words) != 0)//to avoid calculating vectors difference between given word and given word.
		{
			sum = sqrt(sum);
			sum=1/sum;
			if(sum>biggest_sum)
			{
			string_cpy(w2,words);
			biggest_sum=sum;
			}
		}
		sum=0;
	}
	
	fclose(fptr);
	
	if(biggest_sum>threshold) return biggest_sum;
	else return 0;
}

int histogram(const char *words[],const int occurrences[], const char hist[][MAX_WORD_SIZE+5+20])
{
	int i,j;
	int num_for_scale=40000,scale ;
	for(i=0;hist[i][0]!='-';i++)//to calculate scale
	{
		if(occurrences[i]<num_for_scale && occurrences[i]!=-1)
		{
			num_for_scale=occurrences[i];
		}
	}
	if(num_for_scale==40000) scale=1; else scale=num_for_scale/5+1;
	
	printf("Scale: %d\n",scale);
	int num;
	for(i=0;hist[i][0]!='-';i++)
	{
		if(occurrences[i]==-1)
		{
			printf("'%s': NO MATCHES\n",hist[i]);
		}
		else if(occurrences[i]==0)//if occurences value of current word is 0,next occurences value is occurences value of closest word.  
		{
			printf("'%s'->'%s': ",hist[i],hist[i+1]);
			num=occurrences[i+1]/scale;
			for(j=0;j<num;j++)
			{
				printf("+");
			}
			printf("\n");
			i++;
		}
		else//to print directly.
		{
			printf("'%s': ",hist[i]);
			num=occurrences[i]/scale;
			for(j=0;j<num;j++)
			{
				printf("*");
			}
			printf("\n");
		}
	}
	return scale;
}
