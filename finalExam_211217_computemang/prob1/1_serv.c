#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUF_SIZE 100
#define NUM_THREAD 3
#define MAX_CLNT 256
pthread_mutex_t mutx;
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];

void *thread_main(void *arg);
void error_handling(char *msg);
void send_msg(char*msg, int len);
int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;

	int i;
	pthread_t t_id[NUM_THREAD];
	
	int thread_Id;
	int tid_ret_val;
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	pthread_mutex_init(&mutx, NULL);
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

	while(1){
		clnt_adr_sz= sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else{
			pthread_mutex_lock(&mutx);
			clnt_socks[clnt_cnt++] = clnt_sock;
			pthread_mutex_unlock(&mutx);

			pthread_mutex_lock(&mutx);

			for(i=0;i<NUM_THREAD;i++){
				thread_Id = pthread_create(&(t_id[i]), NULL, thread_main, (void*)&clnt_sock);
				if(thread_Id!=0){
					printf("pthread_create error");
					exit(1);
				}
			}
			for(i=0;i<NUM_THREAD;i++){
				thread_Id = pthread_join(t_id[i],(void*)&tid_ret_val);
				if(thread_Id<0){
					printf("pthread_join error\n");
					exit(1);
				}
				printf("스레드아이디 pthread_t 자료형 t_id 배열의 원소가 %ld인 스레드가 종료했습니다\n", t_id[i]);
				printf("스레드아이디 %ld인 스레드의반환값은 %d입니다\n", t_id[i], tid_ret_val);
				printf("Connected cline IP : %s\n", inet_ntoa(clnt_adr.sin_addr));
			}
		}
	}
	pthread_mutex_destroy(&mutx);
	close(serv_sock);
	return 0;
}
void *thread_main(void *arg)
{
	int clnt_sock = *((int*)arg);
	int str_len;
	char msg[BUF_SIZE];
	sleep(1);
	while((str_len=read(clnt_sock,msg,sizeof(msg)))!=0)
		send_msg(msg, str_len);
	pthread_mutex_lock(&mutx);
	for(int i=0;i<clnt_cnt;i++){
		if(clnt_sock==clnt_socks[i]){
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	printf("thread_main 종료\n");
	close(clnt_sock);
	pthread_exit((void*)NULL);
}
void send_msg(char *msg, int len)
{//send to all
	pthread_mutex_lock(&mutx);
	for(int i=0;i<clnt_cnt;i++){
		write(clnt_socks[i], msg, len);
	}
	pthread_mutex_unlock(&mutx);
}
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
