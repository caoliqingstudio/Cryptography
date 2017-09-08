#include "stdafx.h"
#include "MySPNPlus.h"

MySPNPlus::MySPNPlus()
{
}

MySPNPlus::~MySPNPlus()
{
}

void MySPNPlus::setKey(unsigned long long key)
{
	unsigned long key1, key2, key3;
	key1 = (unsigned long)*((unsigned long *)(&key));
	key2 = (unsigned long)*(((unsigned long *)(&key)+1));
	key3 = key1+key2;
	myspn1.setKey(key1);
	myspn2.setKey(key2);
	myspn3.setKey(key3);
}

void MySPNPlus::setKeyDecrypt(unsigned long long key)
{
	unsigned long key1, key2, key3;
	key1 = (unsigned long)*((unsigned long *)(&key));
	key2 = (unsigned long)*(((unsigned long *)(&key) + 1));
	key3 = key1 + key2;
	myspn1.setKeyDecrypt(key1);
	myspn2.setKeyDecrypt(key2);
	myspn3.setKeyDecrypt(key3);
}

unsigned short MySPNPlus::encrypt(unsigned short plaintext)
{
	unsigned short temp;
	temp = myspn1.encrypt16(plaintext);
	temp = myspn2.encrypt16(temp);
	return myspn3.encrypt16(temp);
}

bool MySPNPlus::encryptFile(char * filename, char * newfilename)
{
	FILE * file;
	FILE * aimfile;
	if (fopen_s(&file, filename, "rb")) {
		std::cout << filename << "打开失败" << std::endl;
		return false;
	}
	if (fopen_s(&aimfile, newfilename, "w+")) {
		std::cout << newfilename << "创建失败" << std::endl;
		return false;
	}
	char c1[2];
	c1[0] = fgetc(file);
	while (!feof(file)) {
		c1[1] = fgetc(file);
		if (feof(file)) {
			c1[1] = 0;
		}
		unsigned short y = encrypt((unsigned short)*((unsigned short *)c1));
		char * out = (char *)&y;
		fputc(out[0], aimfile);
		fputc(out[1], aimfile);
		c1[0] = fgetc(file);
	}
	fclose(file);
	fclose(aimfile);
	return true;
}

unsigned short MySPNPlus::decrypt(unsigned short crytext)
{
	unsigned short temp;
	myspn3.decrypt16(crytext, temp);
	myspn2.decrypt16(temp, temp);
	return myspn1.decrypt16(temp, temp);
}

bool MySPNPlus::decryptFile(char * filename, char * newfilename)
{
	FILE * file;
	FILE * aimfile;
	if (fopen_s(&file, filename, "rb")) {
		std::cout << filename << "打开失败" << std::endl;
		return false;
	}
	if (fopen_s(&aimfile, newfilename, "wb")) {
		std::cout << newfilename << "创建失败" << std::endl;
		return false;
	}
	//int i = 0;
	char c1[2];
	c1[0] = fgetc(file);
	while (!feof(file)) {
		//i++;
		c1[1] = fgetc(file);
		if (feof(file)) {
			c1[1] = 0;
		}
		unsigned short y;
		y = decrypt((unsigned short)*((unsigned short *)c1));
		char * out = (char *)&y;
		fputc(out[0], aimfile);
		fputc(out[1], aimfile);
		c1[0] = fgetc(file);
	}
	//std::cout << i << std::endl;
	fclose(file);
	fclose(aimfile);
	return true;
}
