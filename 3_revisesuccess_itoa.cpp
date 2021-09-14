#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>//itoa
#include<string.h>//strcat, memcpy, strcat_s, strlen
#include<malloc.h>
#define MAX_LEN 44
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
		((gyc*)p)->m_max_len = 1;
		printf("%p : (gyc*)p, %p : &((gyc*)p)->m_max_len)\n", (gyc*)p, &((gyc*)p)->m_max_len);
		((gyc*)p)->m_len = 1;
		((gyc*)p)->m_count = 0;
		((gyc*)p)->message = (char*)malloc(1);
		((gyc*)p)->message = 0;
	}
}
void GetInfo(struct gycchang* p_gyc)
{
	printf("%d %d %d %s\n", p_gyc->m_max_len, p_gyc->m_len, p_gyc->m_count, p_gyc->message);//1 1 0 널문자(null) 
}
char* method1(const char(*ptr)[MAX_LEN])
{
	char* temp = (char*)malloc(sizeof(MAX_LEN));
	char array_temp[MAX_LEN];
	strcpy_s(array_temp,MAX_LEN, *ptr);
	temp = array_temp;
	return temp;
}
char* SetString(struct gycchang* p_gyc, const char* ap_str)
{
	char temp_left[20] = { 0, };
	char* message = NULL;
	p_gyc->m_len = strlen(ap_str)+1;//널문자 1바이트 추가
	if (p_gyc->m_max_len < p_gyc -> m_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_gyc->message = (char*)malloc(p_gyc->m_max_len + 1);//숫자담을 용도 1바이트 추가
	}
	memcpy(p_gyc->message, ap_str, p_gyc->m_max_len+1);

	const char tmp[MAX_LEN] = "after connection, second(s) has passed";
	char * r_tmp = method1(&tmp);
	char tmp_array[MAX_LEN];
	strcpy_s(tmp_array, MAX_LEN, r_tmp);
	p_gyc->m_count++;
	for (int i = 0; i < 18; i++) {// after connection, //18자(temp_left), 
		*(temp_left + i) = tmp_array[i];
		if (i == 18)	temp_left[i] = '\0';
	}
	printf("%s : temp_left\n", temp_left);
	char temp_right[22] = { 0, };
	//printf("%d: p_gyc->m_max_len\n", p_gyc->m_max_len);//39
	//printf("%d: strlen(tmp_array)\n", strlen(tmp_array));
	for (int i = 18; i < p_gyc->m_max_len; i++) {
		*(temp_right + i - 18) = *(p_gyc->message + i);
	}
	printf("%s : temp_right\n", temp_right);
	//message에 temp_left와 숫자와 temp_right를 붙여서 변경하는 작업
	char ins[6] = { 0, };
	char* p;
	p = _itoa(p_gyc->m_count, ins,10);//errno_t __cdecl _itoa_s(int _Value, char *_Buffer, size_t _BufferCount, int _Radix)//_itoa: C안됨, CPP only
	int total_size = 0;
	if (message == NULL) {//처음할당
		total_size = strlen(temp_left) + 1;
		message = (char*)malloc(total_size);
		strcpy_s(message, total_size, temp_left);
	}
	else{//처음할당아닌 경우
		total_size = strlen(temp_left) + 1;
		message = (char*)realloc(message, total_size);
		strcat_s(message, total_size, temp_left);
	}
	strcat_s(message, p_gyc->m_max_len, ins);//숫자 들어갈 자리(1)+나머지 문자열 자리수(m_max_len,널문자포함)
	strcat_s(message, p_gyc->m_max_len, " ");
	strcat_s(message, p_gyc->m_max_len+3, temp_right);//p_gyc->m_max_len : 39, temp_left : 20 + temp_right : 22 = 42 -> 39+3
	printf("%s : message 수정완료\n", message);
	return message;
}
int main()
{
	gyc gy;
	gyc* p_gyc = &gy;
	const char tmp[MAX_LEN] = "after connection, second(s) has passed";
	const char* p_tmp = tmp;
	Init(p_gyc, 1, "after connection, second(s) has passed");
	GetInfo(p_gyc);
	char *r_SetString = SetString(p_gyc, p_tmp);
	printf("%s: SetString 반환값\n", r_SetString);
	return 0;
}