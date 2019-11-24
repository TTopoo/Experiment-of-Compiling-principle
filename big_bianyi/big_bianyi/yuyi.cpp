#include "data.h"
comtab table[TXMAX];  //���ű�
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
		if (!table[i].used)//û�ҵ�
			break;
		if (!strcmp(table[i].name, name)){//�ҵ���
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
			printf("%sδʹ�ã�\n", table[i].name);
	}
}


