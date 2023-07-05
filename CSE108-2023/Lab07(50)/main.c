#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	part1();
	part2();
	return 0;
}

int minOperations(char s[]) 
{
    int n = strlen(s);
    int i, j, count = 0;
    
    for (i = 0; i < n; ) 
	{
        j = i + 1;
        while (j < n && s[j] == s[i]) 
		{
            j++;
        }
        count++;
        i = j;
    }
    
    return count;
}

void part1(void)
{
    char string[100];
    printf("Enter the string: ");
    scanf("%s", string);
    int num = minOperations(string);
    printf("Minimum number of operations required: %d\n", num);
    return 0;
}

void foo_min_max(int arr[], int n)
{
	int i=0,j;
	int cnt;
	
	while(i<n)
	{
		cnt=0;
		j=0;
		while(j<n)
		{
			if(arr[i]>arr[j])
			{
				cnt++;
			}
			j++;
		}
		if(cnt==(n-1)) 
		{
			printf("max %d\n",arr[i]);	
		}
		i++;
	}
	
	i=0;
	while(i<n)
	{
		cnt=0;
		j=0;
		while(j<n)
		{
			if(arr[i]<arr[j])
			{
				cnt++;
			}
			j++;
		}
		if(cnt==(n-1)) 
		{
			printf("min %d\n",arr[i]);	
		}
		i++;
	}
	
}

void part2(void)
{
	int size,i;
	printf("Enter array size:");
	scanf("%d",&size);
	int arr[size];
	for(i=0;i<size;i++)
	{
		printf("Enter %d. element:",i+1);
		scanf("%d",&arr[i]);
	}
	foo_min_max(arr,size);
}


