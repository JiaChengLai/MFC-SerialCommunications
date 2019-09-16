#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

class Utils
{
public:
	// ���ַ���ƴ��
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

	// ��ȡ��ǰʱ��
	static  SYSTEMTIME  getSystemTime()
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		return st;
	}

	// ���ļ��ж�ȡ������һ��
	static CString readOneLineFromFile(FILE* fp)
	{
		char chBuf[BUFFER_SIZE];
		int nLen;

		// ����ȡBUFFER_SIZE-3���ַ� �ճ�3��λ�����ڷ�'\r''\n''\0'
		if (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)
		{

			nLen = strlen(chBuf);
			if (chBuf[nLen - 1] == '\n')// ������ĵ����ڶ����ַ�Ϊ���з� ���ȡ��������һ��
			{
				// ����ǰλ������λ�滻��'\r''\n''\0'
				chBuf[nLen - 1] = '\r';
				chBuf[nLen] = '\n';
				chBuf[nLen + 1] = '\0';
				return CString(chBuf);
			}
			else// û�ж�ȡ��������һ�� �� �ļ��Ѷ�ȡ���
			{
				if (feof(fp) != 0)// ���ļ��Ѷ�ȡ���
				{
					return CString(chBuf);
				}
				CString cstrOneLine = CString(chBuf);
				while (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)// ��û�ж�ȡ��������һ���������ȡ
				{
					nLen = strlen(chBuf);
					if (chBuf[nLen - 2] == '\n')// ������ĵ����ڶ����ַ�Ϊ���з� ���ȡ��������һ��
					{
						// ����ǰλ������λ�滻��'\r''\n''\0'
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
		else// ���ļ������� �򷵻�" "
		{
			return CString(" ");
		}
	}

	// ���ı��ļ���ָ��λ�ö�ȡָ�����ȵ��ַ�->getc()ʵ��
	// ������ʾ����4000�ֽڴ�С���ļ���ʾǰ4000�ֽ��ļ�����
	static char* readFileByPositionAndLength(char* pchFilePath, int nReadLength, long lPosition, long lOrigin)
	{
		// �࿪��һ���ַ� ���ڴ��\0
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
				// ����\n�򽫵�ǰ�ַ����¸��ַ��滻��\r\n���ڻ���
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

	// ��ȡ�ļ���С:ͨ����ȡ�ļ���״̬��Ϣ����ȡ�ļ���С����ͨ����ȡ�ļ�����ļ���С���ٶȿ�
	static long getFileSize(char* pchFilePath)
	{
		struct stat struct_statbuf;
		stat(pchFilePath, &struct_statbuf);
		return struct_statbuf.st_size;
	}

	// ���ɷ������� -> [08:54:51.493]������content��
	static CString initTransmitDeclaration() {
		CString csTemp;
		SYSTEMTIME st = getSystemTime();
		csTemp.Format(_T("[%02d:%02d:%02d.%03d]������"), st.wHour + 8, st.wMinute, st.wSecond, st.wMilliseconds);
		return csTemp;
	}

	// ���ɽ������� -> [08:54:51.493]�ա���content
	static CString initReceiveDeclaration() {
		CString csTemp;
		SYSTEMTIME st = Utils::getSystemTime();
		csTemp.Format(_T("[%02d:%02d:%02d.%03d]�ա���"), st.wHour + 8, st.wMinute, st.wSecond, st.wMilliseconds);
		return csTemp;
	}

	// �ϲ�2��int
	static int combineTwoInt(int nNum1, int nNum2)
	{
		nNum1 <<= 4;// ����4λ
		return nNum1 | nNum2;
	}

	// ���ַ���ASCII��ת��Ϊ��Ӧ��16����ֵ
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

	// ��1��Byte���Ϊ2��int
	static int* splitUpByte(BYTE byte)
	{
		int* anResult = (int*)malloc(2 * sizeof(int));
		anResult[0] = byte >> 4;// ȡ��4λ
		anResult[1] = byte & 0x0F;// ȡ��4λ
		return anResult;
	}

	// ���ַ���16����ֵת��Ϊ��Ӧ��ASCII��
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