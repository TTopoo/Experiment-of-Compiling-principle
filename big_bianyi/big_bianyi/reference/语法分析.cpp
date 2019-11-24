#include "data.h"

int sno = 0;                    //��Ԫʽ���
int temp = 0;                   //��ʱ�������
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
	/*  1 */    "ȱ�٣�function����functionƴд����",
	/*  2 */    "ȱ�٣�begin����beginƴд����",
	/*  3 */    "ȱ�٣�end����endƴд����",
	/*  4 */    "ȱ�٣�integer����integerƴд����",
	/*  5 */    "ȱ�٣�if����ifƴд����",
	/*  6 */    "ȱ�٣�then����thenƴд����",
	/*  7 */    "ȱ�٣�else����elseƴд����",
	/*  8 */    "ȱ�٣�and����andƴд����",
	/*  9 */    "ȱ�٣�or����orƴд����",
	/* 10 */    "ȱ�٣���ʶ�������ʶ��ƴд����",
	/* 11 */    "ȱ�٣�������������ƴд����",
	/* 12 */    "ȱ�٣�+����+ƴд����",
	/* 13 */    "ȱ�٣�-����-ƴд����",
	/* 14 */    "ȱ�٣�*����*ƴд����",
	/* 15 */    "ȱ�٣�/����/ƴд����",
	/* 16 */    "ȱ�٣�(����(ƴд����",
	/* 17 */    "ȱ�٣�)��)д����",
	/* 18 */    "ȱ�٣�=����=ƴд����",
	/* 19 */    "ȱ�٣�����ƴд����",
	/* 20 */    "ȱ�٣�����ƴд����",
	/* 21 */    "ȱ�٣�>����>ƴд����",
	/* 22 */    "ȱ�٣�<����<ƴд����",
	/* 23 */    "����β�����ֶ��൥�ʣ�",
	/* 24 */    "�Ƿ����"
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

//<����>-> ���� [ ����]
//<����>-> function id ( ) begin <����б�> end
//<����б�>-> <���> [ ��� ]
//<���>-> <�������> | <��ֵ���> | <���ʽ>
//<���ʽ>-><��> [ +|- <��>]
//<��>-><����> [ *|/ <����>]
//<����>->id | digits | (  <���ʽ>  )
//<if>->if <�������ʽ> then begin <����б�> end { else begin <����б�> end }
//<�������ʽ>-> <������> [ OR <������>]
//<������>-> <�Ƚϱ��ʽ> [ AND <�Ƚϱ��ʽ>]
//<�Ƚϱ��ʽ>-> <���ʽ> <|> <���ʽ>



//<����>-> ���� [ ����]
void program() {

	func();

	while (lookahead == function){
		func();
	}

	if(lookahead!=FEOF) {
		error(unexpectedFileEnd);
	}
}

//<����>-> function id ( ) begin <����б�> end
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

//<����б�>-> <���> [ ��� ]
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

//<�������>-> integer id [ , id] ;
void declarartion(){

	match(integer);

	match(id);

	if (!enter(token))
		printf("%s�Ѷ��壬�����ظ����壡\n",token);

	while (lookahead == comma){
		advance();
		match(id);

		if (!enter(token))
			printf("%s�Ѷ��壬�����ظ����壡\n", token);
	}

	match(semi);

}

//<��ֵ���>-> id = <���ʽ> ;
void assignment(){

	match(id);

	char x[256];
	strcpy(x,token);

	if (lookup(token)<0 ) printf("%sδ���壡\n", token);

	match(equ);

	Val v=expression();

	match(semi);


	//������Ԫʽ
	printf("%3d ��=��",sno++);
	v.type == 1 ? printf("%s", getID(v.value)) : v.type == 0 ? printf("%d", v.value) : printf("t%d", v.value);
	printf("��%s��\n", x);

}

//<���ʽ>-><��> [ +|- <��>]
Val expression() {
	
	Val v1=term();

	while (lookahead == plus || lookahead == minus){
		
		int op = lookahead;

		advance();

		Val v2=term();

		//������Ԫʽ
		printf("%3d ��%c��", sno++, op == plus ? '+' : '-');
		v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
		printf("��");
		v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
		printf("��t%d��\n", temp);

		v1.type = -1;
		v1.value = temp++;


	}

	return v1;
}

//<��>-><����> [ +|- <����>]
Val term() {
	
	Val v1=factor();

	while (lookahead == mutiply || lookahead == div){
		
		int op = lookahead;

		advance();

		Val v2 = factor();

		//������Ԫʽ
		printf("%3d ��%c��", sno++, op == mutiply ? '*' : '/');
		v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
		printf("��");
		v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
		printf("��t%d��\n", temp);

		v1.type = -1;
		v1.value = temp++;

	}

	return v1;
}

//<����>->id | digits | (  <���ʽ>  )
Val factor(){
	Val val;
	if (lookahead == id) {
		val.type = 1;
		val.value = lookup(token);
		if (val.value<0) printf("%sδ���壡\n", token);

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

//<if>->if <�߼����ʽ> then begin <����б�> end { else begin <����б�> end }
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

//<�������ʽ>-> <������> [ OR <������>]
void logic(){

	conjunction();

	while (lookahead == OR){

		advance();

		conjunction();
	}
}

//<������>-> <�Ƚϱ��ʽ> [ AND <�Ƚϱ��ʽ>]
void conjunction() {

	comparison();

	while (lookahead == AND){

		advance();

		comparison();
	}
}

//<�Ƚϱ��ʽ>-> <���ʽ> <|> <���ʽ>
void comparison(){

	Val v1=expression();

	int op = lookahead;

	if (lookahead == ge || lookahead == le)	{
		advance();
	}

	Val v2=expression();

	//��Ԫʽ����
	printf("%3d ��J%c��", sno++, op == ge ? '>' : '<');
	v1.type == 1 ? printf("%s", getID(v1.value)) : v1.type == 0 ? printf("%d", v1.value) : printf("t%d", v1.value);
	printf("��");
	v2.type == 1 ? printf("%s", getID(v2.value)) : v2.type == 0 ? printf("%d", v2.value) : printf("t%d", v2.value);
	printf("��___��\n");

	printf("%3d ��J�� �� ��____)\n", sno++);


}
