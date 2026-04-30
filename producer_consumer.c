#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5
int next_add = 0; //add is kind of bad name but its basically the letter the producer reads
int next_read = 0; // this is what the consumer reads 
int count = 0;

pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER; 
char queue[BUFFER_SIZE];
FILE *fp;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *thread)
{
    char letter;

    while((letter = fgetc(fp)) != EOF)
    {
        pthread_mutex_lock(&mutex);

        while(count == BUFFER_SIZE)
        {
            pthread_cond_wait(&full, &mutex);
        }

        //I set it to letter here too bc the loop above ends because letter == EOF
        queue[next_add] = letter;
        ++count;
        next_add = (next_add + 1) % BUFFER_SIZE;
        
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);

    while(count == BUFFER_SIZE)
    {
        pthread_cond_wait(&full, &mutex);
    }

    queue[next_add] = letter;
    ++count;
    next_add = (next_add + 1) % BUFFER_SIZE;
        
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);

    
    return NULL;
}

void *consumer(void *thread)
{
    char letter;

    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(count == 0)
        {
            pthread_cond_wait(&empty, &mutex);
        }

        letter = queue[next_read];
        --count;
        next_read = (next_read + 1) % BUFFER_SIZE;
        
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        if(letter == EOF){break;}
        printf("%c", letter);
    }
    return NULL;
}

int main()
{
    pthread_t prod, cons;
    
    if((fp=fopen("message.txt", "r"))==NULL){
		printf("ERROR: can't open message.txt!\n");
		return 0;
	}

    pthread_create(&prod,NULL,producer,NULL);
    pthread_create(&cons,NULL,consumer,NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
}