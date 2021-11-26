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
	//pthread_create�� ������� �� �� ����
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
	printf("thread1�� ���̵� tid1 %ld�� ���ϰ� %d == sum�� %d == (1~5������)\n", tid1, *(int*)&thr_ret, sum);
	sleep(3);//�Ͻ������ؼ� tid1�� �����尡 ������ ���� ������ �� �������� ��ٸ�
	//printf("thread1�� ���̵� tid1 %ld�� ���ϰ�=%d ==(1~5������)\n", tid1, (int*)thr_ret);

	thr_ret = pthread_join(tid2, (void*)NULL);
	if (thr_ret < 0) {
		puts("pthread_join error");
		exit(0);
	}
	printf("thread2�� ���̵� tid2 %ld�� ���ϰ� %d == sum �� %d == (6~10������)\n", tid2, *(int*)&thr_ret, sum);
	sleep(3);
	printf("%d == %d(sum���� ���� ���) : sum after all threads were ended.\n", *(int*)&thr_ret, sum);
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