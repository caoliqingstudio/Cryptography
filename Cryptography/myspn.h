#pragma once
#include "stdafx.h"
#include <windows.h>

//#define NumRound 4

class MySPN {
public:
	MySPN();//构造函数
	void setKey(char * key);
	void setKey(unsigned long key);
	unsigned short encrypt16(unsigned short plaintext);
	unsigned short encrypt16(char * plaintext);
	~MySPN();
	//析构函数，用于释放内存
private:
	static void roundKeyCreat(LPVOID argu);//轮密钥生成，此处打算另开一个线程运行
	struct argumKey {
		unsigned long inputKey;
		struct storeKey
		{
			unsigned short key;
			bool state;
		} roundKey[5];//存储轮密钥
	} threadArgu;
	HANDLE hThread;
};