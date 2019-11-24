#include "data.h"

int sno = 0;                    //四元式序号
int temp = 0;                   //临时变量序号
extern FILE * fp;
extern int lookahead;
extern char token[256];

void program();
void func();
void statementlists();
void declarartion();
void assignment();
Val  expression();
void statement();
Val  term();
Val  factor();
void if_Statement();
void logic();
void conjunction();
void comparison();



char* err_msg[] =
{
	/*  0 */    "",
	/*  1 */    "缺少：function，或function拼写错误！",
	/*  2 */    "缺少：begin，或begin拼写错误！",
	/*  3 */    "缺少：end，或end拼写错误！",
	/*  4 */    "缺少：integer，或integer拼写错误！",
	/*  5 */    "缺少：if，或if拼写错误！",
	/*  6 */    "缺少：then，或then拼写错误！",
	/*  7 */    "缺少：else，或else拼写错误！",
	/*  8 */    "缺少：and，或and拼写错误！",
	/*  9 */    "缺少：or，或or拼写错误！",
	/* 10 */    "缺少：标识符，或标识符拼写错误！",
	/* 11 */    "缺少：整数，或整数拼写错误！",
	/* 12 */    "缺少：+，或+拼写错误！",
	/* 13 */    "缺少：-，或-拼写错误！",
	/* 14 */    "缺少：*，或*拼写错误！",
	/* 15 */    "缺少：/，或/拼写错误！",
	/* 16 */    "缺少：(，或(拼写错误！",
	/* 17 */    "缺少：)，)写错误！",
	/* 18 */    "缺少：=，或=拼写错误！",
	/* 19 */    "缺少：，或，拼写错误！",
	/* 20 */    "缺少：；或；拼写错误！",
	/* 21 */    "缺少：>，或>拼写错误！",
	/* 22 */    "缺少：<；或<拼写错误！",
	/* 23 */    "程序尾部出现多余单词！",
	/* 24 */    "非法语句"
};

void error(int n) {
	printf("%s\n", err_msg[n]);
	fclose(fp);
	getchar();
	exit(0);
}

void advance() {
	lookahead = getToken();
}

void match(int Tok) {
	if (lookahead != Tok) {
		error(Tok);
	}
	advance();
}

//<程序>-> 函数 [ 函数]
//<函数>-> function id ( ) begin <语句列表> end
//<语句列表>-> <语句> [ 语句 ]
//<语句>-> <声明语句> | <赋值语句> | <表达式>
//<表达式>-><项> [ +|- <项>]
//<项>-><因子> [ *|/ <因子>]
//<因子>->id | digits | (  <表达式>  )
//<if>->if <布尔表达式> then begin <语句列表> end { else begin <语句列表> end }
//<布尔表达式>-> <与运算> [ OR <与运算>]
//<与运算>-> <比较表达式> [ AND <比较表达式>]
//<比较表达式>-> <表达式> <|> <表达式>



//<程序>-> 函数 [ 函数]
void program() {

	func();

	while (lookahead == function){
		func();
	}

	if(lookahead!=FEOF) {
		error(unexpectedFileEnd);
	}
}

//<函数>-> function id ( ) begin <语句列表> end
void func()
{
	match(function);

	match(id);

	match(LP);

	match(RP);

	match(begin);

	statementlists();

	match(end);
}

//<语句列表>-> <语句> [ 语句 ]
void statementlists()
{
	statement();

	while (lookahead == integer || lookahead == id || lookahead == IF ){
		statement();
	}

}

void statement()
{
	if (lookahead == integer)
		declarartion();
	else if (lookahead == id)
		assignment();
	else if (lookahead == IF)
		if_Statement();
}

//<声明语句>-> integer id [ , id] ;
void declarartion(){

	match(integer);

	match(id);

	if (!enter(token))
		printf("%s已定义，不能重复定义！\n",token);

	while (lookahead == comma){
		advance();
		match(id);

		if (!enter(token))
			printf("%s已定义，不能重复定义！\n", token);
	}

	match(semi);

}

//<赋值语句>-> id = <表达式> ;
void assignment(){

	match(id);

	char x[256];
	strcpy(x,token);

	if (lookup(token)<0 ) printf("%s未定义！\n", token);

	match(equ);

	Val v=expression();

	match(semi);


	//生成四元式
	printf("%3d （=，",sno++);
	v.type == 1 ? printf("%s", getID(v.value)) : v.type == 0 ? printf("%d", v.value) : printf("t%d", v.value);
	printf("，%s）\n", x);

}

//<表达式>-><项> [ +|- <项>]
Val expression() {
	
	Val v1=term();

	while (lookahead == plus || lookahead == minus){
		
		int op = lookahead;

		advance();

		Val v2=term();

		//生成四元式
		printf("%3d （%c，", sno++, op == plus ? '+' : '-');
		v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
		printf("，");
		v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
		printf("，t%d）\n", temp);

		v1.type = -1;
		v1.value = temp++;


	}

	return v1;
}

//<项>-><因子> [ +|- <因子>]
Val term() {
	
	Val v1=factor();

	while (lookahead == mutiply || lookahead == div){
		
		int op = lookahead;

		advance();

		Val v2 = factor();

		//生成四元式
		printf("%3d （%c，", sno++, op == mutiply ? '*' : '/');
		v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
		printf("，");
		v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
		printf("，t%d）\n", temp);

		v1.type = -1;
		v1.value = temp++;

	}

	return v1;
}

//<因子>->id | digits | (  <表达式>  )
Val factor(){
	Val val;
	if (lookahead == id) {
		val.type = 1;
		val.value = lookup(token);
		if (val.value<0) printf("%s未定义！\n", token);

		advance();
	}
	else if (lookahead == digits) {
		val.type = 0;
		val.value = atoi(token);

		advance();
	}
	else {
		match(LP);

		val = expression();

		match(RP);
	}

	return val;

}

//<if>->if <逻辑表达式> then begin <语句列表> end { else begin <语句列表> end }
void if_Statement(){
	
	match(IF);

	logic();

	match(THEN);
	
	match(begin);

	statementlists();

	match(end);

	if (lookahead == ELSE){
		
		advance();
		
		match(begin);

		statementlists();

		match(end);
	}
}

//<布尔表达式>-> <与运算> [ OR <与运算>]
void logic(){

	conjunction();

	while (lookahead == OR){

		advance();

		conjunction();
	}
}

//<与运算>-> <比较表达式> [ AND <比较表达式>]
void conjunction() {

	comparison();

	while (lookahead == AND){

		advance();

		comparison();
	}
}

//<比较表达式>-> <表达式> <|> <表达式>
void comparison(){

	Val v1=expression();

	int op = lookahead;

	if (lookahead == ge || lookahead == le)	{
		advance();
	}

	Val v2=expression();

	//四元式翻译
	printf("%3d （J%c，", sno++, op == ge ? '>' : '<');
	v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
	printf("，");
	v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
	printf("，___）\n");

	printf("%3d （J， ， ，____)\n", sno++);


}
