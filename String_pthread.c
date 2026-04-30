#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_THREADS	4
#define MAX 1024

void *sub_string(void *);
int readf(FILE *fp);
int total=0;
int nlocal,n1,n2;
char *s1,*s2;
FILE *fp;
pthread_mutex_t total_lock;

int main(int argc, char *argv[])
{
	int i,rc;
	pthread_t threads[NUM_THREADS];

	pthread_mutex_init(&total_lock,NULL);
	readf(fp);
	for(i=0;i<NUM_THREADS;i++){
		rc=pthread_create(&threads[i],NULL,sub_string,(void *)i);
		if (rc){
			printf("ERROR: return error from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for(i=0; i<NUM_THREADS; i++){
		rc = pthread_join(threads[i], NULL);
		if (rc){
			printf("ERROR: return error from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	printf("the occurences of s2 in s1 is %d\n",total);
	pthread_exit(0);
}



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
	s1[strcspn(s1, "\n")] = '\0';
	s2=fgets(s2, MAX, fp);
	s2[strcspn(s2, "\n")] = '\0';
	
	n1=strlen(s1); /*length of s1*/
	n2=strlen(s2); /*length of s2*/
	
	nlocal=n1/NUM_THREADS;  /*data length held by process*/
	if(s1==NULL || s2==NULL ||n1<n2)  /*when error exit*/
		return -1;
}

void *sub_string(void *threadid) 	/*each process searches in the string with the step of nprocs until it reach or beyond*/ 
	/*the (n1-n2)th char which is the last possible beginning of the substring*/
{
	int i,j,k;
	int count;

	int start_index = (int) threadid * nlocal;
	int end_index = start_index + nlocal;
	//Basically I want the last thread to just check whatever is left
	if((int) threadid == NUM_THREADS -1){end_index = n1;};
	//if(end_index > (n1 - n2)) {end_index = n1 - n2;};
	
	for (i = start_index; i < end_index; i++){   
		count=0;
		for(j = i,k = 0; k < n2; j++,k++){  /*search for the next string of size of n2*/  
			if (j > (n1 - 1) || *(s1+j)!=*(s2+k)){
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
				//printf("\nthread %d found 1\n", (int)threadid);
				pthread_mutex_lock(&total_lock);   
				total++;
				pthread_mutex_unlock(&total_lock);
			}		/*find a substring in this step*/                          
		}
	}
	//printf("\ncount = %d\n", count);
	//printf("%d = total", total);
	return (void *)total;
}






