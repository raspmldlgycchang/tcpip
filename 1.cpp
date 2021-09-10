#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
struct gycchang {
	int m_max_len;
	int m_len;
	int m_count;
	char* message;
};
void Init(void* const p, char flag, const char* ap_str)
{
	if (flag) {//flag==1: gycchang����ü �ڷ���
		*(int*)((gycchang*)p)->m_max_len = 1;
		*(int*)((gycchang*)p)->m_len = 1;
		*(int*)((gycchang*)p)->m_count = 0;
		((gycchang*)p)->message = (char*)malloc(1);
	}
}
void Destroy(void **const sp_head) {
	free(((gycchang*)(*sp_head))->message);
}
class Tipsware {
private:
	int m_len;
	int m_max_len;
	char* mp_str;

public:
	Tipsware();
	~Tipsware();
	void Init();
	void SetString(const char*);
	char* GetString() const;
	void printString() const;
	void Destroy();
};

Tipsware::Tipsware() {
	Init();
}
Tipsware::~Tipsware() {
	free(mp_str);
}
void Tipsware::Init() {
	mp_str = 0;
	mp_str = (char*)malloc(1);
	*mp_str = 0;
}
void Tipsware::SetString(const char* ap_str)
{
	m_len = strlen(ap_str)+1;
	if (m_max_len < m_len) {
		free(mp_str);
		m_max_len = m_len;
		mp_str = (char*)malloc(m_max_len);
	}
	memcpy(mp_str, ap_str, m_max_len);
}
char* Tipsware::GetString() const {
	return mp_str;
}
void Tipsware::printString() const {
	printf("%s: mp_str\n", Tipsware::GetString());
}
void Tipsware::Destroy() {
	free(mp_str);
	Init();
}
int main()
{
	int num;
	void* p;
	while (1) {
		printf("� ���ڿ��� ������ �ǰ���, ������?\n");
		printf("����ü gycchang�� �ִ� ������� �̿��Ͻ� �ǰ���?\n");
		printf("�����Ͻ� �Ÿ� 999�� �Է�: ");
		scanf_s("%d", &num);
		if (num != 999) {

		}
		else {
			break;
		}

	}
	::Init();
	return 0;
}