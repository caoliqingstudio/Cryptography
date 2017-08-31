// Cryptography.cpp : 定义控制台应用程序的入口点。
//
//这里是spn部分
#include "stdafx.h"
#include "myspn.h"
#include "MyRSA.h"

int main() {
	MyRSA test;
	test.CreateKey();
	return 0;
}

int test_spn_01() {
	char input[20];
	char keyinput[50];
	for (int i = 0; i < 32; i++)
	{
		std::cin >> keyinput[i];
	}
	MySPN test;
	test.setKey_01(keyinput);
	std::cout << "key ok" << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::cin >> input[i];
	}
	std::cout << test.encrypt16_01(input) << std::endl;
	return 0;
}

int spn()
{
	int keyType;
	unsigned char plaintext[10];
	char keyString[10];
	MySPN test;
	unsigned short keyNum;
	do {
		std::cout << "请输入密钥格式：\n16进制表示请输入1\n十进制数字表示请输入0\n" << std::endl;
		std::cin >> keyType;
		std::cout << "请按照您确定的格式输入密钥（二进制32位）(16进制请不要输入0x)\n"
			<<"16进制请按照地址顺序输入（类小端格式，与地址顺序相同） "<< std::endl;
		if (keyType == 1) {
			std::cin >> keyString;
			test.setKey(keyString);
			break;
		}else if(keyType==0){
			std::cin >> keyNum;
			test.setKey(keyNum);
			break;
		}else {
			std::cout << "请按要求输入0/1" << std::endl;
			continue;
		}
	} while (true);//如果输错选项就循环执行
	std::cout << "请输入16进制明文（二进制长度为16位）\n" << std::endl;
	std::cin >> plaintext;//没有对类型进行检测，应该有的，但是为了减少运行时间
	test.encrypt16(plaintext);
	std::cout << "密文： " << std::hex << (int)test.crytext[0] 
		<<(int) test.crytext[1] << (int)test.crytext[2] 
		<< (int)test.crytext[3] << std::endl;
	system("pause");//看结果的
	return 0;
}

