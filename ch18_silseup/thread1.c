#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
void* thread_main(void* arg);

int main(int argc, char* argv[])
{
	pthread_t tid;
	int thread_param = 7;//7를 매개변수로
	if (pthread_create(&tid, NULL, thread_main, &thread_main) != 0) {
		puts("pthread_create() error!");
		exit(0);
	}
	sleep(10);
	puts("end of main");
	exit(0);
}
void* thread_main(void* arg)
{

	int result = *(int*)arg;
	for (unsigned int i = 0; i < result; i++) {
		sleep(1);
		puts("running thread");
	}
	pthread_exit((void*)NULL);
}