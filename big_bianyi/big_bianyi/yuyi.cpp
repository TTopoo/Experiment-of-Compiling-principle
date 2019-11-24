#include "data.h"
comtab table[TXMAX];  //符号表
bool enter(char name[]){
	int i;
	for (i = 0; i < TXMAX; i++) {
		if (!table[i].used)
			break;
		if (!strcmp(table[i].name, name))
			return false;
	}
	strcpy_s(table[i].name, name);
	table[i].used = true;
	return true;
}

int lookup(char name[]){
	for (int i = 0; i < TXMAX; i++)	{
		if (!table[i].used)//没找到
			break;
		if (!strcmp(table[i].name, name)){//找到了
			table[i].trueuesd = 1;
			return i;
		}
	}
	return -1;
}

char* getID(int index){
	return table[index].name;
}

void finalcheck(){
	for (int i = 0; i < TXMAX; i++) {
		if (table[i].used && !table[i].trueuesd)
			printf("%s未使用！\n", table[i].name);
	}
}


