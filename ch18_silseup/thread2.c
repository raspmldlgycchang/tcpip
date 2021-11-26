#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>

void* thread_func(void* ap_string)
{
	int len = strlen((char*)ap_string) + 1;
	char* msg = (char*)malloc(sizeof(char) * len);
	memcpy(msg, (char*)ap_string, len);
	//printf("%s\n", msg);
	//fflush(stdout);
	for (unsigned int i = 0; i < 5; i++) {
		sleep(1);
		puts("running thread");
	}
	return (void*)msg;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	int threadId;
	char p_string[] = "Hello, I\'\am thread~\n";
	void* thr_ret;

	if (pthread_create(&tid, NULL, thread_func, (void*)p_string) != 0) {
		puts("pthread_create() error");
		exit(0);
	}

	threadId = pthread_join(tid, (void*)&thr_ret);
	if (threadId < 0) {//if(threadId!=0)
		puts("pthread_join error");
		exit(0);
	}

	//�����Լ� �ڵ�����ϱ������� �Լ� ����ð��� ���� �ʾƵ� �Ǵ� ������ pthread_join�� ���� �̹� ���ᰡ �ǰ� �����Լ��� ����Ǽ� �׷���
	printf("Thread return message : %s\n", (char*)thr_ret);//void���������� ����Ű�� ������ �����ؼ� �� ������ ���� ����ϱ⿡�� ���ڿ��̴ϱ� �����ּҸ� �ָ� �Ǽ� char*�� ��ȯ�ؼ� �ּҸ� ��ȯ�ϴ� ��
	printf("����� thread�� ���̵� tid %ld�� ���ϰ�=%s\n", tid, (char*)thr_ret);
	printf("���� �Լ� ����\n");
	free(thr_ret);
	exit(0);
}