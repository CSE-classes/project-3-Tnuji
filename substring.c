#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 1024

int total = 0;
int n1,n2;
char *s1,*s2;
FILE *fp;

int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1) - 1;  /*length of s1*/
	n2=strlen(s2); /*length of s2*/
	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
}

int num_substring(void)
{
	//Copied the substring check func and modded it for the multithreading
	int i,j,k;
	int count;

	for (i = 0; i <= (n1-n2); i++){   
		count=0;
		for(j = i,k = 0; k <= n2; j++,k++){  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k)){
				break;
				//abcded
				//def
			}
			else
			{
				count++;
				//printf("\ncount = %d for j = %d", count, j);
			}
			if(count==n2)
			{
				//printf("count %d == n2 %d", count, n2);   
				total++;
			}		/*find a substring in this step*/                          
		}
	}
	//printf("\ncount = %d\n", count);
	//printf("%d = total", total);
	return total;
}

int main(int argc, char *argv[])
{
	int count;
 
	readf(fp);
	//printf("%s", s1);
	//printf("%s", s2);
	//printf("\n size of n2 = %d\n", n2);
	//printf("\n size of n1 = %d\n", n1);
	count = num_substring();
 	printf("The number of substrings is: %d\n", count);
	return 1;
}