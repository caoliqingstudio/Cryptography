// Cryptography.cpp : 定义控制台应用程序的入口点。
//
//这里是spn部分
#include "stdafx.h"
#include "myspn.h"
#include "MyRSA.h"
#include "cryptanalysis.h"
#include "MySPNPlus.h"
#include "communicate.h"

time_t start, end;

int main() {
	Differential test;
	test.analyse();
	system("pause");
}

//通信
int test_communicate() {
	Communicate test;
	char file1[] = "D://first";
	char file2[] = "D://second";
	char file3[] = "D://third";
	system("cls");
	std::cout << "接下来 是通信测试\n" << "请查看D盘\n" << std::endl;
	start = clock();
	test.encrypt(file1, file2);
	end = clock();
	printf("%s 文件加密得到 %s\t用时 %t\n", file1, file2, end - start);
	start = clock();
	test.decrypt(file2, file3);
	end = clock();
	printf("%s 文件解密得到 %s\t用时 %t\n", file2, file3, end - start);
	system("pause");
	return 0;
}

//RSA
int test_RSA() {
	mpz_t pub,result;
	mpz_init(result);
	mpz_init(pub);
	mpz_set_ui(pub,0x454624);
	//mpz_t thistext;
	//mpz_init_set(thistext, text);
	//mpz_set_ui(result, 1);
	MyRSA myrsa;
	system("cls");
	std::cout << "接下来 是RSA测试\n" << "参数生成" << std::endl;
	start = clock();
	myrsa.CreateKey();
	end = clock();
	myrsa.printKey();
	printf("\n用时 %t\n", end - start);
	printf("100次加密数据");
	gmp_printf("%Zd\n", pub);


	start = clock();
	for (int i = 0; i < 100; i++)
	{
		myrsa.encrypt_ModS2(pub, result);
	}
	end = clock();
	std::cout << "模重复平方法加密时间 : " << end - start << std::endl;
	gmp_printf("\n密文: %Zd\n", result);
	printf("100次解密该数据\n");

	start = clock();
	for (int i = 0; i < 100; i++)
	{
		myrsa.decrypt_Montgomery(pub, result);
	}
	end = clock();
	std::cout << "蒙哥马利解密时间 : " << end - start << std::endl;
	
	start = clock();
	for (int i = 0; i < 100; i++)
	{
		myrsa.decrypt_China(pub, result);
	}
	end = clock();
	std::cout << "中国剩余定理解密时间 : " << end - start << std::endl;
	
	gmp_printf("\n明文: %Zd\n", pub);
	system("pause");
	return 0;
}

//线性分析
int test_LinearAnalyse() {
	Linear text;
	system("cls");
	std::cout << "接下来 是线性分析测试\n" << "预设密钥是0xab851257" << std::endl;
	start = clock();
	text.analyse();
	end = clock();
	std::cout <<"总计用时  "<< end - start << std::endl;
	system("pause");
	return 0;
}

//spn增强版 文件加解密
int test_spnPlus_file() {
	MySPNPlus test;
	test.setKey(0x7011409070114090);
	test.setKeyDecrypt(0x7011409070114090);
	system("cls");
	std::cout << "spn 增强版 文件加解密" << std::endl;
	test.encryptFile("D://first", "D://new");
	test.decryptFile("D://new", "D://old");
	system("pause");
	return 0;
}

//spn 文件加解密
int test_SPN_file() {
	MySPN test;
	test.setKey(0x70114090);
	test.setKeyDecrypt(0x70114090);
	system("cls");
	std::cout << "spn 文件加解密" << std::endl;
	test.encryptFile("D://first", "D://new");
	test.decryptFile("D://new", "D://old");
	system("pause");
	return 0;
}

//spn 加解密  数字形式十进制
int test_decrypt_SPN() {
	MySPN test;
	test.setKey(255);
	test.setKeyDecrypt(255);
	unsigned short text, aaa=0,bbb=0;
	system("cls");
	std::cout << "输入明文" << std::endl;
	std::cin >> text;
	aaa=test.encrypt16(text);
	std::cout << "加密" << std::endl;
	std::cout << aaa << std::endl;
	std::cout << "解密" << std::endl;
	std::cout << test.decrypt16(aaa,bbb)<< std::endl;
	return 0;
}

//RSA 纯密钥生成
int test_RSA_key() {
	MyRSA test;
	clock_t start;
	clock_t end;	
	system("cls");
	std::cout << "RSA 纯密钥生成 用时" << std::endl;
	start = clock();
	test.CreateKey();
	end = clock();
	test.printKey();
	std::cout << "\n\n" << end - start << std::endl;
	system("pause");
	return 0;
}

//spn 以01二进制形式 输入输出测试
int test_spn_01() {
	char input[20];
	char keyinput[50];
	system("cls");
	std::cout << "spn 01二进制形式输入输出测试\n 请输32位入密钥" << std::endl;
	for (int i = 0; i < 32; i++)
	{
		std::cin >> keyinput[i];
	}
	MySPN test;
	test.setKey_01(keyinput);
	std::cout << "key ok\n请输入明文16位" << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::cin >> input[i];
	}
	std::cout << "密文" << std::endl;
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
	system("cls");
	std::cout << "spn 常规测试" << std::endl;
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

