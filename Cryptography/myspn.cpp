#include "stdafx.h"
#include "myspn.h"

MySPN::MySPN()
{
	sboxDecrypt_c[0][0] = 1; sboxDecrypt_c[0][1] = 1; sboxDecrypt_c[0][2] = 1; sboxDecrypt_c[0][3] = 0;
	sboxDecrypt_c[2][0] = 0; sboxDecrypt_c[2][1] = 1; sboxDecrypt_c[2][2] = 0; sboxDecrypt_c[2][3] = 0;
	sboxDecrypt_c[9][0] = 1; sboxDecrypt_c[9][1] = 1; sboxDecrypt_c[9][2] = 0; sboxDecrypt_c[9][3] = 1; 
	sboxDecrypt_c[4][0] = 0; sboxDecrypt_c[4][1] = 0; sboxDecrypt_c[4][2] = 0; sboxDecrypt_c[4][3] = 1;
	
	sboxDecrypt_c[13][0] = 0; sboxDecrypt_c[13][1] = 0; sboxDecrypt_c[13][2] = 1; sboxDecrypt_c[13][3] = 0;
	sboxDecrypt_c[7][0] = 1; sboxDecrypt_c[7][1] = 1; sboxDecrypt_c[7][2] = 1; sboxDecrypt_c[7][3] = 1;
	sboxDecrypt_c[12][0] = 1; sboxDecrypt_c[12][1] = 0; sboxDecrypt_c[12][2] = 1; sboxDecrypt_c[12][3] = 1;
	sboxDecrypt_c[3][0] = 1; sboxDecrypt_c[3][1] = 0; sboxDecrypt_c[3][2] = 0; sboxDecrypt_c[3][3] = 0;
	sboxDecrypt_c[1][0] = 0; sboxDecrypt_c[1][1] = 0; sboxDecrypt_c[1][2] = 1; sboxDecrypt_c[1][3] = 1;
	sboxDecrypt_c[6][0] = 1; sboxDecrypt_c[6][1] = 0; sboxDecrypt_c[6][2] = 1; sboxDecrypt_c[6][3] = 0;
	sboxDecrypt_c[11][0] = 0; sboxDecrypt_c[11][1] = 1; sboxDecrypt_c[11][2] = 1; sboxDecrypt_c[11][3] = 0;
	sboxDecrypt_c[5][0] = 1; sboxDecrypt_c[5][1] = 1; sboxDecrypt_c[5][2] = 0; sboxDecrypt_c[5][3] = 0;
	sboxDecrypt_c[15][0] = 0; sboxDecrypt_c[15][1] = 1; sboxDecrypt_c[15][2] = 0; sboxDecrypt_c[15][3] = 1;
	sboxDecrypt_c[10][0] = 1; sboxDecrypt_c[10][1] = 0; sboxDecrypt_c[10][2] = 0; sboxDecrypt_c[10][3] = 1;
	sboxDecrypt_c[14][0] = 0; sboxDecrypt_c[14][1] = 0; sboxDecrypt_c[14][2] = 0; sboxDecrypt_c[14][3] = 0;
	sboxDecrypt_c[8][0] = 0; sboxDecrypt_c[8][1] = 1; sboxDecrypt_c[8][2] = 1; sboxDecrypt_c[8][3] = 1;
	for (int i = 0; i < 5; ++i) {
		threadArgu.roundKey[i].state = false;
	}
	for (int i = 0; i < 5; ++i) {
		threadArguDecrypt.roundKey[i].state = false;
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

void MySPN::setKeyDecrypt(char * key)
{
	for (int i = 0; i < 32; i++)
	{
		threadArguDecrypt.key[i] = key[i];
	}
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)roundKeyCreatDecrypt, (LPVOID)&threadArguDecrypt, 0, NULL);
}

void MySPN::setKeyDecrypt(unsigned long key)
{
	char keyChar[32];
	for (int i = 0; i < 32; i++)
	{
		keyChar[i] = (unsigned char)(key % 2);
		key /= 2;
	}
	setKeyDecrypt(keyChar);
}

unsigned short MySPN::encrypt16(unsigned short plaintext_s)
{
	unsigned short temp[4] = { 0 };
	plaintext[0] = plaintext_s & 0x000f;
	plaintext[1] = (plaintext_s >> 4) & 0x000f;
	plaintext[2] = (plaintext_s >> 8) & 0x000f;
	plaintext[3] = (plaintext_s >> 12) & 0x000f;
	encrypt16_hex(plaintext);
    temp[0] = crytext[0];
	temp[1] = crytext[1];
	temp[1] = (temp[1] << 4)&0xF0;
	temp[2] = crytext[2];
	temp[2] = (temp[2] << 8)&0xf00;
	temp[3] = crytext[3];
	temp[3] = (temp[3] << 12 )&0xf000;
	return temp[0]|temp[1]|temp[2]|temp[3];
}

unsigned char* MySPN::encrypt16(unsigned char * plaintext_c) {
	plaintext[0] = c2hc(plaintext_c[0]);
	plaintext[1] = c2hc(plaintext_c[1]);
	plaintext[2] = c2hc(plaintext_c[2]);
	plaintext[3] = c2hc(plaintext_c[3]);
	return encrypt16_hex(plaintext);
}

bool MySPN::encryptFile(char * filename, char *newfilename)
{
	FILE * file;
	FILE * aimfile;
	if (fopen_s(&file,filename, "rb")) {
		std::cout << filename<<"打开失败" << std::endl;
		return false;
	}
	if (fopen_s(&aimfile,newfilename, "wb")) {
		std::cout << newfilename << "创建失败" << std::endl;
		return false;
	}
	while (!feof(file)) {
		char c1[2];
		c1[0]=fgetc(file);
		if (!feof(file)) {
			c1[1] = fgetc(file);
		}
		else {
			c1[1] = 0;
		}
		unsigned short y = encrypt16((unsigned short)*((unsigned short *)c1));
		char * out = (char *)&y;
		fputc(out[0],aimfile);
		fputc(out[1], aimfile);
	}
	fclose(file);
	fclose(aimfile);
	return true;
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

//解密
unsigned short MySPN::decrypt16(unsigned short crypttext_s, unsigned short &plaintext_s)
{
	char plaintext_char[16];
	char crypttext_char[16];
	unsigned short temp=1;
	plaintext_s = 0;
	for (int i = 0; i <16; ++i)
	{
		crypttext_char[i] = (char)(crypttext_s % 2);
		crypttext_s /= 2;
	}
	decrypt16(crypttext_char, plaintext_char);
	for (int i = 0; i <16; ++i)
	{
		plaintext_s += (unsigned short)(plaintext_char[i]*temp);
		temp *= 2;
	}
	return plaintext_s;
}

//16 位长度的字符串数组
unsigned short MySPN::decrypt16(char * thiscrypttext, char * thisplaintext)
{
	for (int i = 0; i < 16; ++i)
	{
		thisplaintext[i] = thiscrypttext[i];
	}
	while (!threadArguDecrypt.roundKey[4].state);
	xorDecrypt(thisplaintext, threadArguDecrypt.roundKey[4].keyChar);
	sboxDecrypt(thisplaintext);
	while (!threadArguDecrypt.roundKey[3].state);
	xorDecrypt(thisplaintext, threadArguDecrypt.roundKey[3].keyChar);
	for (int i = 2; i >= 0; --i)
	{
		pboxDecrypt(thisplaintext);
		sboxDecrypt(thisplaintext);
		while (!threadArguDecrypt.roundKey[i].state);
		xorDecrypt(thisplaintext, threadArguDecrypt.roundKey[i].keyChar);
	}	
	return 0;
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

bool MySPN::decryptFile(char * filename, char *newfilename)
{
	FILE * file;
	FILE * aimfile;
	if (fopen_s(&file,filename, "rb")) {
		std::cout << filename << "打开失败" << std::endl;
		return false;
	}
	if (fopen_s(&aimfile,newfilename, "wb")) {
		std::cout << newfilename << "创建失败" << std::endl;
		return false;
	}
	while (!feof(file)) {
		char c1[2];
		c1[0] = fgetc(file);
		if (!feof(file)) {
			c1[1] = fgetc(file);
		}
		else {
			c1[1] = 0;
		}
		unsigned short y;
		decrypt16((unsigned short)*((unsigned short *)c1),y);
		char * out = (char *)&y;
		fputc(out[0], aimfile);
		fputc(out[1], aimfile);
	}
	fclose(file);
	fclose(aimfile);
	return true;
}

MySPN::~MySPN()
{
	for (int i = 0; i < 5; ++i) {
		threadArgu.roundKey[i].state = false;
	}
	for (int i = 0; i < 5; ++i) {
		threadArguDecrypt.roundKey[i].state = false;
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

void MySPN::roundKeyCreatDecrypt(LPVOID argu)
{
	struct argumKeyDecrypt * argument = (struct argumKeyDecrypt *) argu;
	for (int i = 4; i >= 0; --i)
	{
		int m = i * 4;
		for(int j = 0;j<16;++j)
		{
			(argument->roundKey[i]).keyChar[j] = argument->key[m++];
		}
		argument->roundKey[i].state = true;
	}
	return;
}

inline void MySPN::sboxDecrypt(char * input)
{
	for (int i = 0; i < 4; ++i)
	{
		int j = input[i * 4] * 8 + input[i * 4 + 1] * 4 + input[i * 4 + 2] * 2 + input[i * 4 + 3];
		input[i*4]= sboxDecrypt_c[j][0];
		input[i*4+1]= sboxDecrypt_c[j][1];
		input[i * 4 + 2]=sboxDecrypt_c[j][2];
		input[i * 4 + 3]=sboxDecrypt_c[j][3];
	}
}

inline void MySPN::pboxDecrypt(char * input)
{
	char temp;
	temp = input[1]; input[1] = input[4]; input[4] = temp;
	temp = input[2]; input[2] = input[8]; input[8] = temp;
	temp = input[3]; input[3] = input[12]; input[12] = temp;
	
	temp = input[6]; input[6] = input[9]; input[9] = temp;
	temp = input[7]; input[7] = input[13]; input[13] = temp;
	
	temp = input[11]; input[11] = input[14]; input[14] = temp;
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
