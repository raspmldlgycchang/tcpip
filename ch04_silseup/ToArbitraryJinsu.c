#include<stdio.h>
void main()
{
	int i = 0, input_num = 127;
	while (input_num / 2 != 0 || input_num % 2 != 0) {
		printf("%d: %d��° �ڸ��� ������\n", input_num % 2, i++);
		input_num /= 2;
	}
}