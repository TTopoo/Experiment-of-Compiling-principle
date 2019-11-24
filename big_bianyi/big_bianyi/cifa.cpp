#include "data.h"
using namespace std;

const char* keyword[keywordSum] = //关键词 单词类别：1 注意：关键字大小敏感（区分大小写）。
{
	"","program","var","integer","float","procedure",
	"begin","end","read","write","if",
	"then","else","fi","while","do",
	"endwh","and","or"
};
char token[256];
char singleword[50] = "(),; ",
calcword[10] = "+-*=",
doubleword[10] = "><",//定义双分界符的首字符
PL0Scanin[255] = "E:\\下载\\MobileFile\\170950113_石京儒_实验一\\123.txt";

int row = 1, col = 1;

FILE* fin;//用于指向输入输出文件的指针

char* lwchar(char* a)//转为小写形式与保留字表匹配
{
	char* copy1;
	errno_t err;
	err = _strlwr_s(copy1 = _strdup(a), strlen(a) + 1);
	return copy1;
}


//1保留字	2标识符	3无符号整数	4无符号小数	5运算符	6界符	7注释
int getToken()//词法分析函数
{

	char ch;			//ch为每次读入的字符
	int number = 0, num = 0;	//number用于保存识别出的数字
	int tokeType = ERROR;		//tokeType单词类型

	ch = fgetc(fin);
	if(ch == EOF)
		return FEOF;
	if(ch != EOF)
	{
		while (ch == ' ' || ch == '\t' || ch == '\n')
		{
			if (ch == '\t' || ch == ' '){
				col++;
			}
			else if (ch == '\n'){
				row++;col = 1;
			}
			ch = fgetc(fin);
		}
		col++;
		if (ch == EOF)				//文件结束
			return FEOF;
		if (isalpha(ch) || ch == '_')//如果是字母或者下划线，则进行标识符处理
		{
			int j = 0;
			token[j++] = ch;
			ch = fgetc(fin);
			col++;
			while (isalnum(ch))		//如果是字母数字则组合标识符；如果不是则标识符组合结束
			{
				token[j++] = ch;	//组合的标识符保存在id中
				ch = fgetc(fin);	//读下一个字符
				col++;
			}
			token[j] = '\0';		//标识符组合结束
			tokeType = id;			//类型为标识符
			if (ch != EOF)
			{
				fseek(fin, -1, 1);	//回退一个字符
				col--;
			}

			//查保留字
			int n = 1;
			while ((n < keywordSum) && strcmp(lwchar(token), keyword[n]))
				n++;
			tokeType = n ;
			if (n >= keywordSum && strlen(token) <= 20)	//不是保留字，输出标识符
				printf("2，%s，%d，%d\n", token, row, col - strlen(token));	
			else if (n < keywordSum)
				printf("1，%s，%d，%d\n", token, row, col - strlen(token));
			else{
				tokeType = -2;
				printf("ERROR\n");
			}

		}
		else if (isdigit(ch) && ch != '0')	//数字处理，首位数字不为0，则可能是无符号整数/小数
		{
			number = ch - '0';
			int j = 0;
			token[j++] = ch;
			ch = fgetc(fin);		//读下一个字符
			col++;
			int xiaoshu = 0;
			while (isdigit(ch))
			{
				number = 10 * number + (ch - '0');
				token[j++] = ch;
				ch = fgetc(fin);	//读下一个字符
				col++;
			}
			if (ch == '.')
			{
				token[j++] = ch;
				ch = fgetc(fin);
				col++;
				xiaoshu = 1;
				if (isdigit(ch))	//x.xx
				{
					num = ch - '0';
					token[j++] = ch;
					ch = fgetc(fin);
					col++;
					while (isdigit(ch))
					{
						num = 10 * num + (ch - '0');
						token[j++] = ch;
						ch = fgetc(fin);  //读下一个字符
						col++;
					}
					fseek(fin, -1L, SEEK_CUR);
					col--;

				}
				else	//x.
				{
					fseek(fin, -1L, SEEK_CUR);
					col--;
				}
			}
			else		//x
			{
				fseek(fin, -1L, SEEK_CUR);
				col--;
			}
			token[j] = '\0';
			if (num == 0 && xiaoshu == 0)
			{
				printf("3，%s，%d，%d\n", token, row, col - strlen(token));
				tokeType = digits;
			}
				
			else
			{
				printf("4，%s，%d，%d\n", token, row, col - strlen(token));
				tokeType = deci;
			}
				
			number = 0; num = 0;
		}
		else if (strchr(singleword, ch) > 0)  //单分符处理
		{
			token[0] = ch;token[1] = '\0';
			if (ch == '(') tokeType = LP;
			else if (ch == ')') tokeType = RP;
			else if (ch == ',') tokeType = comma;
			else if (ch == ';') tokeType = semi;
			printf("6，%s，%d，%d\n", token, row, col-1);//输出单分界符符号
		}
		
		else if (strchr(calcword, ch) > 0)  //运算符处理
		{
			token[0] = ch;token[1] = '\0';
			if (ch == '=')
			{
				ch = fgetc(fin); col++;
				if (ch == '=')
				{
					token[1] = '='; token[2] = '\0';
					tokeType = eqeq;
					printf("5，%s，%d，%d\n", token, row, col - strlen(token));
				}
				else
				{
					fseek(fin, -1L, SEEK_CUR);
					col--;
					tokeType = equ;
					printf("5，%s，%d，%d\n", token, row, col - 1);//输出运算符符号
				}
			}
			else
			{
				if (ch == '+')	tokeType = plus;
				else if(ch == '-')	tokeType = minus;
				else if (ch == '*')	tokeType = mutiply;
				printf("5，%s，%d，%d\n", token, row, col - 1);//输出运算符符号
			}
				
		}
		
		else if (strchr(doubleword, ch) > 0)  //双分界符处理
		{
			bool flag = 0;
			token[0] = ch;
			ch = fgetc(fin);  //读下一个字符判断是否为双分界符
			col++;
			if (token[0] == '<' && ch == '>')
			{
				token[1] = ch; token[2] = '\0';
				printf("5，%s，%d，%d\n", token, row, col - strlen(token));
				flag = 1;
				tokeType = lege;
			}

			if (ch == '=')		//如果是=，组合双分界符
			{
				token[1] = ch;token[2] = '\0';	//组合双分界符结束
				if (token[0] == '<')
					tokeType = leeq;
				else
					tokeType = geeq;
			}
			else				//不是=则为单分界符
			{
				if (token[0] == '<')
					tokeType = le;
				else
					tokeType = ge;
				token[1] = '\0';

				fseek(fin, -1L, SEEK_CUR);
				col--;

			}				
			//输出单或双分界符符号
			if (flag == 0)
				printf("5，%s，%d，%d\n", token, row, col-1);
		}
		else if (ch == '/')			//注释or运算除法处理
		{
			ch = fgetc(fin);		//读下一个字符
			col++;
			if (ch == '*')			//如果是*，则开始处理注释
			{
				char ch1;
				ch1 = fgetc(fin);	//读下一个字符
				do					//删除注释
				{
					ch = ch1; ch1 = fgetc(fin);
				} while ((ch != '*' || ch1 != '/') && ch1 != EOF);	//直到遇到注释结束符或文件尾
			}
			else if (ch == '/')		//单行注释
			{
				char ch1;
				ch1 = fgetc(fin);	//读下一个字符
				do
				{
					ch = ch1; ch1 = fgetc(fin);
				} while ((ch1 != '\n'));
			}
			else				//不是*也不是//则处理单分界符/
			{
				fseek(fin, -1L, SEEK_CUR);
				col--;
				token[0] = '/'; token[1] = '\0';
				tokeType = div;
				printf("5，%s，%d，%d\n", token, row, col-1);
			}
		}
		else//错误处理
		{
			tokeType = -2;
			printf("ERROR\n");
		}
	}
	
	return tokeType;  //返回主程序
}
/*
int main(int argc, char* argv[])
{
	int tokeType = 0;
	row = 1;
	errno_t err;
	err = fopen_s(&fin, PL0Scanin, "r");
	if (err != NULL) //判断输入文件名是否正确
	{
		printf("\n打开词法分析输入文件出错!\n");
		return 0;
	}
	while (1)
	{
		tokeType = getToken();//调词法分析
		if (tokeType == -1)	break;
	}
	fclose(fin);
	return 0;
}
*/