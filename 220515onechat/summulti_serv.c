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
#define NUM_THREAD 2

char* IntToBuf(int i);
void* thread_summation(void* arg);
void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(char* msg);

typedef void(*data_type);
data_type p_data;
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int result;

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t tid[NUM_THREAD];
	int tid_return_val;
	int thread_Id;
	int num;

	if (argc != 2) {
		printf("Usage :%s <port> \n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1) {
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else {
			pthread_mutex_lock(&mutex);
			clnt_socks[clnt_cnt++] = clnt_sock;
			pthread_mutex_unlock(&mutex);

			//handle_clnt에서 read, send함수를 수행
			//clnt_sock을 handle_clnt함수에서 read함수의 매개변수로
			for (int i = 0; i < NUM_THREAD; i++) {
				data_type arr[2] = { (void*)&num, (void*)&clnt_sock };
				p_data = arr;
				thread_Id = pthread_create(&(tid[i]), NULL, thread_summation, p_data);
			}

			//pthread_detach함수는 pthread_join의 역할이므로 이 위치에

			for (int i = 0; i < NUM_THREAD; i++) {
				thread_Id = pthread_join(tid[i], (void*)&tid_return_val);
				//thread_Id = pthread_detach(tid[i]);
				printf("스레드아이디 pthread_t자료형 t_id배열의 원소가 %ld인 스레드가 종료했습니다\n", tid[i]);
				printf("스레드아이디 %ld인 스레드의 반환값은 %d입니다\n", tid[i], tid_return_val);
			}

			printf("Connected client IP :%s\n", inet_ntoa(clnt_adr.sin_addr));
			printf("두 스레드로부터 합산한 합을 나타내는 변수인 result :%d\n", result);
		}
	}
	close(serv_sock);
	exit(0);
}
char* IntToBuf(int i)
{
	char* buf = (char*)malloc(1);
	sprintf(buf, "%d", i);
	return buf;
}
void* thread_summation(void* arg)
{
	int* p = (int*)arg;
	int clnt_sock = *(p + 1);
	int str_len = 0;
	char* p_msg = IntToBuf(*(int*)arg);
	while ((str_len = read(clnt_sock, p_msg, sizeof(p_msg))))
		send_msg(p_msg, str_len);
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < clnt_cnt; i++) {
		if (clnt_sock == clnt_socks[i]) {
			while (i++ < clnt_cnt - 1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	int num = *(int*)arg;
	sleep(1);
	result += num;
	pthread_mutex_unlock(&mutex);
	printf("현재까지 sum : %d\n", result);
	close(clnt_sock);
	pthread_exit((void*)NULL);
}
void* handle_clnt(void* arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	char msg[BUF_SIZE];

	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);

	//클라이언트가 추가되면 handle_clnt함수호출되고 변수 clnt_cnt와 배열 clnt_socks가 동시에 수정되는데 A스레드는 clnt_socks에서 소켓 정보 삭제, B스레드는 clnt_cnt를 참고하면 A스레드가 다시 clnt_cnt를 참조하는데 B가 update중이면 안되므로 clnt_socks와 clnt_cnt는 하나의 임계영역에서 관리되어야 한다.

	//끊어진 소켓 정보 삭제
	pthread_mutex_lock(&mutex);
	for (unsigned int i = 0; i < clnt_cnt; i++) {
		if (clnt_sock == clnt_socks[i])
		{
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
	//clnt_socks의 모든 클라에게 write
	for (unsigned int i = 0; i < clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutex);
}
void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}