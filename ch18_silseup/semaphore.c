#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
//#include<unistd.h>//read함수 중복
#include<sys/types.h>
#include<fcntl.h>

void* read(void* arg);
void* accu(void* arg);
//두개의 semaphore object가 필요하다
//그래서 아래에서 sem_init을 통해 두 개의 세마포어를 생성한다
static sem_t sem_one;
static sem_t sem_two;
static int num;

//semaphore 생성 및 소멸 함수
//semaphore의 참조 값 저장을 위한 semaphore변수의 주소 값 전달
//sem_init(sem_t *sem, int pshared, unsigned int value);
//semaphore의 참조 값을 저장하고 있는 semaphore변수의 주소 값 전달
//sem_destroy(sem_T *sem);

//sem변수
//semaphore의 참조 값 저장을 위한 semaphore변수의 주소 값 전달
//semaphore의 참조 값을 저장하고 있는 semaphore변수의 주소 값 전달

//pshared변수
//0이외의 값 전달 시, 둘 이상의 프로세스에 의해 접근 가능한 semaphore생성
//0전달시 하나의 프로세스 내에서만 접근 가능한 semaphore생성
//우리는 하나의 프로세스 내에 존재하는 스레드들의 동기화가 목적이므로 0전달

//value변수
//생성되는 semaphore의 초기값 저장

//semaphore변수를 이용해서 임계영역을 동기화시키는 방법(semaphore변수의 초기값은 1이라고 가정함)
//sem_init함수가 호출되면 운영체제에 의해서 semaphore object라는 것이 만들어지고 이곳에 'semaphore value'라고 불리는 정수가 하나 기록됨
//sem_post 함수가 호출되면 이값이 1증가하고 sem_wait함수가 호출되면 이값이 1감소하는데
//semaphore값은 0보다 작아질 수 없어서 만약에 현재 0인 상태라면 sem_wait함수를 호출하면 호출한 스레드는 함수가 반환되지 않기에, 블락킹 상태에 놓이게 됨
//이럴때 다른 스레드가 sem_post함수를 호출해서 semaphore의 값이 1이 되면 이 1을 0으로 sem_wait가 감소시키면서 블락킹 상태에서 빠져나옴
//이것이 임계영역을 동기화하는 방법임

//방금 상황을 코드로 써보면
//sem_wait(&sem); semaphore변수의 값을 0으로 
//임계영역의 시작
//...
//임계영역의 끝
//sem_post(&sem);

int main(int argc, char* argv[])
{
	//pthread_t tid[2];//실행중에러
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
//read함수
//sem_two변수를 사용한 sem_wait, sem_post함수
//accu함수를 호출하는 스레드가 값을 소비하지 않았는데 read함수를 호출하는 스레드가 새로운 데이터를 생산하는 것을 막기위해 값을 read함수에서 읽어오는 것을 임계영역에서 수행하도록 함
//accu함수
//sem_one변수를 사용한 sem_wait, sem_post함수
//read함수를 호출하는 스레드가 새로운 데이터를 생산하기도 전에 accu함수를 호출하는 스레드가 값을 소비하는 것을 막기위해(이전의 데이터를 가져가는 것을 막기위해) accu함수에서 새로운 값인 num이 더해진 sum값으로 sum을 바꾸는 코드를 임계영역에서 수행하도록 함
//semaphore변수 sem_two를 이용해서 sem_wait함수로 sem_two값을 1감소
//sem_one을 이용해서 sem_post함수로 sem_one값을 1증가
void* read(void* arg)
{
	for (unsigned int i = 0; i < 5; i++) {
		fputs("Input num: ", stdout);

		sem_wait(&sem_two);
		//임계영역 시작
		scanf("%d", &num);
		//임계영역 나감
		sem_post(&sem_one);

	}
	pthread_exit((void*)NULL);
}
//semaphore변수 sem_one을 이용한
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