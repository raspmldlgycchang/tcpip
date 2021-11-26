#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>
void* thread_sum(void* data);
int sum;
int main(int argc, char* argv[])
{
	//pthread_create를 순서대로 두 번 진행
	pthread_t tid1, tid2;
	int thr_ret;
	int arg_range1[] = { 1,5 };
	int arg_range2[] = { 6,10 };
	if (pthread_create(&tid1, NULL, (void*)thread_sum, arg_range1) < 0) {
		puts("pthread_create error");
		exit(0);
	}
	if (pthread_create(&tid2, NULL, thread_sum, (void*)arg_range2) != 0) {
		puts("pthread_create error");
		exit(0);
	}
	thr_ret = pthread_join(tid1, (void*)0);
	if (thr_ret != 0) {
		puts("pthread_join error");
		exit(0);
	}
	printf("thread1의 아이디 tid1 %ld의 리턴값 %d == sum값 %d == (1~5까지합)\n", tid1, *(int*)&thr_ret, sum);
	sleep(3);//일시정지해서 tid1인 스레드가 끝나기 전에 메인이 안 끝나도록 기다림
	//printf("thread1의 아이디 tid1 %ld의 리턴값=%d ==(1~5까지합)\n", tid1, (int*)thr_ret);

	thr_ret = pthread_join(tid2, (void*)NULL);
	if (thr_ret < 0) {
		puts("pthread_join error");
		exit(0);
	}
	printf("thread2의 아이디 tid2 %ld의 리턴값 %d == sum 값 %d == (6~10까지합)\n", tid2, *(int*)&thr_ret, sum);
	sleep(3);
	printf("%d == %d(sum변수 직접 출력) : sum after all threads were ended.\n", *(int*)&thr_ret, sum);
	exit(0);
}
void* thread_sum(void* data)
{
	int start = ((int*)data)[0];
	int end = ((int*)data)[1];
	while (start <= end) {
		sum += start;
		start++;
	}
	return (void*)(&sum);
}