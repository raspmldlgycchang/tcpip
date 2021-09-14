#include<stdio.h>
#include<stdlib.h>
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
	char* message;
	p_gyc->m_len = strlen(ap_str)+1;//널문자 1바이트 추가
	if (p_gyc->m_max_len < p_gyc -> m_len) {
		free(p_gyc->message);
		p_gyc->m_max_len = p_gyc->m_len;
		p_gyc->message = (char*)malloc(p_gyc->m_max_len + 1);//숫자담을 용도 1바이트 추가
	}
	memcpy(p_gyc->message, ap_str, p_gyc->m_max_len+1);

	const char tmp[MAX_LEN] = "after connection, second(s) has passed";
	method1(&tmp);
	//char * r_tmp = method1(&tmp);
	//char tmp_array[MAX_LEN];
	//strcpy_s(tmp_array, MAX_LEN, r_tmp);
	//return tmp_array;
	return temp_left;
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
	//printf("%s: SetString 반환값\n", r_SetString);
	return 0;
}