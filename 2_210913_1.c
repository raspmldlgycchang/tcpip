#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char* message);
typedef struct gycchang {
	int m_max_len;
	int m_len;
	int m_count;
	char* message;
}gyc;
void Init(void* const p, char flag, const char* ap_str)
{
	if (flag) {//flag==1: gycchang구조체 자료형
		*(int*)((gyc*)p)->m_max_len = 1;
		*(int*)((gyc*)p)->m_len = 1;
		*(int*)((gyc*)p)->m_count = 0;
		((gyc*)p)->message = (char*)malloc(1);
	}
}
void SetString(struct gycchang* p_gyc, const char* ap_str)
{
	p_gyc->m_len = strlen(ap_str) + 1;
	if (p_gyc->m_max_len < p_gyc->m_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_gyc->message = (char*)malloc(p_gyc->m_max_len);
	}
	memcpy(p_gyc->message, ap_str, p_gyc->m_max_len);
}
int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[40];

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htmol(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");
	else {
		printf("a new client is connected\n");
	}

	int i = 1;
	gyc gy
		gyc* p_gyc = &gyc;
	while (i < 6) {
		SetString(stru
			write(clnt_sock, message
	}

	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
	
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}