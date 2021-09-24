#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char* message);
#define BUF_SIZE 40

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_adr;
	char message[BUF_SIZE];

	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	char* p_temp_str = NULL;
	char temp_str[BUF_SIZE / 4] = { 0, };
	while (1)
	{
		fputs("Input q if want exit : ", stdout);
		rewind(stdin);
		fgets(message, BUF_SIZE, stdin);
		printf("%ld : strlen(message)\n", strlen(message));
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		write(sock, message, BUF_SIZE);
		int i = 0;

		str_len = read(sock, message, strlen(message));

		int j = 0;
		int total_size = 0;
		while (total_size <= str_len) {
			*(temp_str + 0) = '\0';
			for (int i = j * str_len / 4; i < str_len / 4 * (j + 1) + 1; i++) {
				printf("%d %c : i인덱스 i번째원소\n", i, message[i]);
				rewind(stdin);
				*(temp_str + i - (j * str_len / 4)) = message[i];
				if (i == str_len / 4 * (j + 1))	*(temp_str + i - (j * str_len / 4)) = 0;
			}
			if (p_temp_str == NULL) {
				//total_size = str_len/4*(j+1)+1;
				total_size = str_len / 4 + 1;
				p_temp_str = (char*)malloc(total_size);
				strcpy(p_temp_str, temp_str);
			}
			else {
				total_size += str_len / 4;
				p_temp_str = (char*)realloc(p_temp_str, total_size);
				strcat(p_temp_str, temp_str);
			}
			printf("%d번째 total_size %d : %s\n", i++, total_size, temp_str);
			fflush(stdout);
			rewind(stdin);
			j++;

		}


		rewind(stdin);

		message[total_size] = 0;
		printf("%s : Message from server\n", message);
		printf("%s : Message from server using 분할\n", p_temp_str);
		rewind(stdin);
		fflush(stdout);

		//if(!str_len)
		//	error_handling("No data from server!");
	}
	close(sock);
	return 0;
}
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}