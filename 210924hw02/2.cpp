#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
#include<time.h>
#include<ctime>
#include<malloc.h>
#include<string.h>

class Tipsware {
private:
	int m_len;
	int m_max_len;
	char* mp_str;
public:
	Tipsware();
	~Tipsware();
	void Init();
	void SetString(const char* ap_str);
	void Destroy();
};
Tipsware::Tipsware() {
	Init();
}
Tipsware::~Tipsware()
{
	free(mp_str);
}
void Tipsware::Init() {
	m_len = m_max_len = 0;
	mp_str = NULL;
	mp_str = (char*)malloc(1);
	*mp_str = 0;
}
void Tipsware::SetString(const char* ap_str)
{
	m_len = strlen(ap_str);
	if (m_max_len < m_len) {
		free(mp_str);
		m_max_len = m_len;
		mp_str = (char*)malloc(m_max_len);
	}
	memcpy(mp_str, ap_str, m_max_len);
}
void Tipsware::Destroy()
{
	free(mp_str);
	Init();
}

namespace Num_class{
	enum class Num_class {
		ROCK,
		SCISSORS,
		PAPER
	};
}
typedef struct gycchang {
	void* numResult;//������ ����� �Է��� ���� ���
	Num_class::Num_class enumObj;
}gycchang, * p_gyc;//cpp�ϱ� typedef struct���� struct �ڷ��� ��밡��

void method1(void *p);
void GetInfo(p_gyc p_gycchang);
void Change1(p_gyc p_gyc, int input);
void Enum(void* const p, char, int);
void Change2(p_gyc p_gyc, Num_class::Num_class);
void Init1(void** const p);
void Init2(void* const p);
void IntToChar(void* const p, char type, int input);


void Init1(void** const p)
{
	*(int*)(&((p_gyc)(*p))->numResult) = 0;
	((p_gyc)(*p))->enumObj = Num_class::Num_class::ROCK;
}
void Init2(void* const p)
{
	(((p_gyc)p)->numResult) = 0;
	printf("%p :(int*)(&((p_gyc)p)->numResult), %p :(p_gyc)p\n", (int*)(&((p_gyc)p)->numResult), (p_gyc)p);
	((p_gyc)p)->enumObj = Num_class::Num_class::ROCK;
}
void GetInfo(p_gyc p_gycchang)
{
	printf("GetInfo�Լ� =>  %d : (int)p_gycchang->numResult, %d : (int)p_gycchang->enumObj\n", (int)p_gycchang->numResult, (int)p_gycchang->enumObj);
}
void method1(void* p)
{
	printf("method1�Լ�: %d: (int)((gycchang*)p)->numResult\t %d: ((gycchang*)p)->enumObj\n", (int)((gycchang*)p)->numResult, ((gycchang*)p)->enumObj);
}
//����ü �Ű������� �޾Ƽ� ���������� �̿��ؼ� switch~case�� �ۼ�
void IntToChar(void* const p, char type, int input)
{
	if (type) {
		printf("IntToChar�Լ�:\n");
		gycchang* p_gyc_tmp = (gycchang*)malloc(sizeof(gycchang));
		Init2(p_gyc_tmp);
		method1(p_gyc_tmp);
		printf("��� ������ �� method1�� ����� IntToChar�Լ��� ���������� malloc�� ���� ����ü�� ����, �ʱ�ȭ �� ���� ����Դϴ�\n");
		method1((gycchang*)p);
		printf("��� ������ �� IntToChar�Լ��� void�������� ������ method1�Լ��� �ѱ� ����Դϴ�\n");
		printf("��� ������ �� ����� ����ü ������ ������ ���±⿡ IntToChar�Լ��� void*�� �Ű������� �����Լ����� ����ü ������ ������ ������ ���� �����մϴ�\n");
		
		printf("%d : p_gyc_tmp->numResult\n", p_gyc_tmp->numResult);
		/*
		int data = 0x12345678;
		void* p_showBlog = &data;
		char* p_charBlog = (char*)p_showBlog;
		printf("�����쿡�� �ۼ��� ���α׷����� c������ ��Ʋ����� ���� ������� �Ǿ��ֽ��ϴ�\n");
		printf("������ ���� %#X�� ���� ����Ʈ ������ 1����Ʈ�� ����غ��� ��Ʋ����� ��Ŀ����� ����ó�� ����Ǿ��ֽ��ϴ�\n", data);
		printf("�����ּ� : %#X, %p == %lld����\n", *(char*)p_showBlog, (char*)p_showBlog, (long long int)p_showBlog);
		for (int i = 0; i < 4; i++) {
			printf("%#X, %lld����\n", *p_charBlog++, (long long int)p_charBlog);
			//����
			//printf("%#X, %p����\n", *(char*)p_showBlog, ((char*)p_showBlog)++);
		}
		printf("���� ��������� �� �� �ֵ��� ���� �������� ������ ������ ���� ����Ʈ ���� ����Ǿ��ֽ��ϴ�\n");
		*/


		//�����߿���
		//memcpy((int*)((gycchang*)p)->numResult,(const char*)&input,4);
		//printf("%d : (int*)((gycchang*)p)->numResult\n", (int)((gycchang*)p)->numResult);
		//printf("%d : *(int*)((gycchang*)p)->numResult\n", *(int*)(((gycchang*)p)->numResult));
		//������ ����(�Ʒ���)
		//*(int*)(((gycchang*)p)->numResult) = input;
		//printf("%d : (int*)((gycchang*)p)->numResult\n", (int)((gycchang*)p)->numResult);
		//printf("%d : *(int*)((gycchang*)p)->numResult\n", *(int*)(((gycchang*)p)->numResult));
		//void�������� �⺻���� �̿�
		void* p_tmp = (int*)(((gycchang*)p)->numResult);// &(*(int*)((gycchang*)p)->numResult);
		printf("%p : (int*)p_tmp�ּ�, %p : (int*)(((gycchang*)p)->numResult)\n", (int*)p_tmp, (int*)(((gycchang*)p)->numResult));
		*(int*)(&((gycchang*)p)->numResult) = input;
		printf("%d : *(int*)(&((gycchang*)p)->numResult)\n", *(int*)(&((gycchang*)p)->numResult));
		//������ ����
		//*(int*)p_tmp = input;
		//*(int*)(((gycchang*)p)->numResult) = input;
		//printf("%d : *(int*)(((gycchang*)p)->numResult)\n", *(int*)(((gycchang*)p)->numResult));
		//�����߿���
		//int* p_tmp_int = (int*)((gycchang*)p)->numResult;
		//*p_tmp_int = input;
		//printf("%d : *p_tmp_int\n", *p_tmp_int);


		//int tmp_int = *p_tmp_int;
		//printf("%d : tmp_int, %d : *(int*)p_tmp_int\n", tmp_int, *(int*)p_tmp_int);
		int temp = *(int*)(&((gycchang*)p)->numResult);
		printf("%d : temp\n", temp);
		return Change1((p_gyc)p, temp);
	}

}
void Change1(p_gyc p_gyc, int input)
{
	Num_class::Num_class num_class;

	switch (input) {
	case 0:
		//enum�ڷ��� �ʱ�ȭ����� �Ϲݺ��� �ʱ�ȭ����̶� �����ϴ�
		p_gyc->enumObj = Num_class::Num_class::ROCK;
		break;
	case 1:
		//enum�ڷ��� �ʱ�ȭ����� �Ϲݺ��� �ʱ�ȭ����̶� �����ϴ�
		p_gyc->enumObj = Num_class::Num_class::SCISSORS;
		break;
	case 2:
		//enum�ڷ��� �ʱ�ȭ����� �Ϲݺ��� �ʱ�ȭ����̶� �����ϴ�
		p_gyc->enumObj = Num_class::Num_class::PAPER;
		break;
	default:
			break;
	}

}
//enum���� ����ؼ� switch~case�� �ۼ�
void Enum(void* const p, char type, int input)
{
	if (type) {
		*(int*)((gycchang*)p)->numResult = input;
		int temp = *(int*)((gycchang*)p)->numResult;
		scanf_s("%d", &(((p_gyc)p)->enumObj));
		rewind(stdin);
		return Change2((p_gyc)p, ((p_gyc)p)->enumObj);
	}
}
void Change2(p_gyc p_gyc, Num_class::Num_class num_class)
{
	switch (num_class)
	{
	case Num_class::Num_class::ROCK:
		p_gyc->enumObj = Num_class::Num_class::ROCK;
		break;
	case Num_class::Num_class::SCISSORS:
		p_gyc->enumObj = Num_class::Num_class::SCISSORS;
		break;
	case Num_class::Num_class::PAPER:
		p_gyc->enumObj = Num_class::Num_class::PAPER;
		break;
	default:
		break;
	}
}



//����ü�� �޾Ƽ� ����ü �ڷ����� 
// 
//Ŭ������ �̿��ؼ� Ŭ������ ������� enum������ ����ϴ� ���

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	int result = 0;
	int total_size = sizeof(gycchang);
	p_gyc p_gycchang=NULL;
	p_gyc add_p_gyc = NULL;
	if (p_gycchang == NULL) {
		total_size = sizeof(gycchang);
		p_gycchang = (p_gyc)malloc(total_size);
	}
	else {
		total_size += sizeof(gycchang);
		p_gycchang = (p_gyc)realloc(p_gycchang, total_size);
		add_p_gyc = (p_gyc)malloc(total_size - sizeof(gycchang));
		add_p_gyc = p_gycchang + sizeof(gycchang);

	}

	int i = 0;//3���� �����ϱ� ���� ȸ�� ���� ����

	//����ü�ʱ�ȭ�Լ�

	if (add_p_gyc == NULL) {
		//1����void�������� ���
		Init2(p_gycchang);
		////2����void�������� ���
		//Init2(&p_gyc);
		printf("%s : ����\n", argv[0]);
		GetInfo(p_gycchang);
		while(1)
		{
			if (i == 3)
				break;
			result = (int)(rand()%3);
			printf("%d : rand()���_�������� ���� ��(rand�Լ��� ���� �ʱ�ȭ�� �Լ�)\n", result);
			IntToChar(p_gycchang, 1, result);
			//Enum(p_gycchang, 1, result);
			printf("������ IntToChar�Լ����� ����ü ���� ���� �Ϸ� : ");
			GetInfo(p_gycchang);
			i++;
		}
	}
	else {
		//1����void�������� ���
		Init2(add_p_gyc);
		////2����void�������� ���
		//Init2(&add_p_gyc);
		//p_gycchang�� ���� �ּҸ� ����Ű�Ƿ� p_gycchang�� sizeof(gycchang)��ŭ 
		//������ �ּ��� ����ü�� �ʱ�ȭ�ȴ�
		//��ġ Doit!C����Թ� �������� 13�� 6��(char*������ int��������
		//Ư����ġ�� ������ �ش� �κ� ���� �����ϴ� ��)ó��
		printf("%s : ����\n", argv[0]);
		printf("���� �ʱ�ȭ�� add_p_gyc�� ��Ҹ� ����غ��ϴ�\n");
		GetInfo(add_p_gyc);

		printf("realloc�� p_gycchang�� ������ ���� �״�� ����Ǿ��ִ��� Ȯ���մϴ�\n");
		GetInfo(p_gycchang);
		printf("�׸��� realloc���� sizeof(p_gycchang)��ŭ ���� ���� �޸𸮿� ����ü ���� �ʱ�ȭ�� ���� �� ����Ǿ��ִ��� Ȯ���մϴ�\n");
		GetInfo(add_p_gyc);
		while (1) {
			if (i == 3)
				break;
			result = (int)(rand()%3);
			//cpp�� �ڷ�����ȯ�� �־(ĳ����) ���ڿ� ��ȣ�ϴ� �͵� �����մϴ�._�輺���� Tipsware CPP �¶��ΰ��� �� ����
			//result = int(rand()%3);
			printf("%d : rand()���_�������� ���� ��(rand�Լ��� ���� �ʱ�ȭ�� �Լ�)\n", result);
			IntToChar(add_p_gyc, 1, result);
			//Enum(add_p_gyc, 1, result);
			printf("������ IntToChar�Լ����� ����ü ���� ���� �Ϸ� : ");
			GetInfo(p_gycchang);
			i++;
		}
	}
	free(p_gycchang);
	if(add_p_gyc!=NULL)	free(add_p_gyc);
	return 0;
}