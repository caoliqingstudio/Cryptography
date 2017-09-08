/*
线性分析，差分分析专用
*/
#pragma once
#include "stdafx.h"
#include "myspn.h"

#define T_NUM   10000
#define LINEAR_SETKEY  0xab851257
#define DIFF_SETKEY  0xab851257
#define T_DIFF_NUM 1000
#define THREAD_NUM 8
#define THREAD_ADD 0x1fffff
#define THREAD_MUL 0x200000
//设置的密钥

struct keyStore {
	unsigned long key;
	struct keyStore *next;
};

class Linear :protected MySPN {
public:
	Linear();
	~Linear();
	unsigned long Linear_setKey;
	void analyse();
private:
	bool Hex();
	inline void hex2char(unsigned char x, char *y);
	inline void sboxDecrypt_4(char *input);
	char maxKey[8];
	int num;//合适密钥数量
	struct keyStore *An_key;
	FILE *file;
	bool creatFile(unsigned long key);
	bool bigRun();//暴力破解
};

class Differential{
public:
	Differential();
	~Differential();
	unsigned long Diff_setKey;
	struct keyArgueDif
	{
		int num;
		unsigned short thisplaintext, thiscrytext;
		unsigned short thisplaintext2, thiscrytext2;
		char maxKey[8];
		bool  state;
		int start, end;
		unsigned long key[5];
	}threadArgue[THREAD_NUM];
	void analyse();
private:
	int textNum;
	inline void sboxDecrypt_4(char *input);
	bool hex();
	char maxKey[8];
	bool bigRun();
	static void bigRunThread(LPVOID argu);
};