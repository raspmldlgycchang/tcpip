#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>

#define NUM_THREAD 100
void* thread_inc(void* arg);
void* thread_des(void* arg);
long long num = 0;

//mutex���� ���� �� �Ҹ� �Լ�
//mutex������ ���� �� ������ ���� mutex������ �ּ� �� ����
//pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
//pthread_mutex_destroy(pthread_mutex_t *mutex);

//mutex����(�Ӱ迵���� ��ġ�� �ڹ���)
//mutex������ ���� �� ������ ���� mutex������ �ּ� �� ����
//mutex������ �Ҹ��� ���� mutex������ �ּ� �� ����

//attr����
//�����ϴ� ���ؽ��� Ư�������� ��� �ִ� mutex������ �ּ� �� ����
//������ Ư���� �������� ���� ��� NULL����
//NULL�� �����ϴ� ��쿡�� ��ũ�θ� �̿��ؼ� mutex���� �ʱ�ȭ�� ����

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//mutex�� �̿��ؼ� �Ӱ迵���� ��ġ�� �ڹ���(mutex) �ɾ� ��״� �Լ�
//pthread_mutex_lock(pthread_mutex_t *mutex);
//�Ӱ迵���� ����
//...(�Ӱ迵������ ������ �ڵ�)
//�Ӱ迵���� ��
//mutex�� �̿��ؼ� �Ӱ迵���� ��ġ�� �ڹ��� Ǫ�� �Լ�
//pthread_mutex_unlock(pthread_mutex_t *mutex);

//�̶� �Ӱ迵�������� ������ ���� pthread_mutex_lock�Լ��� ���ŷ ���¿��� ���������� ���Ѵ�
//�̰��� '�����'�̶�� ��
//�̰��� �ذ��ϱ� ���� ���� mutex������


int main(int argc, char* argv[])
{
	int thr_ret;
	pthread_t tid[NUM_THREAD];

	pthread_mutex_init(&mutex, NULL);

	for (unsigned int i = 0; i < NUM_THREAD; i++) {
		//¦����° �������� ����������ϸ鼭 thread_inc�Լ�����
		if (i % 2) {
			if (pthread_create(&(tid[i]), NULL, thread_inc, NULL) != 0) {
				//������ NULL�� �����ִ��� thread_inc��ü���� �����ؼ��������
				puts("pthead_create error");
				exit(0);
			}
		}
		//Ȧ����° �������� ����������ϸ鼭 thread_des�Լ�����
		else {
			if (pthread_create(&(tid[i]), NULL, thread_des, NULL) != 0) {
				puts("pthread_creat error");
				exit(0);
			}
		}
	}

	for (unsigned int i = 0; i < NUM_THREAD; i++) {
		thr_ret = pthread_join(tid[i], NULL);
		if (thr_ret < 0) {//if(thr_ret!=0)
			puts("pthread_join error");
			exit(0);
		}
	}

	printf("result : %lld \n", num);
	pthread_mutex_destroy(&mutex);
	exit(0);
}
void* thread_inc(void* arg)
{
	pthread_mutex_lock(&mutex);
	//�Ӱ迵�� �ڵ� ����
	for (unsigned int i = 0; i < 50000000; i++) {
		num += 1;//���� ���� 1�� ������Ŵ
	}
	//�Ӱ迵�� ��ȯ
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)NULL);
}
//���� �Լ��� �ݴ�� ���ҽ�Ű�� ��
void* thread_des(void* arg)
{
	pthread_mutex_lock(&mutex);
	//�Ӱ迵�� �ڵ� ����
	for (unsigned int i = 0; i < 50000000; i++) {
		num -= 1;//���� ���� 1�� ���ҽ�Ŵ
	}
	//�Ӱ迵�� ��ȯ
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)NULL);
}