#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[50];//strlen(temp_left)+strlen(temp_right)+3=41(visual studio에서 c코드로 SetString_new함수에서 temp_right까지 다 붙여넣기 하고 출력해본 결과(SetString_new에서 반환된값 사용시) 혹은 BUF_SIZE+1=45)
		//따라서 더 큰 값 따르면 된다(버퍼 사이즈가 작지만 않으면 된다)

	int str_len;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	else{
		printf("The connection is created with the server(127.0.0.1, port 8080)\n");
	}
	for(int i=0;i<5;i++){
		str_len = read(sock, message, 50);
		printf("Message from server: %s\n", message);
		fflush(stdout);
		usleep(100000);
	}
	if(str_len==-1)
		error_handling("No data from server during 5 second(s).");

	close(sock);

	return 0;
}
void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
