#include "data.h"

comtab table[TXMAX];  //·ûºÅ±í

bool enter(char name[])
{
	int i;
	for (i = 0; i < TXMAX; i++){
		if (!table[i].used) 
			break;

		if (!strcmp(table[i].name, name))
			return false;
	}
	
	strcpy(table[i].name, name);
	table[i].used = true;

	return true;
}

int lookup(char name[])
{
	int i=0;
	for ( ; i < TXMAX; i++){
		if (!table[i].used)
			break;

		if (!strcmp(table[i].name, name))
			return i;
	}

	return -1;
}

char * getID(int index)
{
	return table[index].name;
}


