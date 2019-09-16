#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

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

	// 从文件中读取完整的一行
	static CString readOneLineFromFile(FILE* fp)
	{
		char chBuf[BUFFER_SIZE];
		int nLen;

		// 最多读取BUFFER_SIZE-3个字符 空出3个位置用于放'\r''\n''\0'
		if (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)
		{

			nLen = strlen(chBuf);
			if (chBuf[nLen - 1] == '\n')// 若数组的倒数第二个字符为换行符 则读取了完整的一行
			{
				// 将当前位和下两位替换成'\r''\n''\0'
				chBuf[nLen - 1] = '\r';
				chBuf[nLen] = '\n';
				chBuf[nLen + 1] = '\0';
				return CString(chBuf);
			}
			else// 没有读取完完整的一行 或 文件已读取完毕
			{
				if (feof(fp) != 0)// 若文件已读取完毕
				{
					return CString(chBuf);
				}
				CString cstrOneLine = CString(chBuf);
				while (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)// 若没有读取完完整的一行则继续读取
				{
					nLen = strlen(chBuf);
					if (chBuf[nLen - 2] == '\n')// 若数组的倒数第二个字符为换行符 则读取了完整的一行
					{
						// 将当前位和下两位替换成'\r''\n''\0'
						chBuf[nLen - 1] = '\r';
						chBuf[nLen] = '\n';
						chBuf[nLen + 1] = '\0';
						return cstrOneLine + CString(chBuf);
					}
					else
					{
						cstrOneLine += CString(chBuf);
					}
				}
			}
		}
		else// 若文件读完了 则返回" "
		{
			return CString(" ");
		}
	}

	// 从文本文件的指定位置读取指定长度的字符->getc()实现
	// 用于显示超过4000字节大小的文件显示前4000字节文件内容
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

	// 合并2个int
	static int combineTwoInt(int nNum1, int nNum2)
	{
		nNum1 <<= 4;// 左移4位
		return nNum1 | nNum2;
	}

	// 将字符的ASCII码转化为对应的16进制值
	static int convertAcsiiToHex(char ch)
	{
		char chRetsult = ' ';
		if ((ch >= '0') && (ch <= '9'))
			chRetsult = ch - '0';
		else if ((ch >= 'A') && (ch <= 'F'))
			chRetsult = ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			chRetsult = ch - 'a' + 10;
		return chRetsult;
	}

	// 将1个Byte拆分为2个int
	static int* splitUpByte(BYTE byte)
	{
		int* anResult = (int*)malloc(2 * sizeof(int));
		anResult[0] = byte >> 4;// 取高4位
		anResult[1] = byte & 0x0F;// 取低4位
		return anResult;
	}

	// 将字符的16进制值转化为对应的ASCII码
	static int convertHexToAcsii(int nNum)
	{
		int nResult = -1;
		if ((nNum >= 0) && (nNum <= 9))
			nResult = nNum + '0';
		else if ((nNum >= 10) && (nNum <= 15))
			nResult = nNum - 10 + 'A';
		return nResult;
	}
};