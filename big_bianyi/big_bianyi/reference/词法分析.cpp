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

int getToken()//词法分析函数
{
	int tokeType = ERROR;         //词法分析返回的单词类型
	char ch = getc(fp);

	if (ch != EOF)
	{
		while (ch == ' ' || ch == '\n' || ch == '\t')//跳过所有的分隔符
			ch = getc(fp);  
		if (ch == EOF)
			return FEOF;  //文件结束
		if (isalpha(ch))   //如果是字母，则进行标识符处理
		{
			int j = 0;
			token[j++] = ch;
			ch = getc(fp);
			while (isalpha(ch))		//如果是字母则组合标识符；如果不是则标识符组合结束
			{
				token[j++] = ch; //组合的标识符保存在token中
				ch = getc(fp);  //读下一个字符
			}
			token[j] = '\0';     //标识符组合结束  
			tokeType = id;
			if (ch != EOF) fseek(fp, -1, 1);   //回退一个字符 

			//判断是否关键字
			for (int i = 1; i < NRW; i++)
				if (!strcmp(token, word[i])){
					tokeType = i;
					break;
				}

		}
		else if (isdigit(ch))//数字处理
		{
			int j = 0;
			token[j++] = ch;
			ch = getc(fp);  //读下一个字符
			while (isdigit(ch))  //如果是数字则组合整数；如果不是则整数组合结束
			{
				token[j++] = ch;  //组合整数保存在token中
				ch = getc(fp);  //读下一个字符
			}
			token[j] = '\0';  //整数组合结束   
			tokeType = digits;
			if (ch != EOF) fseek(fp, -1, 1);   //回退一个字符 
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

