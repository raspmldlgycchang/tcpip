#include<stdio.h>
#include<stdlib.h>
#include<string.h>//strcat, memcpy, strcat_s, strlen
#include<malloc.h>
//#include<unistd.h>
//#include<arpa/inet.h>
//#include<sys/socket.h>
#define MAX_LEN 40
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
char* method1(const char(*ptr)[MAX_LEN]) {
	char tmp_array[MAX_LEN];
	strcpy_s(tmp_array, MAX_LEN, *ptr);
	return tmp_array;
}
char* SetString(struct gycchang* p_gyc, const char* ap_str)
{
	char* message;
	char temp_left[20] = { 0, };
	p_gyc->m_len = strlen(ap_str) + 2;//숫자끼어넣을것1바이트+널문자1바이트
	if (p_gyc->m_max_len < p_gyc->m_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_gyc->message = (char*)malloc(p_gyc->m_max_len);
	}
	char tmp_array[MAX_LEN] = method1(p_gyc->message);
	p_gyc->m_count++;
	for (int i = 0; i < 19; i++) {// after connection, //18자(temp_left), 
		*(temp_left + i) = tmp_array[i];
		if (i == 18)	temp_left[i] = '\0';
	}
	printf("%s : temp_left\n", temp_left);
	char temp_right[20] = { 0, };
	for (int i = 19; i < p_gyc->m_max_len+1; i++) {// after connection, //18자(temp_left), 
		*(temp_right + i-19) = *(tmp_array + i);
	}
	printf("%s : temp_right\n", temp_right);
	//message에 temp_left와 숫자와 temp_right를 붙여서 변경하는 작업
	char* ins = itoa(p_gyc->m_count, ins, 10);
	int total_size=0;
	if (message != NULL) {
		total_size = strlen(temp_left) + 1;
		message = (char*)realloc(message, total_size);
		message = strcat_s(message, total_size, temp_left);
	}
	else {//처음할당
		total_size = strlen(temp_left) + 1;
		message = (char*)malloc(total_size);
		strcpy_s(message, total_size, temp_left);
	}
	strcat_s(message, p_gyc->m_max_len, ins);//숫자 들어갈 자리(1)+나머지 문자열 자리수(m_max_len,널문자포함)
	strcat_s(message, p_gyc->m_max_len, " ");
	strcat_s(message, p_gyc->m_max_len, temp_right);
	printf("%s : message 수정완료\n", message);
	for (int i = 0; i < p_gyc->m_max_len + 1; i++) {
		printf("%c", (p_gyc->message)[i]);
	}
	printf("\n");
	printf("%s : message SetString함수에서 반환전에 message출력\n", message);
	//memcpy(p_gyc->message, ap_str, p_gyc->m_max_len);
	return message;
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