#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUF_SIZE 100
#define NUM_THREAD 3
int number=0;
pthread_mutex_t mutex;
int thread_cnt=1;

void *thread_inc(void *arg);
void *error_handling(char *msg);
int main(int argc, char *argv[])
{
	int i;
	pthread_t t_id[NUM_THREAD];
	
	int thread_Id;
	int tid_ret_val;
	int arg_range[]= {0,30};
	pthread_mutex_init(&mutex, NULL);
	int start=arg_range[0];
	int end = arg_range[1];
	while(start<=end){
		for(i=0;i<NUM_THREAD;i++){
			thread_Id = pthread_create(&(t_id[i]), NULL, thread_inc, NULL);
			if(thread_Id!=0){
				printf("pthread_create error");
				exit(1);			
			}
		}
		start++;
	}
	for(i=0;i<NUM_THREAD;i++){
		thread_Id = pthread_join(t_id[i],(void*)&tid_ret_val);
		if(thread_Id<0){
			printf("pthread_join error\n");
			exit(1);
		}
	}
	printf("number: %d\n", number);
	pthread_mutex_destroy(&mutex);
	return 0;
}
void *thread_inc(void *arg)
{
	pthread_mutex_lock(&mutex);
	sleep(1);
	number+=1;
	pthread_mutex_unlock(&mutex);
	printf("Thread %d: number: %d\n",thread_cnt++, number);
	if(thread_cnt==3)	thread_cnt=1;
	pthread_exit((void*)NULL);

}
