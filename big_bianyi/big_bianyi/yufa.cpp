#include "data.h"

int sno = 0;                    //四元式序号
int temp = 0;                   //临时变量序号
extern FILE* fin;
int lookahead;
extern char token[256];
extern char PL0Scanin[255];
void Program();
void ProgramHead();
void VarDecpart();
void VarDecList();
void VarIdList();
void TypeName();
void ProgramBody();
void ProcDec();
void ParamList();
void Param();
void ProcBody();
void StmList();
void Stm();
void InputStm();
void OutputStm();
void CallStm(char x[]);
void AssignmentStm(char x[]);
void ConditionalStm();
void LoopStm();
void ActParamList();
Val Exp();
Val Term();
Val Factor();
void ConditionalExp();
void RelationExp();
void CompExp();

const char* err_msg[] =
{
	"",											//0
	"\t\t缺少：program，或program拼写错误！",	//1
	"\t\t缺少：var，或var拼写错误！",			//2
	"\t\t缺少：integer，或integer拼写错误！",	//3
	"\t\t缺少：float，或float拼写错误！",		//4
	"\t\t缺少：procedure，或procedure拼写错误！",//5
	"\t\t缺少：begin，或begin拼写错误！",		//6
	"\t\t缺少：end，或end拼写错误！",			//7
	"\t\t缺少：read，或read拼写错误！",			//8
	"\t\t缺少：write，或write拼写错误！",		//9
	"\t\t缺少：if，或if拼写错误！",		//10
	"\t\t缺少：then，或then拼写错误！",	//11
	"\t\t缺少：else，或else拼写错误！",	//12
	"\t\t缺少：fi，或fi拼写错误！",		//13
	"\t\t缺少：while，或while拼写错误！",//14
	"\t\t缺少：do，或do拼写错误！",		//15
	"\t\t缺少：endwh，或endwh拼写错误！",//16
	"\t\t缺少：end，或end拼写错误！",	//17
	"\t\t缺少：or，或or拼写错误！",		//18
	"\t\t缺少：标识符，或标识符拼写错误！",	//19
	"\t\t缺少：整数，或整数拼写错误！",	//20
	"\t\t缺少：小数，或小数拼写错误！",	//21
	"\t\t缺少：+，或+拼写错误！",		//22
	"\t\t缺少：-，或-拼写错误！",		//23
	"\t\t缺少：*，或*拼写错误！",		//24
	"\t\t缺少：/，或/拼写错误！",		//25
	"\t\t缺少：(，或(拼写错误！",		//26
	"\t\t缺少：)，或)拼写错误！",		//27
	"\t\t缺少：=，或=拼写错误！",		//28
	"\t\t缺少：,，或,拼写错误！",		//29
	"\t\t缺少：;，或;拼写错误！",		//30
	"\t\t缺少：>，或>拼写错误！",		//31
	"\t\t缺少：<，或<拼写错误！",		//32
	"\t\t缺少：<>，或<>拼写错误！",		//33
	"\t\t缺少：<=，或<=拼写错误！",		//34
	"\t\t缺少：>=，或>=拼写错误！",		//35
	"\t\t缺少：==，或==拼写错误！",		//36

	"\t\t程序尾部出现多余单词！",		//37
	"\t\t非法语句"						//38
};

void error(int n) {
	printf("%s\n", err_msg[n]);
}

void advance() {
	lookahead = getToken();
}

void match(int Tok) {
	if (lookahead != Tok) {
		error(Tok);
	}
	else
		advance();
}

/***总程序***/
//Program→ ProgramHead VarDecpart ProgramBody 
void Program(){
	advance();
	ProgramHead();
	VarDecpart();
	ProgramBody();

	if (lookahead != FEOF) {
		error(unexpectedFileEnd);
	}
}

/***总程序***/
//ProgramHead→ 'program' ID
void ProgramHead(){
	match(program);
	match(id);
}

/***变量声明***/
//VarDecpart→ ε| 'var' VarDecList
void VarDecpart(){
	if (lookahead == var){
		match(var);
		VarDecList();
	}
}

//VarDecList→ VarIdList {VarIdList}
void VarDecList(){
	VarIdList();
	while (lookahead==integer || lookahead== float) {
		VarIdList();
	}
}

//VarIdList→ TypeName ID {',' ID} ';'
void VarIdList(){
	TypeName();

	if (!enter(token))
		printf("%s重复定义！\n", token);
	match(id);
	
	while (lookahead == comma) {
		match(comma);
		if (!enter(token))
			printf("%s重复定义！\n", token);
		match(id);
		
	}
	match(semi);
}

//TypeName→'integer'| 'float'
void TypeName(){
	if (lookahead == integer)
		match(integer);
	else if (lookahead == float)
		match(float);
}

/***过程声明***/
//ProgramBody→ε| ProcDec{ ProcDec }
void ProgramBody(){
	ProcDec();
	while (lookahead==procedure) {
		ProcDec();
	}
}

/***过程声明***/
//ProcDec→ 'procedure' ID '(' ParamList ')' ';' VarDecpart ProcBody
void ProcDec(){
	match(procedure);
	match(id);
	match(LP);
	ParamList();
	match(RP);
	match(semi);
	VarDecpart();
	ProcBody();
}

/***形参声明***/
//ParamList→ ε| Param{ ';' Param }
void ParamList(){
	if (lookahead == integer || lookahead == float)	{
		Param();
		while (lookahead == semi) {
			advance();
			Param();
		}
	}
}

/***形参声明***/
//Param→ TypeName ID {',' ID} 
void Param(){
	TypeName();
	match(id);
	while (lookahead == comma){
		advance();
		match(id);
	}
}

/***过程体***/
//ProcBody→ 'begin' StmList 'end'
void ProcBody(){
	match(begin);
	StmList();
	match(end);
}

//StmList→ ε| Stm{ ';' Stm }
void StmList(){
	Stm();
	while (lookahead == semi) {
		advance();
		Stm();
	}
}

/***语句***/
//Stm→ConditionalStm| LoopStm| InputStm| OutputStm| CallStm| AssignmentStm
void Stm(){
	if (lookahead == IF)
		ConditionalStm();
	else if (lookahead == WHILE)
		LoopStm();
	else if (lookahead == read)
		InputStm();
	else if (lookahead == write)
		OutputStm();
	else if (lookahead == id)
	{
		char x[256];
		strcpy_s(x, token);
		match(id);
		if(lookahead == LP)
			CallStm(x);
		else if(lookahead == equ)
			AssignmentStm(x);
	}
}

//InputStm→'read' ID
void InputStm() {
	match(read);
	char x[256];
	strcpy_s(x, token);
	lookup(x);
	match(id);
	//生成四元式
	printf("\t\t\t\t%d （read, , ,", sno++);
	printf(" %s）\n", x);
}

//OutputStm→'write' Exp
void OutputStm() {
	match(write);
	Val v1 = Exp();
	printf("\t\t\t\t%d （write, , ,", sno++);

	if (v1.type == 1)
		printf("%s", getID(v1.value));
	else if (v1.type == 0)
		cout << v1.value;
	else
		printf("t%d）", (int)v1.value);
	cout << endl;
}

//CallStm→ ID '(' ActParamList ')'
void CallStm(char x[256]) {
	if (!enter(token))
		printf("%s重复定义！\n", token);

	match(LP);
	ActParamList();
	match(RP);
	printf("\t\t\t\t%d （call，%s， ， ）\n", sno++,x);
}

//AssignmentStm→ ID '=' Exp
void AssignmentStm(char x[256]) {

	if (lookup(x) < 0)
		printf("%s未定义！\n", x);
	match(equ);
	Val v = Exp();

	//生成四元式
	printf("\t\t\t\t%d （=，", sno++);
	v.type == 1 ? printf("%s", getID(v.value)) : v.type == 0 ? printf("%lf", v.value) : printf("t%d", (int)v.value);
	printf(", ，%s）\n", x);
}

//ConditionalStm→'if' ConditionalExp 'then' StmList 'else' StmList 'fi'
void ConditionalStm() {
	match(IF);
	ConditionalExp();
	match(THEN);
	StmList();
	match(ELSE);
	StmList();
	match(FI);
}

//LoopStm→'while' ConditionalExp 'do' StmList 'endwh'
void LoopStm() {
	match(WHILE);
	ConditionalExp();
	match(DO);
	StmList();
	match(ENDWH);
}

/***实参声明***/
//ActParamList→ ε| Exp{ ',' Exp }
void ActParamList(){
	if (lookahead == id || lookahead == digits ||
		lookahead == deci || lookahead == LP){
		Val v1 = Exp();
		while (lookahead == comma){
			advance();
			Val v2 = Exp();
		}
	}
}

/***四则表达式***/
//Exp→ Term {'+'|'-' Term}
Val Exp(){
	Val v1 = Term();
	while (lookahead == plus || lookahead == minus) {
		int op = lookahead;
		advance();
		//生成四元式
		Val v2 = Term();
		printf("\t\t\t\t%d （%c，", sno++, op == plus ? '+' : '-');

		if (v1.type == 1)		printf("%s，", getID(v1.value));
		else if (v1.type == 0)	cout << v1.value<<"，";
		else					printf("t%d，", (int)v1.value);

		if (v2.type == 1)		printf("%s", getID(v2.value));
		else if (v2.type == 0)	cout << v2.value;
		else					printf("t%d", (int)v2.value);
		
		printf("，t%d）\n", temp);
		v1.type = -1;
		v1.value = temp++;
	}
	return v1;
}

/***四则表达式***/
//Term→ Factor{ '*' | '/' Factor }
Val Term() {
	Val v1 = Factor();
	while (lookahead == mutiply || lookahead == div) {
		int op = lookahead;
		advance();
		Val v2 = Factor();
		//生成四元式
		printf("\t\t\t\t%d （%c，", sno++, op == mutiply ? '*' : '/');
		if (v1.type == 1)		printf("%s，", getID(v1.value));
		else if (v1.type == 0)	cout << v1.value << "，";
		else					printf("t%d，", (int)v1.value);
		if (v2.type == 1)		printf("%s", getID(v2.value));
		else if (v2.type == 0)	cout << v2.value;
		else					printf("t%d", (int)v2.value);
		printf("，t%d）\n", temp);
		v1.type = -1;
		v1.value = temp++;
	}
	return v1;
}

/***四则表达式***/
//Factor→ ID | INTC | DECI | '(' Exp ')'
Val Factor() {
	Val val;
	if (lookahead == id) {
		val.type = 1;
		val.value = lookup(token);
		if (val.value < 0) printf("%s未定义！\n", token);
		advance();
	}
	else if (lookahead == digits) {
		val.type = 0;
		val.value = atoi(token);
		advance();
	}
	else if (lookahead == deci) {
		val.type = 0;
		val.value = atof(token);
		advance();
	}
	else {
		match(LP);
		val = Exp();
		match(RP);
	}
	return val;
}

/***条件表达式***/
//ConditionalExp→RelationExp {'or' RelationExp}
void ConditionalExp() {
	RelationExp();
	while (lookahead == OR) {
		advance();
		RelationExp();
	}
}

/***条件表达式***/
//RelationExp→ CompExp {'and' CompExp}
void RelationExp() {
	CompExp();
	while (lookahead == AND) {
		advance();
		CompExp();
	}
}

/***条件表达式***/
//CompExp→ Exp CmpOp Exp
void CompExp() {
	Val v1 = Exp();
	int op = lookahead;

	if (lookahead == ge		|| lookahead == le		||
		lookahead == geeq	|| lookahead == leeq	||
		lookahead == lege	|| lookahead == eqeq	) 
	{
		advance();
	}
	Val v2 = Exp();
	//四元式翻译
	
	printf("\t\t\t\t%d （J%c，", sno++, op == ge ? '>' : '<');
	
	if (v1.type == 1)
		printf("%s，", getID(v1.value));
	else if (v1.type == 0)
		cout << v1.value << "，";
	else
		printf("t%d，", (int)v1.value);

	if (v2.type == 1)
		printf("%s", getID(v2.value));
	else if (v2.type == 0)
		cout << v2.value;
	else
		printf("t%d", (int)v2.value);
	
	printf("，___）\n");

	printf("\t\t\t\t%d （J， ， ，___)\n", sno++);

}

int main()
{
	errno_t err;
	err = fopen_s(&fin, PL0Scanin, "r");
	if (err != NULL) //判断输入文件名是否正确
	{
		printf("\n打开输入文件出错!\n");
		return 0;
	}
	Program();
	
	fclose(fin);

	finalcheck();

	return 0;
}