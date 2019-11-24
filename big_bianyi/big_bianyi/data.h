#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <string>
#include <conio.h>
using namespace std;

#define keywordSum				19     // number of reserved words
#define TXMAX					233    // length of identifier table
#define MAXIDLEN				20     // length of identifiers


#define FEOF                   -1
#define ERROR                  -2

#define program					1
#define var						2
#define integer					3
#define float					4
#define procedure				5
#define begin					6
#define end						7
#define read					8
#define write					9

#define IF						10
#define THEN					11
#define ELSE					12
#define FI						13
#define WHILE					14
#define DO						15
#define ENDWH					16
#define AND						17
#define OR						18

#define id						19//标识符
#define digits					20//整数
#define deci					21//小数
#define plus					22//+
#define minus					23//-
#define mutiply					24//*
#define div						25//
#define LP						26//(
#define RP						27//)
#define equ						28//=
#define comma					29//,
#define semi					30//;
#define ge						31//>
#define le						32//<
#define lege					33//<>
#define leeq					34//<=
#define geeq					35//>=
#define eqeq					36//==


#define unexpectedFileEnd		37
#define unexpectedStatement		38
typedef struct                      //符号表数据结构
{
	char name[MAXIDLEN + 1];
	bool used;
	bool trueuesd = 0;
} comtab;

typedef struct
{
	double value;
	int  type;  // 1：表示value中保存的是标识符在符号表中的索引；
				// 0：表示value中保存的是常量值；
				//-1：表示value中保存的是临时变量序号；
} Val;


extern void Program();
extern int getToken();
extern char* getID(int index);
extern bool enter(char name[]);
extern int lookup(char name[]);
extern void finalcheck();

