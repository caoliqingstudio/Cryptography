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
	~MySPN();
	unsigned char crytext[5];
	unsigned char plaintext[4];
	//析构函数，用于释放内存
private:
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