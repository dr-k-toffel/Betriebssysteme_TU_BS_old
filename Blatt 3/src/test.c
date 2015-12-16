#include "mymem.h"

int main() {
	int sa = 128;
	unsigned char* a = (unsigned char*)malloc(sa);
	for (int i = 0; i < sa; i++) {
		a[i] = 0xAA;
	}
	//printf("malloc a\n");
	printMem();

	int sb = 256;
	unsigned char* b = (unsigned char*)malloc(sb);
	for (int i = 0; i < sb; i++) {
		b[i] = 0xBB;
	}
	//printf("malloc b\n");
	printMem();

	free(a);
	//printf("free a\n");
	printMem();

	int sc = 64;
	unsigned char* c = (unsigned char*)malloc(sc);
	for (int i = 0; i < sc; i++) {
		c[i] = 0xCC;
	}
	//printf("malloc c\n");
	printMem();

	int sd = 128;
	unsigned char* d = (unsigned char*)malloc(sd);
	for (int i = 0; i < sd; i++) {
		d[i] = 0xDD;
	}
	//printf("malloc d\n");
	printMem();

	free(c);
	//printf("free c\n");
	printMem();

	free(d);
	//printf("free d\n");
	printMem();
	
	int se = 32, sec = 2;
	unsigned char* e = (unsigned char*)calloc(sec, se);
	for (int i = 0; i < sec*se; i++) {
		e[i] = 0xEE;
	}
	//printf("calloc e\n");
	printMem();
}
