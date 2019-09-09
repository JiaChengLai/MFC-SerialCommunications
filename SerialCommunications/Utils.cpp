#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include "Utils.h"

// ×Ö·û´®Æ´½Ó²¢·µ»Ø
char* enhanced_strcat(const char* pchStr1, const char* pchStr2)
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


SYSTEMTIME  getSystemTime()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st;
}