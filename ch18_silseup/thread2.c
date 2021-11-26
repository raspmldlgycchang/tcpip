#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>

void* thread_func(void* ap_string)
{
	int len = strlen((char*)ap_string) + 1;
	char* msg = (char*)malloc(sizeof(char) * len);
	memcpy(msg, (char*)ap_string, len);
	//printf("%s\n", msg);
	//fflush(stdout);
	for (unsigned int i = 0; i < 5; i++) {
		sleep(1);
		puts("running thread");
	}
	return (void*)msg;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	int threadId;
	char p_string[] = "Hello, I\'\am thread~\n";
	void* thr_ret;

	if (pthread_create(&tid, NULL, thread_func, (void*)p_string) != 0) {
		puts("pthread_create() error");
		exit(0);
	}

	threadId = pthread_join(tid, (void*)&thr_ret);
	if (threadId < 0) {//if(threadId!=0)
		puts("pthread_join error");
		exit(0);
	}

	//메인함수 코드수행하기전까지 함수 실행시간을 주지 않아도 되는 이유는 pthread_join을 통해 이미 종료가 되고 메인함수가 실행되서 그렇다
	printf("Thread return message : %s\n", (char*)thr_ret);//void포인터형이 가리키는 변수에 접근해서 그 변수의 값을 출력하기에서 문자열이니까 시작주소를 주면 되서 char*을 반환해서 주소를 반환하는 것
	printf("종료된 thread의 아이디 tid %ld의 리턴값=%s\n", tid, (char*)thr_ret);
	printf("메인 함수 종료\n");
	free(thr_ret);
	exit(0);
}