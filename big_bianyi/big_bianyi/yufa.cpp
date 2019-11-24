#include "data.h"

int sno = 0;                    //��Ԫʽ���
int temp = 0;                   //��ʱ�������
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
	"\t\tȱ�٣�program����programƴд����",	//1
	"\t\tȱ�٣�var����varƴд����",			//2
	"\t\tȱ�٣�integer����integerƴд����",	//3
	"\t\tȱ�٣�float����floatƴд����",		//4
	"\t\tȱ�٣�procedure����procedureƴд����",//5
	"\t\tȱ�٣�begin����beginƴд����",		//6
	"\t\tȱ�٣�end����endƴд����",			//7
	"\t\tȱ�٣�read����readƴд����",			//8
	"\t\tȱ�٣�write����writeƴд����",		//9
	"\t\tȱ�٣�if����ifƴд����",		//10
	"\t\tȱ�٣�then����thenƴд����",	//11
	"\t\tȱ�٣�else����elseƴд����",	//12
	"\t\tȱ�٣�fi����fiƴд����",		//13
	"\t\tȱ�٣�while����whileƴд����",//14
	"\t\tȱ�٣�do����doƴд����",		//15
	"\t\tȱ�٣�endwh����endwhƴд����",//16
	"\t\tȱ�٣�end����endƴд����",	//17
	"\t\tȱ�٣�or����orƴд����",		//18
	"\t\tȱ�٣���ʶ�������ʶ��ƴд����",	//19
	"\t\tȱ�٣�������������ƴд����",	//20
	"\t\tȱ�٣�С������С��ƴд����",	//21
	"\t\tȱ�٣�+����+ƴд����",		//22
	"\t\tȱ�٣�-����-ƴд����",		//23
	"\t\tȱ�٣�*����*ƴд����",		//24
	"\t\tȱ�٣�/����/ƴд����",		//25
	"\t\tȱ�٣�(����(ƴд����",		//26
	"\t\tȱ�٣�)����)ƴд����",		//27
	"\t\tȱ�٣�=����=ƴд����",		//28
	"\t\tȱ�٣�,����,ƴд����",		//29
	"\t\tȱ�٣�;����;ƴд����",		//30
	"\t\tȱ�٣�>����>ƴд����",		//31
	"\t\tȱ�٣�<����<ƴд����",		//32
	"\t\tȱ�٣�<>����<>ƴд����",		//33
	"\t\tȱ�٣�<=����<=ƴд����",		//34
	"\t\tȱ�٣�>=����>=ƴд����",		//35
	"\t\tȱ�٣�==����==ƴд����",		//36

	"\t\t����β�����ֶ��൥�ʣ�",		//37
	"\t\t�Ƿ����"						//38
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

/***�ܳ���***/
//Program�� ProgramHead VarDecpart ProgramBody 
void Program(){
	advance();
	ProgramHead();
	VarDecpart();
	ProgramBody();

	if (lookahead != FEOF) {
		error(unexpectedFileEnd);
	}
}

/***�ܳ���***/
//ProgramHead�� 'program' ID
void ProgramHead(){
	match(program);
	match(id);
}

/***��������***/
//VarDecpart�� ��| 'var' VarDecList
void VarDecpart(){
	if (lookahead == var){
		match(var);
		VarDecList();
	}
}

//VarDecList�� VarIdList {VarIdList}
void VarDecList(){
	VarIdList();
	while (lookahead==integer || lookahead== float) {
		VarIdList();
	}
}

//VarIdList�� TypeName ID {',' ID} ';'
void VarIdList(){
	TypeName();

	if (!enter(token))
		printf("%s�ظ����壡\n", token);
	match(id);
	
	while (lookahead == comma) {
		match(comma);
		if (!enter(token))
			printf("%s�ظ����壡\n", token);
		match(id);
		
	}
	match(semi);
}

//TypeName��'integer'| 'float'
void TypeName(){
	if (lookahead == integer)
		match(integer);
	else if (lookahead == float)
		match(float);
}

/***��������***/
//ProgramBody����| ProcDec{ ProcDec }
void ProgramBody(){
	ProcDec();
	while (lookahead==procedure) {
		ProcDec();
	}
}

/***��������***/
//ProcDec�� 'procedure' ID '(' ParamList ')' ';' VarDecpart ProcBody
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

/***�β�����***/
//ParamList�� ��| Param{ ';' Param }
void ParamList(){
	if (lookahead == integer || lookahead == float)	{
		Param();
		while (lookahead == semi) {
			advance();
			Param();
		}
	}
}

/***�β�����***/
//Param�� TypeName ID {',' ID} 
void Param(){
	TypeName();
	match(id);
	while (lookahead == comma){
		advance();
		match(id);
	}
}

/***������***/
//ProcBody�� 'begin' StmList 'end'
void ProcBody(){
	match(begin);
	StmList();
	match(end);
}

//StmList�� ��| Stm{ ';' Stm }
void StmList(){
	Stm();
	while (lookahead == semi) {
		advance();
		Stm();
	}
}

/***���***/
//Stm��ConditionalStm| LoopStm| InputStm| OutputStm| CallStm| AssignmentStm
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

//InputStm��'read' ID
void InputStm() {
	match(read);
	char x[256];
	strcpy_s(x, token);
	lookup(x);
	match(id);
	//������Ԫʽ
	printf("\t\t\t\t%d ��read, , ,", sno++);
	printf(" %s��\n", x);
}

//OutputStm��'write' Exp
void OutputStm() {
	match(write);
	Val v1 = Exp();
	printf("\t\t\t\t%d ��write, , ,", sno++);

	if (v1.type == 1)
		printf("%s", getID(v1.value));
	else if (v1.type == 0)
		cout << v1.value;
	else
		printf("t%d��", (int)v1.value);
	cout << endl;
}

//CallStm�� ID '(' ActParamList ')'
void CallStm(char x[256]) {
	if (!enter(token))
		printf("%s�ظ����壡\n", token);

	match(LP);
	ActParamList();
	match(RP);
	printf("\t\t\t\t%d ��call��%s�� �� ��\n", sno++,x);
}

//AssignmentStm�� ID '=' Exp
void AssignmentStm(char x[256]) {

	if (lookup(x) < 0)
		printf("%sδ���壡\n", x);
	match(equ);
	Val v = Exp();

	//������Ԫʽ
	printf("\t\t\t\t%d ��=��", sno++);
	v.type == 1 ? printf("%s", getID(v.value)) : v.type == 0 ? printf("%lf", v.value) : printf("t%d", (int)v.value);
	printf(", ��%s��\n", x);
}

//ConditionalStm��'if' ConditionalExp 'then' StmList 'else' StmList 'fi'
void ConditionalStm() {
	match(IF);
	ConditionalExp();
	match(THEN);
	StmList();
	match(ELSE);
	StmList();
	match(FI);
}

//LoopStm��'while' ConditionalExp 'do' StmList 'endwh'
void LoopStm() {
	match(WHILE);
	ConditionalExp();
	match(DO);
	StmList();
	match(ENDWH);
}

/***ʵ������***/
//ActParamList�� ��| Exp{ ',' Exp }
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

/***������ʽ***/
//Exp�� Term {'+'|'-' Term}
Val Exp(){
	Val v1 = Term();
	while (lookahead == plus || lookahead == minus) {
		int op = lookahead;
		advance();
		//������Ԫʽ
		Val v2 = Term();
		printf("\t\t\t\t%d ��%c��", sno++, op == plus ? '+' : '-');

		if (v1.type == 1)		printf("%s��", getID(v1.value));
		else if (v1.type == 0)	cout << v1.value<<"��";
		else					printf("t%d��", (int)v1.value);

		if (v2.type == 1)		printf("%s", getID(v2.value));
		else if (v2.type == 0)	cout << v2.value;
		else					printf("t%d", (int)v2.value);
		
		printf("��t%d��\n", temp);
		v1.type = -1;
		v1.value = temp++;
	}
	return v1;
}

/***������ʽ***/
//Term�� Factor{ '*' | '/' Factor }
Val Term() {
	Val v1 = Factor();
	while (lookahead == mutiply || lookahead == div) {
		int op = lookahead;
		advance();
		Val v2 = Factor();
		//������Ԫʽ
		printf("\t\t\t\t%d ��%c��", sno++, op == mutiply ? '*' : '/');
		if (v1.type == 1)		printf("%s��", getID(v1.value));
		else if (v1.type == 0)	cout << v1.value << "��";
		else					printf("t%d��", (int)v1.value);
		if (v2.type == 1)		printf("%s", getID(v2.value));
		else if (v2.type == 0)	cout << v2.value;
		else					printf("t%d", (int)v2.value);
		printf("��t%d��\n", temp);
		v1.type = -1;
		v1.value = temp++;
	}
	return v1;
}

/***������ʽ***/
//Factor�� ID | INTC | DECI | '(' Exp ')'
Val Factor() {
	Val val;
	if (lookahead == id) {
		val.type = 1;
		val.value = lookup(token);
		if (val.value < 0) printf("%sδ���壡\n", token);
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

/***�������ʽ***/
//ConditionalExp��RelationExp {'or' RelationExp}
void ConditionalExp() {
	RelationExp();
	while (lookahead == OR) {
		advance();
		RelationExp();
	}
}

/***�������ʽ***/
//RelationExp�� CompExp {'and' CompExp}
void RelationExp() {
	CompExp();
	while (lookahead == AND) {
		advance();
		CompExp();
	}
}

/***�������ʽ***/
//CompExp�� Exp CmpOp Exp
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
	//��Ԫʽ����
	
	printf("\t\t\t\t%d ��J%c��", sno++, op == ge ? '>' : '<');
	
	if (v1.type == 1)
		printf("%s��", getID(v1.value));
	else if (v1.type == 0)
		cout << v1.value << "��";
	else
		printf("t%d��", (int)v1.value);

	if (v2.type == 1)
		printf("%s", getID(v2.value));
	else if (v2.type == 0)
		cout << v2.value;
	else
		printf("t%d", (int)v2.value);
	
	printf("��___��\n");

	printf("\t\t\t\t%d ��J�� �� ��___)\n", sno++);

}

int main()
{
	errno_t err;
	err = fopen_s(&fin, PL0Scanin, "r");
	if (err != NULL) //�ж������ļ����Ƿ���ȷ
	{
		printf("\n�������ļ�����!\n");
		return 0;
	}
	Program();
	
	fclose(fin);

	finalcheck();

	return 0;
}