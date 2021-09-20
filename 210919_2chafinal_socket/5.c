#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char* message);
typedef struct node* p_node;
typedef struct node {
	int value;
	struct node* p_next;
}node;
typedef struct gycchang {
	int m_len;
	int m_max_len;
	int m_count;//1�ʰ� ���������� �����ϸ鼭 ������ų �������
	char* message;
}gycchang;
typedef struct gycchang* p_gyc;
#define BUF_SIZE 44
void Init(void* const p, char flag);
char* SetString_ori(struct gycchang* p_gyc, const char* ap_message);//���� ������ strcat���� ���ڿ� �ٿ��ֱ⸦ ���� ��ȯ
char* SetString_new(struct gycchang* p_gyc, const char* ap_message);//���ο� ������ strcat���� ���ڿ��ٿ��ֱ⸦ ���� ��ȯ
void Init(void* const p, char flag)
{
	if (flag) {
		((p_gyc)p)->m_len = 1;
		printf("%p : (gycchang*)p, %p : &((gycchang*)p)->m_max_len)\n", (p_gyc)p, &((gycchang*)p)->m_max_len);//m_len ��� �ʱ�ȭ �� ����ü ���� ���� �ּҿ� �� ���� �ּҸ� ����غ�
		((p_gyc)p)->m_max_len = 1;
		((p_gyc)p)->m_count = 0;
		((p_gyc)p)->message = NULL;
		((p_gyc)p)->message = (char*)malloc(1);
		((p_gyc)p)->message = 0;
	}
}
void GetInfo(struct gycchang* p_gyc)
{
	struct gycchang* p;
	p = p_gyc;
	printf("%d %d %d %s\n", p->m_len, p->m_max_len, p->m_count, p->message);
}
char* method1(const char(*ap_str)[BUF_SIZE])
{
	char* p_tmp_array = (char*)malloc(BUF_SIZE);
	char tmp_array[BUF_SIZE] = { 0, };
	//strcpy_s(tmp_array, BUF_SIZE, *ap_str);
	strcpy(tmp_array, *ap_str);
	p_tmp_array = tmp_array;
	return p_tmp_array;
}
//�ι��ڸ� �������� �ʰ� �ι��ڸ� �迭�� ���޹��� �Լ����� �ٿ��� printf���ִ� �ڵ�
//char*�� ��ȯ�̿��� char�� �迭�� ��ȯ�ص� �����۵��մϴ�
/*char* method2(char(*ptr)[BUF_SIZE], int str_len)
{
	//Ȯ��(�ι��ڸ� �����ϰ� �Ѿ�͵� �����۵��ϴ��� Ȯ���Ϸ��� int�� ������ �Ű������� �޾Ƽ� Ȯ���غ� ���̴�)
	printf("str_len %d = strlen(*ptr) %d\n", str_len, strlen(*ptr));
	//�ι��ڸ� ���̴� �۾� *ptr = *(ptr+0) : ���ڿ� �����ּ�
	//[BUF_SIZE-1] : ������ �ε����� �ι��ڸ� �־��ش�
	(*ptr)[BUF_SIZE - 1] = '\0';
	printf("%s : �ι��ڸ� ���� ���ڿ��� ����ؾ� �����۵��մϴ�\n", *ptr);
	return *ptr;
}
*/
char* IntToBuf(int i)
{
	char* Buf = (char*)malloc(1);
	sprintf(Buf, "%d", i);
	return Buf;
}
char* SetString_ori(struct gycchang* p_gyc, const char* ap_message)
{
	char ori[BUF_SIZE];

	char temp_left[BUF_SIZE] = { 0, };//18����(����+����� ���鹮��)//after connection, (�����������)(18����)+strcat���� ���� ����(1+21)+�ι��ڿ�(1����Ʈ) = 41
	char temp_right[22];//21����+1����Ʈ(�ι���)//second(s) has passed.(21����)
	char ins[1] = { 0, };//���ڸ� ���ڿ��� �ٲ� ���� ������ �迭//-127~128���� ���尡��//������ ���ڿ��� ������ 1~5�� ���� �Ŷ� �̰ɷ� ���
	int total_size = strlen(temp_left) + 1;
	//const int limit = BUF_SIZE;
	char m_message[BUF_SIZE];

	if (p_gyc == NULL) {
		p_gyc = (struct gycchang*)malloc(sizeof(gycchang));
		total_size = strlen(ap_message) + 1;
		p_gyc->message = (char*)malloc(total_size);
	}
	else {
		total_size = strlen(ap_message) + 1;
		p_gyc->message = (char*)realloc(p_gyc->message, total_size);
	}
	p_gyc->m_max_len = total_size;
	p_gyc->m_len = total_size;
	const char tmp[BUF_SIZE] = "after connection, second(s) has passed.";
	char* r_ori = (char*)malloc(BUF_SIZE);
	r_ori = method1(&tmp);
	//strcpy_s(ori, BUF_SIZE, r_ori);
	strcpy(ori, r_ori);
	//strcpy_s(p_gyc->message, total_size, ap_message);
	strcpy(p_gyc->message, ap_message);
	printf("%s : ori\n", ori);
	for (int i = 0; i < 19; i++) {
		*(temp_left + i) = ori[i];
		if (i == 18)	temp_left[i] = '\0';
	}
	printf("%s : temp_left\n", temp_left);
	total_size = strlen(ap_message) + 1;
	p_gyc->message = (char*)realloc(p_gyc->message, total_size);
	//strcpy_s(p_gyc->message, total_size, ap_message);
	strcpy(p_gyc->message, ap_message);
	printf("%s : p_gyc->message ���� ���ڿ��� ������ ��� �ް� ����\n", p_gyc->message);
	char* p = ins;
	p_gyc->m_count++;
	p = IntToBuf(p_gyc->m_count);
	strcpy(ins, p);
	printf("%s : ins\n", ins);
	printf("%s : temp_left: ins�ٿ��ֱ��ϱ���\n", temp_left);
	for (int i = 18; i < p_gyc->m_max_len; i++)
		*(temp_right + i - 18) = (p_gyc->message)[i];
	printf("%s : temp_right\n", temp_right);
	total_size = BUF_SIZE;
	//strcat_s(temp_left, total_size, ins);
	//strcat_s(temp_left, total_size, " ");
	//strcat_s(temp_left, total_size, temp_right);
	strcat(temp_left, ins);
	printf("%s :visual studio���� ����ÿ��� temp_right���� ��� strcat_s�ؾ� �������Ǽ� �°� �����µ� ������ ������ �𸣰�����  temp_left + ins������ �������� ���ڿ��� �ϼ��Ǿ����ϴ�\n", temp_left);
	//strcat(temp_left, " ");
	//printf("%s : temp_left + ins+ ����\n", temp_left);
	//strcat(temp_left, temp_right);
	//printf("%s : temp_left + ins+����+temp_right\n", temp_left);
	total_size = p_gyc->m_max_len + 1;//p_gyc->m_max_len = strlen(ap_message)+1;�� ���� ���Թ��ڿ����� �����ϰ� ������
	p_gyc->m_max_len = total_size;
	p_gyc->m_len = total_size;

	memcpy(m_message, temp_left, total_size);
	printf("%s : ������ȯ�Ϸ��� ���� ���\n", m_message);
	char* p_message = (char*)malloc(total_size);
	strcpy(p_message, m_message);
	printf("%s : p_message\n", p_message);
	return p_message;
}
char* SetString_new(struct gycchang* p_gyc, const char* ap_message)
{
	int total_size = 0;
	char ori[BUF_SIZE];
	char temp_left[19] = { 0, };//18����(����+����� ���鹮��)+1����Ʈ(�ι���)//after connection, (�����������)(18����)+strcat���� ���� ����(1+21) = 
	char* p_message = NULL;
	p_gyc->m_len = strlen(ap_message) + 1;
	if (p_message == NULL) {
		p_message = (char*)malloc(p_gyc->m_len);
	}
	if (p_gyc == NULL) {
		p_gyc = (struct gycchang*)malloc(sizeof(gycchang));
		total_size = strlen(ap_message) + 1;
		p_gyc->message = (char*)malloc(total_size);
	}
	else {
		total_size = strlen(ap_message) + 1;
		p_gyc->message = (char*)realloc(p_gyc->message, total_size);
	}
	//������ ���̺��� �� ��� ���� ���ڿ� ����� ��쿡�� ���Ҵ��� �Ͽ� cpu ����ӵ��� ���δ�
	if (p_gyc->m_len > p_gyc->m_max_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_message = (char*)malloc(p_gyc->m_max_len);//���� ���ڿ��� ��������� p_message�� ���� ���� strcpy�� ���� char�����ڿ��� �����Ͽ� ���� �� ������ ��ȭ���Ѽ� ��ȯ
	}
	//�� �� ������ ���ڿ������ ���Ե�, �ƴ� ���� strcpy�� ��� �����ؾ� �ϹǷ� if�� �ۿ� ���´�
	//strcpy_s(p_message, p_gyc->m_max_len, ap_message);//strcpy�� �� �Ű�������(1���������ͺ���, �迭)(1���������ͺ���, 1���������ͺ���)(1�����迭�����ͺ����� *���ΰ�,1����char���迭)
	//strcpy_s(p_message, p_gyc->m_max_len, ap_message);
	strcpy(p_message, ap_message);
	//�̰ɷε� 1���� ������ ������ ���ڿ� ���簡 �����ϴ�
	//memcpy(p_message, ap_message, p_gyc->m_max_len);

	const char tmp[BUF_SIZE] = "after connection, second(s) has passed.";
	char* r_tmp = method1(&tmp);

	char tmp_array[BUF_SIZE];
	//strcpy_s(tmp_array, BUF_SIZE, r_tmp);
	strcpy(tmp_array, r_tmp);
	p_gyc->m_count++;
	//���Թ��ڿ��� m_count������ ����� ���̴�
	//�Ʒ��� ins�迭�� ũ�⸦ 1�� �ص� �� ����Ǵ� ���� SetString_ori�Լ����� �� �� �ִµ�
	//ũ�⸦ 2�̻����� ����ִ� �� �´�
	char ins[6] = { 0, };//1����Ʈ�� ��������/���ڸ� ���ڿ��� �ٲ� ���� ������ �迭//-127~128���� ���尡��
						 //������ ���ڿ��� ������ 1~5�� ���� �Ŷ� �̰ɷ� ���+�ι��ڿ�(1����Ʈ)�߰�
	char* p = ins;
	//_itoa�Լ� ��ȯ���� char*���̹Ƿ� p��� char*�� ������ �޴´�
	//p = _itoa(p_gyc->m_count, ins, 10);//_itoa�Լ��� CPP�� C��� �۵��ϴ� �Լ���
	//printf("%s: p, %s: ins\n", p, ins);
	//Ȥ�� �Լ��� ����ؼ� �Ѵ�(_itoa�Լ��� ������ �Լ�)
	p = IntToBuf(p_gyc->m_count);
	strcpy(ins, p);
	printf("%s: p, %s: ins\n", p, ins);
	total_size = strlen(temp_left) + 1;
	char* r_ori = (char*)malloc(BUF_SIZE);
	r_ori = method1(&tmp);
	//strcpy_s(ori, BUF_SIZE, r_ori);
	strcpy(ori, r_ori);
	for (int i = 0; i < 19; i++)
	{
		*(temp_left + i) = ori[i];
		if (i == 18)	*(temp_left + i) = '\0';
	}
	printf("%s : temp_left\n", temp_left);
	
	char temp_right[22] = { 0, };
	total_size = strlen(ap_message) + 1;//strcpy�� ���� ���̿��� �ϹǷ�
	p_gyc->m_max_len = total_size;
	//strcpy_s(p_gyc->message, p_gyc->m_max_len, ap_message);
	strcpy(p_gyc->message, ap_message);
	for (int i = 18; i < p_gyc->m_max_len; i++)
		*(temp_right + i - 18) = *(p_gyc->message + i);
	printf("%s : temp_right\n", temp_right);


	//���� ���ڿ��� ���� �κ��� p_message������ strcpy�� ���ڿ� �����ϰ�(ó�� �Ҵ��̹Ƿ� malloc�� ���)
	//strcpy�� �� �Ű�������(1���������ͺ���, �迭)(1���������ͺ���, 1���������ͺ���)(1�����迭�����ͺ����� *���ΰ�,1����char���迭)
	if (p_message == NULL) {
		total_size = strlen(temp_left) + 1;
		printf("%d : if_ù�޼����Ҵ�_strlen(temp_left)+1\n", (int)(strlen(temp_left) + 1));
		p_message = (char*)malloc(total_size);
		//strcpy_s(p_message, total_size, temp_left);
		////strcpy_s(p_message, p_gyc->m_max_len, temp_left);
		strcpy(p_message, temp_left);
		printf("%s : p_message after temp_left ������\n", p_message);
	}
	//���� �̹� �� �ּҿ� �Ҵ��� �Ǿ��ִٸ� realloc�� ����ؼ� strcat���� �ٿ��ֱ⸦ �Ѵ�(�̶� total_size������ temp_left�迭�� ���̰� �����Ƿ�
	// �̹� �� �ּҿ� �ٸ� ���� ����Ǿ��ִ��ص� ����� �޸𸮸� ���� �޸𸮷� �̵��ѵ� �� �޸𸮿� realloc�Լ��� ���Ҵ��� �ϹǷ�
	// �޸𸮿� �߷��� ���°��� �������� �ʾƵ� �ȴ�.
	else {
		total_size = strlen(temp_left) + 1;
		printf("%d : else_�޼����������Ҵ�_strlen(temp_left)+1\n", (int)(strlen(temp_left) + 1));
		p_message = (char*)realloc(p_message, total_size);
		////strcat_s(p_message, total_size, temp_left);
		//strcpy_s(p_message, total_size, temp_left);
		strcpy(p_message, temp_left);
		printf("%s : p_message after temp_left ������\n", p_message);

	}
	//ins ���Թ��ڿ��� realloc�� ���� ���Ҵ� �ѵ� ���ڿ� �ٿ��ֱ⸦ �ϰ�
	total_size = total_size + 1;//���Թ��ڿ� �����δ� 1����Ʈ
	p_message = (char*)realloc(p_message, total_size);
	//strcat_s(p_message, total_size, ins);
	strcat(p_message, ins);
	printf("%s : p_message after ins �ٿ��ֱ� ����\n", p_message);
	//���鹮�� �ֱ�
	total_size = total_size + 1;
	p_message = (char*)realloc(p_message, total_size);
	//strcat_s(p_message, total_size, " ");
	strcat(p_message, " ");
	printf("%s : ���鹮�� �ٿ��ֱ� �ѵ�\n", p_message);
	//temp_right���ڿ��� realloc�� ���� ���Ҵ�� p_message������ ���ڿ� �ٿ��ֱ⸦ �ؼ�

	printf("%s : p_gyc->message before temp_right�� strcat���� �ٿ��ֱ��ϱ���\n", p_gyc->message);
	total_size = strlen(ap_message) + 1 + 1 + 1;//�������ڿ�(ins���°�)+����1����Ʈ+ins1����Ʈ+�ι���1����Ʈ
	p_gyc->m_max_len = total_size;
	total_size = strlen(temp_left) + strlen(temp_right) + 1 + 1 + 1;
	p_message = (char*)realloc(p_message, total_size);

	printf("%s: p_message temp_right���̱���\n", p_message);
	////strcat_s(p_message, total_size, temp_right);
	strcat(p_message, temp_right);
	printf("%s: p_message ���� ������\n", p_message);
	//Ŭ���̾�Ʈ���� ���ֱ� ���� ���ڿ��� ���̸� ���ο� �ѱ�� ����
	p_gyc->m_max_len = total_size;
	//temp_right�� ins�� ���鹮�ڸ� �� ���� �ִ� ���
	//realloc�� �ϱ����� total_size�� ������ ����� �Ͱ� temp_left���� ���� �����̻�����
	//���ش�
	//�̷��� �Ϸ��� ins�� strcat���� �ٿ��ֱ��ϱ����� temp_right�ʱ�ȭ�� �������ƾ� �Ѵ�
	/*char temp_right[22] = {0,};
	for (int i = 18; i < p_gyc->m_max_len; i++)
		*(temp_right + i) = ori[i];
	printf("%s : temp_right\n", temp_right);
	p_gyc->m_max_len = strlen(temp_left) + strlen(temp_right) + 6 + 1;//6:ins�迭, 1: ����
	total_size = p_gyc->m_max_len;
	p_message = (char*)realloc(p_message, total_size);
	strcat_s(p_message, total_size, ins);
	strcat_s(p_message, total_size, " ");
	strcat_s(p_message, total_size, temp_right);
	printf("%s : p_message�����Ϸ�\n", p_message);*/
	//p_message�� ��ȯ�Ѵ�.
	return p_message;
}
int main(int argc, char* argv[])
{
	gycchang gyc1;
	p_gyc p_gyc1;
	p_gyc1 = &gyc1;
	Init(p_gyc1, 1);
	GetInfo(p_gyc1);//Init�� ����ü������ ����� �Ҵ�Ǿ����� Ȯ��
	const char tmp_message[BUF_SIZE] = "after connection, second(s) has passed.";
	const char* p_tmp_message = tmp_message;
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(INADDR_ANY));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("listen() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");
	else
		printf("a new client is connected\n");
	/*
	char* r_SetString_ori = NULL;
	for (int i = 0; i < 5; i++) {
		r_SetString_ori = SetString_ori(p_gyc1, p_tmp_message);
		printf("������ �ٷ� ���� �� �������� ���ø� �˴ϴ�\n");
		printf("%s : r_SetString_ori ��ȯ��\n", r_SetString_ori);
		//�����ͺ����� sizeof�� �ϸ� 4����Ʈ�� �����Ƿ� r_SetString�� sizeof�� �ƴ϶� ���� ���ڿ��� ���̸�ŭ ������
		write(clnt_sock, r_SetString_ori, p_gyc1->m_max_len);
		fflush(stdout);
		usleep(100000);//�ɼ� : -std=gnu99
	}
	fflush(stdout);*/

	//�Ʒ� �ڵ�� ���� �ּ����� ����ߵ���, temp_left�� strcat���� �ٿ��ֱ��ѵ�
	//r_SetString_ori�� �����Ͽ� ��ȯ�ϴ� ���� �ƴ�
	//���ο� �����ͺ��� p_message�� strcat���� �ٿ��ֱ��ѵ�
	//r_SetString_new�� �����Ͽ� ��ȯ�ϴ� �����μ�
	//���� ����ü ������ ����ϱ⿡ 6�ʺ��� 10�ʱ��� �������ȯ�մϴ�
	char* r_SetString_new = NULL;
	printf("�Ʒ��ڵ�� 6�ʿ���10�ʱ��� ����� ��ȯ�մϴ�. ���� ����ü ������ ����ϱ� �����Դϴ�\n");
	fflush(stdout);

	for (int i = 0; i < 5; i++) {
		r_SetString_new = SetString_new(p_gyc1, p_tmp_message);
		printf("������ �ٷ� ���� �� �������� ���ø� �˴ϴ�\n");
		printf("%s : r_SetString_new ��ȯ��\n", r_SetString_new);
		write(clnt_sock, r_SetString_new, p_gyc1->m_max_len);
		fflush(stdout);
		usleep(100000);//�ɼ� : -std=gnu99
	}
	fflush(stdout);
	r_SetString_new = "the connection is closed";

	printf("%s\n", r_SetString_new);

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