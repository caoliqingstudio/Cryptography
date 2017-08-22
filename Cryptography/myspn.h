/*myspn 接口介绍
void setKey(char * key);
void setKey(unsigned long key);
	设置密钥，分两种，一种是字符串类型的，char[4]，内容为16进制
	另一种是数值 unsigned long
	无返回
unsigned short encrypt16(unsigned short plaintext_s);
unsigned char* encrypt16(unsigned char * plaintext_c);
	16位明文加密，分两种，一种是对16位数值进行加密，返回16位数值
	另一种是对字符串类型的十六进制内容加密，char[4]，
		输入内容为字符‘0’--‘9’，‘a/A’--‘f/F’，
		返回字符串，非字符，而是四位一组的数值
~MySPN();
	释放空间，未用到，但是伪初始化了一下，可以新设置密钥（其实没啥用）
unsigned char crytext[5];
	密文，unsiigned char* encrypt16返回内容，crytext[4]='/0'，输出可能用到
unsigned char plaintext[4];
	明文，结构同crytext

2017.8.21 为了线性和查分分析，特别添加以下两个接口
void setKey_01(char *key); 
	输入为 字符串字面的“010101”
char * encrypt16_01(char *plaintext_01); 
	输入为 字符串字面你的“010101”，输出为字符串数值的“\0\1\0\1\0\1”
*/
#pragma once
#include "stdafx.h"
#include <windows.h>

//#define NumRound 4

class MySPN {
public:
	MySPN();//构造函数
	void setKey(char * key);
	void setKey(unsigned long key);
	unsigned short encrypt16(unsigned short plaintext_s);
	unsigned char* encrypt16(unsigned char * plaintext_c);
	~MySPN();	//析构函数，用于释放内存
	unsigned char crytext[5];
	unsigned char plaintext[4];
	void setKey_01(char *key); //分析专用，01比特输入字符串
	char * encrypt16_01(char *plaintext_01); //分析专用，01比特输入字符串

private:
	char crytext_01[17]; //分析专用，01比特字符串
	unsigned char * encrypt16_hex(unsigned char * plaintext_c);
	static void roundKeyCreat(LPVOID argu);//轮密钥生成，此处打算另开一个线程运行
	inline char c2hc(char temp) {
		return (temp >= '0'&&temp <= '9') ? temp - '0' :
			(temp >= 'A'&&temp <= 'F') ? temp - 'A' + 10 : temp - 'a' + 10;
	}
	struct argumKey {
		unsigned long inputKey;
		struct storeKey
		{
			unsigned short key;
			unsigned char keyChar[4];
			bool state;
		} roundKey[5];//存储轮密钥
	} threadArgu;
	inline void xor16(unsigned char *key,unsigned char *text) {
		text[0] = text[0] ^ key[0];
		text[1] = text[1] ^ key[1];
		text[2] = text[2] ^ key[2];
		text[3] = text[3] ^ key[3];
	}//密文异或的内联函数
	unsigned char sbox_c[16];
	inline void sbox16(unsigned char *text) {
		text[0] = sbox_c[text[0]];
		text[1] = sbox_c[text[1]];
		text[2] = sbox_c[text[2]];
		text[3] = sbox_c[text[3]];
	}
	inline void pbox16(unsigned char *text);
	HANDLE hThread;
};