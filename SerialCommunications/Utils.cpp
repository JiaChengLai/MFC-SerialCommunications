#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// �ַ���ƴ�Ӳ�����
char* enhanced_strcat(char* pchStr1, char* pchStr2)
{
	char* pchResult = (char*)malloc(strlen(pchStr1) + strlen(pchStr2));
	if (pchResult != NULL)
	{
		strcpy(pchResult, pchStr1);
		return strcat(pchResult, pchStr2);
	}
	else {
		return "";
	}
}