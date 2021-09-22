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
	void* numResult;//정수로 사용자 입력을 받을 경우
	Num_class::Num_class enumObj;
}gycchang, * p_gyc;//cpp니까 typedef struct없이 struct 자료형 사용가능

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
	printf("GetInfo함수 =>  %d : (int)p_gycchang->numResult, %d : (int)p_gycchang->enumObj\n", (int)p_gycchang->numResult, (int)p_gycchang->enumObj);
}
void method1(void* p)
{
	printf("method1함수: %d: (int)((gycchang*)p)->numResult\t %d: ((gycchang*)p)->enumObj\n", (int)((gycchang*)p)->numResult, ((gycchang*)p)->enumObj);
}
//구조체 매개변수를 받아서 정수형변수 이용해서 switch~case문 작성
void IntToChar(void* const p, char type, int input)
{
	if (type) {
		printf("IntToChar함수:\n");
		gycchang* p_gyc_tmp = (gycchang*)malloc(sizeof(gycchang));
		Init2(p_gyc_tmp);
		method1(p_gyc_tmp);
		printf("방금 위에서 본 method1의 결과는 IntToChar함수의 지역변수로 malloc을 통해 구조체를 선언, 초기화 후 보낸 결과입니다\n");
		method1((gycchang*)p);
		printf("방금 위에서 본 IntToChar함수의 void포인터형 변수를 method1함수에 넘긴 결과입니다\n");
		printf("방금 위에서 본 결과는 구조체 포인터 변수를 보냈기에 IntToChar함수의 void*형 매개변수에 메인함수에서 구조체 포인터 변수를 보내는 것은 가능합니다\n");
		
		printf("%d : p_gyc_tmp->numResult\n", p_gyc_tmp->numResult);
		/*
		int data = 0x12345678;
		void* p_showBlog = &data;
		char* p_charBlog = (char*)p_showBlog;
		printf("윈도우에서 작성된 프로그램에서 c파일은 리틀엔디언 정렬 방식으로 되어있습니다\n");
		printf("정수형 변수 %#X의 상위 바이트 값부터 1바이트씩 출력해보면 리틀엔디언 방식에서는 다음처럼 저장되어있습니다\n", data);
		printf("시작주소 : %#X, %p == %lld번지\n", *(char*)p_showBlog, (char*)p_showBlog, (long long int)p_showBlog);
		for (int i = 0; i < 4; i++) {
			printf("%#X, %lld번지\n", *p_charBlog++, (long long int)p_charBlog);
			//에러
			//printf("%#X, %p번지\n", *(char*)p_showBlog, ((char*)p_showBlog)++);
		}
		printf("위의 결과에서도 알 수 있듯이 작은 번지수에 정수형 변수의 하위 바이트 값이 저장되어있습니다\n");
		*/


		//실행중에러
		//memcpy((int*)((gycchang*)p)->numResult,(const char*)&input,4);
		//printf("%d : (int*)((gycchang*)p)->numResult\n", (int)((gycchang*)p)->numResult);
		//printf("%d : *(int*)((gycchang*)p)->numResult\n", *(int*)(((gycchang*)p)->numResult));
		//실행중 에러(아랫줄)
		//*(int*)(((gycchang*)p)->numResult) = input;
		//printf("%d : (int*)((gycchang*)p)->numResult\n", (int)((gycchang*)p)->numResult);
		//printf("%d : *(int*)((gycchang*)p)->numResult\n", *(int*)(((gycchang*)p)->numResult));
		//void포인터형 기본내용 이용
		void* p_tmp = (int*)(((gycchang*)p)->numResult);// &(*(int*)((gycchang*)p)->numResult);
		printf("%p : (int*)p_tmp주소, %p : (int*)(((gycchang*)p)->numResult)\n", (int*)p_tmp, (int*)(((gycchang*)p)->numResult));
		*(int*)(&((gycchang*)p)->numResult) = input;
		printf("%d : *(int*)(&((gycchang*)p)->numResult)\n", *(int*)(&((gycchang*)p)->numResult));
		//실행중 에러
		//*(int*)p_tmp = input;
		//*(int*)(((gycchang*)p)->numResult) = input;
		//printf("%d : *(int*)(((gycchang*)p)->numResult)\n", *(int*)(((gycchang*)p)->numResult));
		//실행중에러
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
		//enum자료형 초기화방법은 일반변수 초기화방법이랑 동일하다
		p_gyc->enumObj = Num_class::Num_class::ROCK;
		break;
	case 1:
		//enum자료형 초기화방법은 일반변수 초기화방법이랑 동일하다
		p_gyc->enumObj = Num_class::Num_class::SCISSORS;
		break;
	case 2:
		//enum자료형 초기화방법은 일반변수 초기화방법이랑 동일하다
		p_gyc->enumObj = Num_class::Num_class::PAPER;
		break;
	default:
			break;
	}

}
//enum변수 사용해서 switch~case문 작성
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



//구조체를 받아서 구조체 자료형을 
// 
//클래스를 이용해서 클래스의 멤버변수 enum변수를 사용하는 방법

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

	int i = 0;//3번만 실행하기 위해 회수 제어 변수

	//구조체초기화함수

	if (add_p_gyc == NULL) {
		//1차원void포인터형 사용
		Init2(p_gycchang);
		////2차원void포인터형 사용
		//Init2(&p_gyc);
		printf("%s : 성공\n", argv[0]);
		GetInfo(p_gycchang);
		while(1)
		{
			if (i == 3)
				break;
			result = (int)(rand()%3);
			printf("%d : rand()결과_서버쪽이 내는 것(rand함수를 통해 초기화한 함수)\n", result);
			IntToChar(p_gycchang, 1, result);
			//Enum(p_gycchang, 1, result);
			printf("서버쪽 IntToChar함수에서 구조체 변수 수정 완료 : ");
			GetInfo(p_gycchang);
			i++;
		}
	}
	else {
		//1차원void포인터형 사용
		Init2(add_p_gyc);
		////2차원void포인터형 사용
		//Init2(&add_p_gyc);
		//p_gycchang도 같은 주소를 가리키므로 p_gycchang의 sizeof(gycchang)만큼 
		//더해진 주소의 구조체가 초기화된다
		//마치 Doit!C언어입문 연습문제 13장 6번(char*형으로 int형변수의
		//특정위치를 가리켜 해당 부분 값을 수정하는 것)처럼
		printf("%s : 성공\n", argv[0]);
		printf("먼저 초기화한 add_p_gyc의 요소를 출력해봅니다\n");
		GetInfo(add_p_gyc);

		printf("realloc된 p_gycchang의 변수의 값이 그대로 저장되어있는지 확인합니다\n");
		GetInfo(p_gycchang);
		printf("그리고 realloc이후 sizeof(p_gycchang)만큼 더한 뒤의 메모리에 구조체 변수 초기화한 것이 잘 저장되어있는지 확인합니다\n");
		GetInfo(add_p_gyc);
		while (1) {
			if (i == 3)
				break;
			result = (int)(rand()%3);
			//cpp는 자료형변환에 있어서(캐스팅) 인자에 괄호하는 것도 지원합니다._김성엽님 Tipsware CPP 온라인강좌 중 내용
			//result = int(rand()%3);
			printf("%d : rand()결과_서버쪽이 내는 것(rand함수를 통해 초기화한 함수)\n", result);
			IntToChar(add_p_gyc, 1, result);
			//Enum(add_p_gyc, 1, result);
			printf("서버쪽 IntToChar함수에서 구조체 변수 수정 완료 : ");
			GetInfo(p_gycchang);
			i++;
		}
	}
	free(p_gycchang);
	if(add_p_gyc!=NULL)	free(add_p_gyc);
	return 0;
}