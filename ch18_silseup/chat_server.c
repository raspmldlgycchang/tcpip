#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(char* msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t tid;
	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1) {
		cln_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else {
			//임계영역에서 수행
			//accept한 클라이언트 연결요청을 배열에 넣어둔다
			pthread_mutex_lock(&mutex);
			clnt_socks[clnt_cnt++] = clnt_sock;
			pthread_mutex_unlock(&mutex);

			//handle_clnt에서 read,send함수를 수행해야 한다
			//clnt_sock을 handle_clnt함수에서 read함수의 매개변수로 써야해서
			//매개변수를 이렇게 보냄
			if (pthread_create(&tid, NULL, handle_clnt, (void*)&clnt_sock) != 0) {
				puts("pthread_create error");
				exit(0);
			}
			//pthread_detach함수는 pthread_join의 역할이므로 이 위치에서 수행
			if (pthread_detach(tid) != 0) {
				puts("pthread_detach error");
				exit(0);
			}
			printf("Connected client IP : %s \n", inet_ntoa(clnt_adr.sin_addr));
		}
	}
	close(serv_sock);
	exit(0);
}
void* handle_clnt(void* arg)
{
	//clnt_sock을 read함수의 매개인자로 써야 해서
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	char msg[BUF_SIZE];

	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);

	//클라이언트가 새로 추가되면 handle_clnt함수호출되고 변수 clnt_cnt와 배열 clnt_socks에 동시에 변화가 생기는데 만약에
	//A 스레드는 배열 clnt_socks에서 소켓 정보 삭제, B 스레드는 변수 clnt_cnt를 참조하고
	//A 스레드는 변수 clnt_cnt를 참조하는데 B스레드는 배열 clnt_socks에 소켓 정보 추가하는 코드를 수행하는 이 두가지 상황에 대해서 
	//clnt_cnt와 clnt_socks는 하나의 임계영역으로 묶어서 수행해야 한다는 것을 알 수 있다

	//연결이 끊어진클라이언트의 소켓 정보 삭제
	pthread_mutex_lock(&mutex);
	for (unsigned int i = 0; i < clnt_cnt; i++)
	{
		if (clnt_sock == clnt_socks[i])
		{
			//한개의 소켓이 사라졌으므로 앞쪽으로 하나씩 모든 소켓들을 인덱스를 당겨서 줄인다
			while (i++ < clnt_cnt - 1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	close(clnt_sock);
	pthread_exit((void*)NULL);
}
void send_msg(char* msg, int len)//send to all
{
	pthread_mutex_lock(&mutex);
	//clnt_socks의 모든 클라이언트에게 write한다
	//이것을 mutex변수로 lock을걸어서임계영역에서 수행한다
	for (unsigned int i = 0; i < clnt_sock; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutex);
}
void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}