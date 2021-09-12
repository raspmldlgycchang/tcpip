#include<stdio.h>
#include<stdlib.h>
#include<string.h>//strcat, memcpy, strcat_s, strlen
#include<malloc.h>
//#include<unistd.h>
//#include<arpa/inet.h>
//#include<sys/socket.h>
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
		*(((gyc*)p)->message) = 0;
	}
}
void SetString(struct gycchang* p_gyc, const char* ap_str)
{
	char temp_left[20] = { 0, };
	p_gyc->m_len = strlen(ap_str) + 1;
	if (p_gyc->m_max_len < p_gyc->m_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_gyc->message = (char*)malloc(p_gyc->m_max_len);
	}
	p_gyc->m_count++;
	for (int i = 0; i < 19; i++) {// after connection, //18자(temp_left), 
		*(temp_left + i) = (p_gyc->message)[i];
		if (i == 18)	temp_left[i] = '\0';
	}
	printf("%s : temp_left\n", temp_left);
	char temp_right[20] = { 0, };
	for (int i = 19; i < p_gyc->m_max_len+1; i++) {// after connection, //18자(temp_left), 
		*(temp_right + i) = *(p_gyc->message + i);
	}
	printf("%s : temp_right\n", temp_right);
	const char* tmp = p_gyc->m_count;
	char* ins = itoa(p_gyc->m_count, ins, 10);
	strcat_s(temp_left, p_gyc->m_max_len + 1, ins);//숫자 들어갈 자리(1)+나머지 문자열 자리수(m_max_len,널문자포함)
	strcat_s(temp_left, p_gyc->m_max_len+1, " ");
	strcat_s(temp_left, p_gyc->m_max_len+1, temp_right);
	printf("%s : temp_left 바뀐뒤\n", temp_left);
	printf("strlen(temp_left): %d\n", strlen(temp_left));
	p_gyc->message = temp_left;
	for (int i = 0; i < p_gyc->m_max_len + 1; i++) {
		printf("%c", (p_gyc->message)[i]);
	}
	printf("\n");
	//memcpy(p_gyc->message, ap_str, p_gyc->m_max_len);
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
		printf("usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(pf_inet, sock_stream, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = af_inet;
	serv_addr.sin_addr.s_addr = htmol(inaddr_any);
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
	gyc gy;
	gyc* p_gyc = &gy;

	while (1) {
		write(clnt_sock, message, sizeof(message));
		char* inputMessage = read(clnt_sock, message, sizeof(message) - 1);
		if (!strcmp(inputMessage, "bye"))	printf("클라이언트가 연결을 끊을것을 요청했습니다. 클라이언트와의 연결을 끊습니다.\n");
	}

	
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