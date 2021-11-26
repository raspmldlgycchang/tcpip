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

//mutex변수 생성 및 소멸 함수
//mutex변수의 참조 값 저장을 위한 mutex변수의 주소 값 전달
//pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
//pthread_mutex_destroy(pthread_mutex_t *mutex);

//mutex변수(임계영역에 설치할 자물쇠)
//mutex변수의 참조 값 저장을 위한 mutex변수의 주소 값 전달
//mutex변수의 소멸을 위한 mutex변수의 주소 값 전달

//attr변수
//생성하는 뮤텍스의 특성정보를 담고 있는 mutex변수의 주소 값 전달
//별도의 특성을 지정하지 않을 경우 NULL전달
//NULL을 전달하는 경우에는 매크로를 이용해서 mutex변수 초기화도 가능

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//mutex를 이용해서 임계영역에 설치된 자물쇠(mutex) 걸어 잠그는 함수
//pthread_mutex_lock(pthread_mutex_t *mutex);
//임계영역의 시작
//...(임계영역에서 수행할 코드)
//임계영역의 끝
//mutex를 이용해서 임계영역에 설치된 자물쇠 푸는 함수
//pthread_mutex_unlock(pthread_mutex_t *mutex);

//이때 임계영역으로의 진입을 위해 pthread_mutex_lock함수는 블락킹 상태에서 빠져나가지 못한다
//이것을 '데드락'이라고 함
//이것을 해결하기 위한 것이 mutex변수임


int main(int argc, char* argv[])
{
	int thr_ret;
	pthread_t tid[NUM_THREAD];

	pthread_mutex_init(&mutex, NULL);

	for (unsigned int i = 0; i < NUM_THREAD; i++) {
		//짝수번째 스레드라면 스레드생성하면서 thread_inc함수실행
		if (i % 2) {
			if (pthread_create(&(tid[i]), NULL, thread_inc, NULL) != 0) {
				//변수로 NULL을 보내주더라도 thread_inc자체에서 증가해서상관없음
				puts("pthead_create error");
				exit(0);
			}
		}
		//홀수번째 스레드라면 스레드생성하면서 thread_des함수실행
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
	//임계영역 코드 수행
	for (unsigned int i = 0; i < 50000000; i++) {
		num += 1;//전역 변수 1을 증가시킴
	}
	//임계영역 반환
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)NULL);
}
//위의 함수와 반대로 감소시키면 됨
void* thread_des(void* arg)
{
	pthread_mutex_lock(&mutex);
	//임계영역 코드 수행
	for (unsigned int i = 0; i < 50000000; i++) {
		num -= 1;//전역 변수 1을 감소시킴
	}
	//임계영역 반환
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)NULL);
}