#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
//#include<unistd.h>//read�Լ� �ߺ�
#include<sys/types.h>
#include<fcntl.h>

void* read(void* arg);
void* accu(void* arg);
//�ΰ��� semaphore object�� �ʿ��ϴ�
//�׷��� �Ʒ����� sem_init�� ���� �� ���� ������� �����Ѵ�
static sem_t sem_one;
static sem_t sem_two;
static int num;

//semaphore ���� �� �Ҹ� �Լ�
//semaphore�� ���� �� ������ ���� semaphore������ �ּ� �� ����
//sem_init(sem_t *sem, int pshared, unsigned int value);
//semaphore�� ���� ���� �����ϰ� �ִ� semaphore������ �ּ� �� ����
//sem_destroy(sem_T *sem);

//sem����
//semaphore�� ���� �� ������ ���� semaphore������ �ּ� �� ����
//semaphore�� ���� ���� �����ϰ� �ִ� semaphore������ �ּ� �� ����

//pshared����
//0�̿��� �� ���� ��, �� �̻��� ���μ����� ���� ���� ������ semaphore����
//0���޽� �ϳ��� ���μ��� �������� ���� ������ semaphore����
//�츮�� �ϳ��� ���μ��� ���� �����ϴ� ��������� ����ȭ�� �����̹Ƿ� 0����

//value����
//�����Ǵ� semaphore�� �ʱⰪ ����

//semaphore������ �̿��ؼ� �Ӱ迵���� ����ȭ��Ű�� ���(semaphore������ �ʱⰪ�� 1�̶�� ������)
//sem_init�Լ��� ȣ��Ǹ� �ü���� ���ؼ� semaphore object��� ���� ��������� �̰��� 'semaphore value'��� �Ҹ��� ������ �ϳ� ��ϵ�
//sem_post �Լ��� ȣ��Ǹ� �̰��� 1�����ϰ� sem_wait�Լ��� ȣ��Ǹ� �̰��� 1�����ϴµ�
//semaphore���� 0���� �۾��� �� ��� ���࿡ ���� 0�� ���¶�� sem_wait�Լ��� ȣ���ϸ� ȣ���� ������� �Լ��� ��ȯ���� �ʱ⿡, ���ŷ ���¿� ���̰� ��
//�̷��� �ٸ� �����尡 sem_post�Լ��� ȣ���ؼ� semaphore�� ���� 1�� �Ǹ� �� 1�� 0���� sem_wait�� ���ҽ�Ű�鼭 ���ŷ ���¿��� ��������
//�̰��� �Ӱ迵���� ����ȭ�ϴ� �����

//��� ��Ȳ�� �ڵ�� �Ẹ��
//sem_wait(&sem); semaphore������ ���� 0���� 
//�Ӱ迵���� ����
//...
//�Ӱ迵���� ��
//sem_post(&sem);

int main(int argc, char* argv[])
{
	//pthread_t tid[2];//�����߿���
	pthread_t tid1, tid2;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	int thr_ret;
	/*for(unsigned int i=0;i<2;i++){
		if(pthread_create(&(tid[i]), NULL, read, NULL)!=0){
			puts("pthread_create error");
			exit(0);
		}
	}*/

	if (pthread_create(&tid1, NULL, read, NULL) != 0) {
		puts("pthread_create error");
		exit(0);
	}

	if (pthread_create(&tid2, NULL, accu, NULL) != 0) {
		puts("pthread_create error");
		exit(0);
	}

	/*
	for(unsigned int i=0;i<2;i++){
		thr_ret = pthread_join(tid[i], NULL);
		if(thr_ret!=0){
			puts("pthread_join error");
			exit(0);
		}
	}
	*/
	thr_ret = pthread_join(tid1, NULL);
	if (thr_ret != 0) {
		puts("pthread_join error");
		exit(0);
	}

	thr_ret = pthread_join(tid2, NULL);
	if (thr_ret != 0) {
		puts("pthread_join error");
		exit(0);
	}

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	exit(0);
}
//read�Լ�
//sem_two������ ����� sem_wait, sem_post�Լ�
//accu�Լ��� ȣ���ϴ� �����尡 ���� �Һ����� �ʾҴµ� read�Լ��� ȣ���ϴ� �����尡 ���ο� �����͸� �����ϴ� ���� �������� ���� read�Լ����� �о���� ���� �Ӱ迵������ �����ϵ��� ��
//accu�Լ�
//sem_one������ ����� sem_wait, sem_post�Լ�
//read�Լ��� ȣ���ϴ� �����尡 ���ο� �����͸� �����ϱ⵵ ���� accu�Լ��� ȣ���ϴ� �����尡 ���� �Һ��ϴ� ���� ��������(������ �����͸� �������� ���� ��������) accu�Լ����� ���ο� ���� num�� ������ sum������ sum�� �ٲٴ� �ڵ带 �Ӱ迵������ �����ϵ��� ��
//semaphore���� sem_two�� �̿��ؼ� sem_wait�Լ��� sem_two���� 1����
//sem_one�� �̿��ؼ� sem_post�Լ��� sem_one���� 1����
void* read(void* arg)
{
	for (unsigned int i = 0; i < 5; i++) {
		fputs("Input num: ", stdout);

		sem_wait(&sem_two);
		//�Ӱ迵�� ����
		scanf("%d", &num);
		//�Ӱ迵�� ����
		sem_post(&sem_one);

	}
	pthread_exit((void*)NULL);
}
//semaphore���� sem_one�� �̿���
void* accu(void* arg)
{
	int sum = 0;
	for (unsigned int i = 0; i < 5; i++) {
		sem_wait(&sem_one);
		sum += num;
		sem_post(&sem_two);
	}
	printf("result : %d \n", sum);
	pthread_exit((void*)NULL);
}