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
			//�Ӱ迵������ ����
			//accept�� Ŭ���̾�Ʈ �����û�� �迭�� �־�д�
			pthread_mutex_lock(&mutex);
			clnt_socks[clnt_cnt++] = clnt_sock;
			pthread_mutex_unlock(&mutex);

			//handle_clnt���� read,send�Լ��� �����ؾ� �Ѵ�
			//clnt_sock�� handle_clnt�Լ����� read�Լ��� �Ű������� ����ؼ�
			//�Ű������� �̷��� ����
			if (pthread_create(&tid, NULL, handle_clnt, (void*)&clnt_sock) != 0) {
				puts("pthread_create error");
				exit(0);
			}
			//pthread_detach�Լ��� pthread_join�� �����̹Ƿ� �� ��ġ���� ����
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
	//clnt_sock�� read�Լ��� �Ű����ڷ� ��� �ؼ�
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	char msg[BUF_SIZE];

	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);

	//Ŭ���̾�Ʈ�� ���� �߰��Ǹ� handle_clnt�Լ�ȣ��ǰ� ���� clnt_cnt�� �迭 clnt_socks�� ���ÿ� ��ȭ�� ����µ� ���࿡
	//A ������� �迭 clnt_socks���� ���� ���� ����, B ������� ���� clnt_cnt�� �����ϰ�
	//A ������� ���� clnt_cnt�� �����ϴµ� B������� �迭 clnt_socks�� ���� ���� �߰��ϴ� �ڵ带 �����ϴ� �� �ΰ��� ��Ȳ�� ���ؼ� 
	//clnt_cnt�� clnt_socks�� �ϳ��� �Ӱ迵������ ��� �����ؾ� �Ѵٴ� ���� �� �� �ִ�

	//������ ������Ŭ���̾�Ʈ�� ���� ���� ����
	pthread_mutex_lock(&mutex);
	for (unsigned int i = 0; i < clnt_cnt; i++)
	{
		if (clnt_sock == clnt_socks[i])
		{
			//�Ѱ��� ������ ��������Ƿ� �������� �ϳ��� ��� ���ϵ��� �ε����� ��ܼ� ���δ�
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
	//clnt_socks�� ��� Ŭ���̾�Ʈ���� write�Ѵ�
	//�̰��� mutex������ lock���ɾ�Ӱ迵������ �����Ѵ�
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