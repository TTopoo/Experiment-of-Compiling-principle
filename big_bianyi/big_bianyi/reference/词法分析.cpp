#include "data.h"

extern FILE * fp;
extern char token[256];

bool isdigit(char ch)
{
	return '0' <= ch && ch <= '9';
}

bool isalpha(char ch)
{
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
}

char* word[NRW] = {"","function", "begin", "end","integer","if","then","else","and","or"};

int getToken()//�ʷ���������
{
	int tokeType = ERROR;         //�ʷ��������صĵ�������
	char ch = getc(fp);

	if (ch != EOF)
	{
		while (ch == ' ' || ch == '\n' || ch == '\t')//�������еķָ���
			ch = getc(fp);  
		if (ch == EOF)
			return FEOF;  //�ļ�����
		if (isalpha(ch))   //�������ĸ������б�ʶ������
		{
			int j = 0;
			token[j++] = ch;
			ch = getc(fp);
			while (isalpha(ch))		//�������ĸ����ϱ�ʶ��������������ʶ����Ͻ���
			{
				token[j++] = ch; //��ϵı�ʶ��������token��
				ch = getc(fp);  //����һ���ַ�
			}
			token[j] = '\0';     //��ʶ����Ͻ���  
			tokeType = id;
			if (ch != EOF) fseek(fp, -1, 1);   //����һ���ַ� 

			//�ж��Ƿ�ؼ���
			for (int i = 1; i < NRW; i++)
				if (!strcmp(token, word[i])){
					tokeType = i;
					break;
				}

		}
		else if (isdigit(ch))//���ִ���
		{
			int j = 0;
			token[j++] = ch;
			ch = getc(fp);  //����һ���ַ�
			while (isdigit(ch))  //�����������������������������������Ͻ���
			{
				token[j++] = ch;  //�������������token��
				ch = getc(fp);  //����һ���ַ�
			}
			token[j] = '\0';  //������Ͻ���   
			tokeType = digits;
			if (ch != EOF) fseek(fp, -1, 1);   //����һ���ַ� 
		}
		else if (ch == '+')
		{
			tokeType = plus;
		}
		else if (ch == '-')
		{
			tokeType = minus;
		}
		else if (ch == '*')
		{
			tokeType = mutiply;
		}
		else if (ch == '/')
		{
			tokeType = div;
		}
		else if (ch == '(')
		{
			tokeType = LP;
		}
		else if (ch == ')')
		{
			tokeType = RP;
		}
		else if (ch == '=')
		{
			tokeType = equ;
		}
		else if (ch == ',')
		{
			tokeType = comma;
		}
		else if (ch == ';')
		{
			tokeType = semi;
		}
		else if (ch == '>')
		{
			tokeType = ge;
		}
		else if (ch == '<')
		{
			tokeType = le;
		}
	}
	return tokeType;
}

