#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <sys/stat.h>

#define BUFFER_SIZE 2048

class Utils
{
public:
	// 两字符串拼接
	static char* enhanced_strcat(const char* pchStr1, const char* pchStr2)
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

	// 获取当前时间
	static  SYSTEMTIME  getSystemTime()
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		return st;
	}

	// 读取文件全部内容到EditControl中->fgets()实现 
	// nMode当前模式 0-仅浏览 1-浏览并发送
	static CString* readFileToEditControl(char* pchFilePath, CString* cstrData, int nMode)
	{
		char chBuf[BUFFER_SIZE];
		FILE* fp;
		int nLen;
		int nFileSize = getFileSize(pchFilePath);

		if ((fp = fopen(pchFilePath, "r")) == NULL)
		{
			exit(EXIT_FAILURE);
		}

		*cstrData = "";
		// 最多读取BUFFER_SIZE-3个字符 空出3个位置用于放'\r''\n''\0'
		while (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)
		{
			nLen = strlen(chBuf);
			if (chBuf[nLen - 1] == '\n')// 如果是因为读到了换行符而结束
			{
				if (nMode == 1)// 若为发送模式每行指令头部添加发送声明
				{
					*cstrData = *cstrData + initTransmitDeclaration();
					//CSerialCommunicationsDlg cs;
					//cs.updateFileSendingStatusIndicator(ftell(fp), nFileSize);
				}

				// 将当前位和下两位替换成'\r''\n''\0'
				chBuf[nLen - 1] = '\r';
				chBuf[nLen] = '\n';
				chBuf[nLen + 1] = '\0';
			}

			*cstrData = *cstrData + CString(chBuf);
			CSerialCommunicationsDlg cs;
			cs.UpdateData(FALSE);
		}
		return cstrData;
	}

	// 从文本文件的指定位置读取指定长度的字符->getchar()实现
	static char* readFileByPositionAndLength(char* pchFilePath, int nReadLength, long lPosition, long lOrigin)
	{
		// 多开辟一个字符 用于存放\0
		char* pchBuf = (char*)malloc(sizeof(char) * nReadLength + 1);
		FILE* fp;
		char chTemp;
		int i = 0;

		if ((fp = fopen(pchFilePath, "r")) == NULL)
		{
			exit(EXIT_FAILURE);
		}

		fseek(fp, lPosition, lOrigin);
		while ((chTemp = getc(fp)) != EOF && i < nReadLength)
		{
			if (chTemp == '\n')
			{
				// 遇到\n则将当前字符和下个字符替换成\r\n用于换行
				pchBuf[i] = '\r';
				pchBuf[i + 1] = '\n';
				i += 2;
			}
			else
			{
				pchBuf[i] = chTemp;
				i++;
			}
		}

		pchBuf[nReadLength] = '\0';
		fclose(fp);
		return pchBuf;
	}

	// 获取文件大小:通过读取文件的状态信息来获取文件大小而非通过读取文件获得文件大小，速度快
	static long getFileSize(char* pchFilePath)
	{
		struct stat struct_statbuf;
		stat(pchFilePath, &struct_statbuf);
		return struct_statbuf.st_size;
	}

	// 生成发送声明 -> [08:54:51.493]发→◇content□
	static CString initTransmitDeclaration() {
		CString csTemp;
		SYSTEMTIME st = getSystemTime();
		csTemp.Format(_T("[%02d:%02d:%02d.%03d]发→◇"), st.wHour + 8, st.wMinute, st.wSecond, st.wMilliseconds);
		return csTemp;
	}

	// 生成接收声明 -> [08:54:51.493]收←◆content
	static CString initReceiveDeclaration() {
		CString csTemp;
		SYSTEMTIME st = Utils::getSystemTime();
		csTemp.Format(_T("[%02d:%02d:%02d.%03d]收←◆"), st.wHour + 8, st.wMinute, st.wSecond, st.wMilliseconds);
		return csTemp;
	}

	// 将char转化为16进制
	static void StringtoHex(BYTE* GB, int glen, BYTE* SB, int* slen)
	{
		int i;    //遍历输入的字符串
		int a = 0;
		char temp;   //接收字符，用来判断是否为空格，若是则跳过
		char temp1, temp2;   //接收一个字节的两个字符  例如EB，则temp1='E',temp2 = 'B'
		*slen = 0;  //输出的16进制字符串长度
		for (i = 0; i < glen; i++)
		{
			temp = GB[i];
			if (temp == ' ')
				continue;

			if (a == 0)
				temp1 = GB[i];
			if (a == 1)
				temp2 = GB[i];
			a++;

			if (a == 2)
			{
				a = 0;
				temp1 = temp1 - '0';
				if (temp1 > 10)
					temp1 = temp1 - 7;
				temp2 = temp2 - '0';
				if (temp2 > 10)
					temp2 = temp2 - 7;

				SB[*slen] = temp1 * 16 + temp2;
				(*slen)++;
			}
		}
	}

	static int combineTwoDecInt(int nNum1, int nNum2)
	{
		nNum1 <<= 4;// 左移4位
		return nNum1 | nNum2;
	}

	// 将字符的ASCII码转化为对应的十进制值
	static int convertDecInt(char ch)
	{
		if ((ch >= '0') && (ch <= '9'))
			return ch - 0x30;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else return (-1);
	}
};