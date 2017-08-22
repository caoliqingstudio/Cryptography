#include "stdafx.h"
#include "myspn.h"

MySPN::MySPN()
{
	for (int i = 0; i < 5; ++i) {
		threadArgu.roundKey[i].state = false;
	}
	crytext[4] = '\0';
	sbox_c[0] = 0x07;
	sbox_c[1] = 0x0c;
	sbox_c[2] = 0x04;
	sbox_c[3] = 0x0a;
	sbox_c[4] = 0x0b;
	sbox_c[5] = 0x06;
	sbox_c[6] = 0x0d;
	sbox_c[7] = 0x00;
	sbox_c[8] = 0x02;
	sbox_c[9] = 0x05;
	sbox_c[10] = 0x0f;
	sbox_c[11] = 0x09;
	sbox_c[12] = 0x08;
	sbox_c[13] = 0x03;
	sbox_c[14] = 0x01;
	sbox_c[15] = 0x0e;
/*	sbox_c[0] = 0x0e;//s盒子理解有误
	sbox_c[1] = 0x04;
	sbox_c[2] = 0x0d;
	sbox_c[3] = 0x01;
	sbox_c[4] = 0x02;
	sbox_c[5] = 0x0f;
	sbox_c[6] = 0x0b;
	sbox_c[7] = 0x08;
	sbox_c[8] = 0x03;
	sbox_c[9] = 0x0a;
	sbox_c[10] = 0x06;
	sbox_c[11] = 0x0c;
	sbox_c[12] = 0x05;
	sbox_c[13] = 0x09;
	sbox_c[14] = 0x00;
	sbox_c[15] = 0x07;*/
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

unsigned short MySPN::encrypt16(unsigned short plaintext_s)
{
	int i;
	unsigned short temp[4] = { 0 };
	plaintext[0] = plaintext_s & 0x000f;
	plaintext[1] = (plaintext_s >> 4) & 0x000f;
	plaintext[2] = (plaintext_s >> 8) & 0x000f;
	plaintext[3] = (plaintext_s >> 12) & 0x000f;
	encrypt16_hex(plaintext);
	temp[0] = plaintext[0];
	temp[1] = plaintext[1];
	temp[1] = temp[1] << 4;
	temp[2] = plaintext[2];
	temp[2] = temp[2] << 8;
	temp[3] = plaintext[3];
	temp[3] = temp[3] << 12;
	return temp[0]|temp[1]|temp[2]|temp[3];
}

unsigned char* MySPN::encrypt16(unsigned char * plaintext_c) {
	plaintext[0] = c2hc(plaintext_c[0]);
	plaintext[1] = c2hc(plaintext_c[1]);
	plaintext[2] = c2hc(plaintext_c[2]);
	plaintext[3] = c2hc(plaintext_c[3]);
	return encrypt16_hex(plaintext);
}

//仅为 查分 线性分析而用
void MySPN::setKey_01(char * key)
{
	unsigned long keynum = 0;
	unsigned long temp = 1;
	for (int i = 0; i < 32; ++i) {
		keynum = keynum | (key[i] == '0' ? 0 : temp);
		temp *= 2;
	}
	setKey(keynum);
}

//仅为 查分 线性分析而用
char * MySPN::encrypt16_01(char * plaintext_01)
{
	for (int i = 0; i < 4; i++)
	{
		plaintext[i] = (plaintext_01[i * 4] == '0' ? 0 : 1);
		plaintext[i] += (plaintext_01[i * 4 + 1] == '0' ? 0 : 2);
		plaintext[i] += (plaintext_01[i * 4 + 2] == '0' ? 0 : 4);
		plaintext[i] += (plaintext_01[i * 4 + 3] == '0' ? 0 : 8);
	}
	encrypt16_hex(plaintext);
	for (int i = 0; i < 4; i++)
	{
		crytext_01[i * 4 + 0] = ((crytext[i] & 0x01) != 0 ? 1 : 0);
		crytext_01[i * 4 + 1] = ((crytext[i] & 0x02) != 0 ? 1 : 0);
		crytext_01[i * 4 + 2] = ((crytext[i] & 0x04) != 0 ? 1 : 0);
		crytext_01[i * 4 + 3] = ((crytext[i] & 0x08) != 0 ? 1 : 0);
/*		crytext_01[i * 4 + 0] = ((crytext[i] & 0x01) != 0 ? '1' : '0');//方便看，所以弄成字符
		crytext_01[i * 4 + 1] = ((crytext[i] & 0x02) != 0 ? '1' : '0');
		crytext_01[i * 4 + 2] = ((crytext[i] & 0x04) != 0 ? '1' : '0');
		crytext_01[i * 4 + 3] = ((crytext[i] & 0x08) != 0 ? '1' : '0');*/
	}
	crytext_01[16] = '\0';
	return crytext_01;
}

unsigned char* MySPN::encrypt16_hex(unsigned char * plaintext_c)
{
	crytext[0] = plaintext_c[0];
	crytext[1] = plaintext_c[1];
	crytext[2] = plaintext_c[2];
	crytext[3] = plaintext_c[3];
	for (int i = 0; i < 3; i++)
	{
		while (threadArgu.roundKey[i].state == false);
		xor16(threadArgu.roundKey[i].keyChar, crytext);
		sbox16(crytext);
		pbox16(crytext);
	}
	while (threadArgu.roundKey[3].state == false);
	xor16(threadArgu.roundKey[3].keyChar, crytext);
	sbox16(crytext);
	while (threadArgu.roundKey[4].state == false);
	xor16(threadArgu.roundKey[4].keyChar, crytext);
	return crytext;
}

MySPN::~MySPN()
{
	for (int i = 0; i < 5; ++i) {
		threadArgu.roundKey[i].state = false;
	}
}

void MySPN::roundKeyCreat(LPVOID argu)
{
	unsigned char * keypath;
	unsigned long temp=((struct argumKey*)argu)->inputKey;
	for (int i = 1; i <= 5; ++i) {
		keypath = &((struct argumKey *)argu)->roundKey[i - 1].keyChar[0];
		keypath[0] = temp & 0x000F;
		keypath[1] = (temp >> 4) & 0x000F;
		keypath[2] = (temp >> 8) & 0x000F;
		keypath[3] = (temp >> 12) & 0x000F;
		((struct argumKey *)argu)->roundKey[i - 1].key = (unsigned short)temp;
		((struct argumKey *)argu)->roundKey[i - 1].state = true;
		//std::cout <<std::hex<<(unsigned short)temp<<std::endl;//测试参数传递是否正确
		temp = temp >> 4;
	}
	return;
}

inline void MySPN::pbox16(unsigned char * text)
{
	unsigned char temp[4];
	temp[0] = ((text[0] & 0x01) > 0 ? 1 : 0) + ((text[1] & 0x01) > 0 ? 2 : 0)
		+ ((text[2] & 0x01) > 0 ? 4 : 0) + ((text[3] & 0x01) > 0 ? 8 : 0);
	temp[1] = ((text[0] & 0x02) > 0 ? 1 : 0) + ((text[1] & 0x02) > 0 ? 2 : 0)
		+ ((text[2] & 0x02) > 0 ? 4 : 0) + ((text[3] & 0x02) > 0 ? 8 : 0);
	temp[2] = ((text[0] & 0x04) > 0 ? 1 : 0) + ((text[1] & 0x04) > 0 ? 2 : 0)
		+ ((text[2] & 0x04) > 0 ? 4 : 0) + ((text[3] & 0x04) > 0 ? 8 : 0);
	temp[3] = ((text[0] & 0x08) > 0 ? 1 : 0) + ((text[1] & 0x08) > 0 ? 2 : 0)
		+ ((text[2] & 0x08) > 0 ? 4 : 0) + ((text[3] & 0x08) > 0 ? 8 : 0);
	text[0] = temp[0];
	text[1] = temp[1];
	text[2] = temp[2];
	text[3] = temp[3];
}
