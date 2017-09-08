/*
线性分析，差分分析专用
*/
#pragma once
#include "stdafx.h"
#include "myspn.h"

#define T_NUM   8000
#define LINEAR_SETKEY  0x1234567
//设置的密钥
class Linear :private MySPN {
public:
	Linear();
	~Linear();
	void analyse();
protected:
	struct Analyse_text {
		bool state;
		unsigned short plaintext;
		unsigned short crytext;
	}An_text[8000];
private:
	void Hex57();
	static unsigned long Linear_setkey;
	inline void hex2char(char x, char *y);
	bool creattest();
	static void creattest_run(LPVOID arguement);
	char maxKey5, maxKey7;
};

class Differential {
public:
private:
};

#undef T