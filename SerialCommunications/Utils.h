#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <sys/stat.h>

#define BUFFER_SIZE 2048

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

	// ��ȡ�ļ�ȫ�����ݵ�EditControl��->fgets()ʵ�� 
	// nMode��ǰģʽ 0-����� 1-���������
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
		// ����ȡBUFFER_SIZE-3���ַ� �ճ�3��λ�����ڷ�'\r''\n''\0'
		while (fgets(chBuf, BUFFER_SIZE - 2, fp) != NULL)
		{
			nLen = strlen(chBuf);
			if (chBuf[nLen - 1] == '\n')// �������Ϊ�����˻��з�������
			{
				if (nMode == 1)// ��Ϊ����ģʽÿ��ָ��ͷ����ӷ�������
				{
					*cstrData = *cstrData + initTransmitDeclaration();
					//CSerialCommunicationsDlg cs;
					//cs.updateFileSendingStatusIndicator(ftell(fp), nFileSize);
				}

				// ����ǰλ������λ�滻��'\r''\n''\0'
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

	// ���ı��ļ���ָ��λ�ö�ȡָ�����ȵ��ַ�->getchar()ʵ��
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

	// ��charת��Ϊ16����
	static void StringtoHex(BYTE* GB, int glen, BYTE* SB, int* slen)
	{
		int i;    //����������ַ���
		int a = 0;
		char temp;   //�����ַ��������ж��Ƿ�Ϊ�ո�����������
		char temp1, temp2;   //����һ���ֽڵ������ַ�  ����EB����temp1='E',temp2 = 'B'
		*slen = 0;  //�����16�����ַ�������
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
		nNum1 <<= 4;// ����4λ
		return nNum1 | nNum2;
	}

	// ���ַ���ASCII��ת��Ϊ��Ӧ��ʮ����ֵ
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