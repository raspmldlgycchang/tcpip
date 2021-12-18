#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
#define NUM_THREAD 2

void *thread_summation(void *arg);
void send_msg(char*msg, int len);
void error_handling(char *msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int result;//값 합산할 전역변수

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id[NUM_THREAD];
	int tid_return_val;
	int thread_Id;
	int num;
	if(argc!=2){
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	
	pthread_mutex_init(&mutex, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	clnt_adr_sz=sizeof(clnt_adr);

	while(1){
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else{
			pthread_mutex_lock(&mutex);
			clnt_socks[clnt_cnt++]=clnt_sock;
			pthread_mutex_unlock(&mutex);
			
			pthread_mutex_lock(&mutex);
			for(int i=0;i<NUM_THREAD;i++){
				num = rand()%10+1;
				thread_Id = pthread_create(&(t_id[i]), NULL, thread_summation, (void*)&num);
				if(thread_Id!=0){
					printf("pthread_create error");
					exit(1);
				}
			}
			for(int i=0;i<NUM_THREAD;i++){
				thread_Id = pthread_join(t_id[i], (void*)&tid_return_val);
				if(thread_Id<0){
					printf("pthread_join error");
					exit(1);
				}
				printf("스레드아이디 pthread_t자료형 t_id배열의원소가 %ld인 스레드가 종료했습니다\n", t_id[i]);
				printf("스레드아이디 %ld인 스레드의 반환값은 %d입니다\n",t_id[i], tid_return_val); 
				printf("Connected client IP : %s\n", inet_ntoa(clnt_adr.sin_addr));
			}
			printf("두 스레드로부터 합산한 합을 나타내는 변수인 result : %d\n", result);
		}
	}
	close(serv_sock);
	return 0;
}
void * thread_summation(void *arg)
{
    int num = *(int*)arg;
    printf("num from thread : %d\n", num);
    sleep(1);
    result += num;
    printf("thread_summation 종료\n");
    pthread_exit((void*)NULL);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
