#include "stdafx.h"
#include "myspn.h"

MySPN::MySPN()
{
	for (int i = 0; i < 5; ++i) {
		threadArgu.roundKey[i].state = false;
	}
}

void MySPN::setKey(char * key)
{
	unsigned char temp,stemp;
	unsigned char thisKey[4];
	for (int i = 0; i < 4; i++)
	{
		temp = key[2*i+1];
		temp = (temp >= '0'&&temp <= '9') ? temp - '0' : 
			(temp >= 'A'&&temp <= 'F') ? temp - 'A' + 10 : temp - 'a' + 10;
		stemp = temp & 0x0f;
		temp = key[2 * i];
		temp = (temp >= '0'&&temp <= '9') ? temp - '0' :
			(temp >= 'A'&&temp <= 'F') ? temp - 'A' + 10 : temp - 'a' + 10;
		stemp = (stemp << 4) | (temp & 0x0f);
		thisKey[i] = stemp;
	}
	threadArgu.inputKey = *((unsigned long *)thisKey);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)roundKeyCreat,(LPVOID)&threadArgu, 0, NULL);
}

void MySPN::setKey(unsigned long key)
{
	threadArgu.inputKey = key;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)roundKeyCreat, (LPVOID)&threadArgu, 0, NULL);
}

unsigned short MySPN::encrypt16(unsigned short plaintext)
{
	return 0;
}

unsigned short MySPN::encrypt16(char * plaintext)
{
	return 0;
}

MySPN::~MySPN()
{
}

void MySPN::roundKeyCreat(LPVOID argu)
{
	unsigned long temp=((struct argumKey*)argu)->inputKey;
	for (int i = 1; i <= 5; ++i) {
		((struct argumKey *)argu)->roundKey[i - 1].key = (unsigned short)temp;
		((struct argumKey *)argu)->roundKey[i - 1].state = true;
		//std::cout <<std::hex<<(unsigned short)temp<<std::endl;//测试参数传递是否正确
		temp = temp >> 4;
	}
	return;
}
