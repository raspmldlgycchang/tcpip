#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
typedef unsigned int HANDLE;
struct HandleData {
	HANDLE h;
	char* p_string;
};
//cpp
class HandleManager {
private:
	HandleData** mp_users;
	unsigned int m_max_user_cnt, m_max_count;
	unsigned int m_gen_value;
public:
	HandleManager(unsigned int a_max_user_cnt, unsigned int a_max_count) {
		m_gen_value = 1;
		m_max_user_cnt = a_max_user_cnt, m_max_count = a_max_count;
		mp_users = new HandleData * [m_max_user_cnt];
		for (unsigned int j = 0; j < m_max_user_cnt; j++) {
			mp_users[j] = new HandleData[m_max_count];
		}
		for (unsigned int j = 0; j < m_max_user_cnt; j++) {
			memset(mp_users[j], 0, sizeof(HandleData) * m_max_count);
		}
	}
	~HandleManager() {
		HandleData* p_handle = mp_users[0];
		for (unsigned int j = 0; j < m_max_user_cnt; j++) {
			p_handle = &mp_users[j][0];
			for (unsigned int i = 0; i < m_max_count; i++) {
				if (p_handle->h) {
					delete[] p_handle->p_string;
					p_handle++;//sizeof(HandleData)��ŭ ����
				}
			}
			delete[] mp_users[j];
		}
		delete[] mp_users;
	}
	HANDLE SetString(const char* ap_string)
	{
		int len = strlen(ap_string)+1;
		HandleData* p_handle = &mp_users[0][0];
		unsigned int j = 0;
		for (; j < m_max_user_cnt; j++) {
			p_handle = &mp_users[j][0];
			//����ִ°�ã��
			unsigned int i = 0;
			for (i = 0; i < m_max_count; i++) {
				if (p_handle->h)	break;
				else {
					p_handle++;

				}
			}
			if (i < m_max_count) {
				p_handle->p_string = new char[len];
				memcpy(p_handle->p_string, ap_string, len);
				printf("%d ��° user�� %d��° ���Դϴ�\n", j, i);
				p_handle->h = m_gen_value++;
				return p_handle->h;
			}
			else if (j < m_max_user_cnt) {
				p_handle = &mp_users[j++][0];
				//Ȥ�� p_handle = mp_users[j++];
				for (i = 0; i < m_max_count; i++) {
					if (p_handle->h==0)	break;
					else {
						p_handle++;

					}
				}
				if (i < m_max_count) {
					p_handle->p_string = new char[len];
					memcpy(p_handle->p_string, ap_string, len);
					printf("%d ��° user�� %d��° ���Դϴ�\n", j, i);
					p_handle->h = m_gen_value++;
					return p_handle->h;
				}
				else {
					printf("���������һ���ڰ� �����ֽ��ϴ�\n");
					p_handle = &mp_users[j++][0];
					for (i = 0; i < m_max_count; i++) {
						if (p_handle->h)	break;
						else {
							p_handle++;

						}
					}
					if (i < m_max_count) {
						p_handle->p_string = new char[len];
						memcpy(p_handle->p_string, ap_string, len);
						printf("%d ��° user�� %d��° ���Դϴ�\n", j, i);
						p_handle->h = m_gen_value++;
						return p_handle->h;
					}
				}
			}
			else {
				printf("�������ֽ� ����ڼ��� �Ѿ����ϴ�\n");
				return 0xFFFFFFFF;
			}
		}
	}
	void printString(HANDLE h_str)
	{
		HandleData* p_handle = &mp_users[0][0];
		for (unsigned int j = 0; j < m_max_user_cnt; j++) {
			p_handle = &mp_users[j][0];
			for (unsigned int i = 0; i < m_max_count; i++) {
				if (p_handle->h == h_str) {
					printf("%s : ��û�Ͻ� ���ڿ��Դϴ�\n", p_handle->p_string);
					return;
				}
				else if(i<m_max_count){
					p_handle++;

				}
			}
		}
		printf("��û�Ͻ� �޸��ּҰ� �����ϴ�\n");
	}

};
//c
void printString(HandleData** mp_users, unsigned int m_max_user_cnt, unsigned int m_max_count, HANDLE h_str)
{
	HandleData* p_handle = mp_users[0];
	for (unsigned int j = 0; j < m_max_user_cnt; j++) {
		p_handle = &mp_users[j][0];
		for (unsigned int i = 0; i < m_max_count; i++) {
			if (p_handle->h == h_str) {
				return;
			}
			p_handle++;
		}
	}
}
//c
void SetString(const char* ap_string)
{

}
//cpp
int main(int argc, char* argv[])
{
	unsigned int max_user_cnt, max_count;
	printf("�ִ� �� ���� user��, �ִ� �� ���� ���ڿ��� �� user��?:");
	scanf("%d %d", &max_user_cnt, &max_count);
	HandleManager str_user_lists(max_user_cnt, max_count);
	const char* ap_string = "�ȳ��ϼ���";
	HANDLE h_user1_str1 = str_user_lists.SetString(ap_string);
	str_user_lists.printString(h_user1_str1);
	return 0;
}