#include "stdafx.h"
#include "MyRSA.h"
#include "communicate.h"

Communicate::Communicate()
{
	mpz_inits(p, q, n, publicKey, privateKey, NULL);
	myrsa.CreateKey();
	createSPNkey();
}

Communicate::~Communicate()
{
	mpz_clears(p, q, n, publicKey, privateKey, NULL);
}

void Communicate::createRSAkey()
{
	myrsa.CreatePPkey();
}

void Communicate::createSPNkey()
{
	srand((unsigned)time(NULL));
	*((unsigned char*)spnKey) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 1) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 2) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 3) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 4) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 5) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 6) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 7) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 8) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 9) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 10) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 11) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 12) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 13) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 14) = (unsigned char)rand();
	*(((unsigned char*)spnKey) + 15) = (unsigned char)rand();
}

void Communicate::encrypt(char * file, char * newfile)
{
	FILE *thisnewfile, *thiscryfile;
	mpz_t spnKeyGMP, spnKeyCry;
	mpz_init(spnKeyGMP);
	mpz_init(spnKeyCry);
	
	//加密 spn
	char keyChar[50];
	char keyCharInput[300] = { 0 };

	//密文写入写入文件
	fopen_s(&thisnewfile, newfile, "w");
	if (thisnewfile) {
		_ui64toa_s(spnKey[0], keyChar, 50, 10);
		mpz_set_str(spnKeyGMP, keyChar, 10);
		myrsa.encrypt_ModS2(spnKeyGMP, spnKeyCry);
		mpz_export(keyCharInput, NULL, 1, 1, 0, 0, spnKeyCry);//可以计算得到 255
		for (int i = 0; i < 256; i++)
		{
			fputc(keyCharInput[i], thisnewfile);
		}
		_ui64toa_s(spnKey[1], keyChar, 50, 10);
		mpz_set_str(spnKeyGMP, keyChar, 10);
		myrsa.encrypt_ModS2(spnKeyGMP, spnKeyCry);
		mpz_export(keyCharInput, NULL, 1, 1, 0, 0, spnKeyCry);//可以计算得到 255
		for (int i = 0; i < 256; i++)
		{
			fputc(keyCharInput[i], thisnewfile);
		}

		//spn 加密 并加入文件
		myspnplus.setKey((unsigned char *)spnKey);
		myspnplus.encryptFile(file, TEMP_FILE_NAME);
		fopen_s(&thiscryfile, TEMP_FILE_NAME, "rb");
		if (thiscryfile) {
			char c;
			int mm = 0;
		    fread(&c,sizeof(char),1,thiscryfile);
			while (!feof(thiscryfile)) {
				fputc(c, thisnewfile);
				fread(&c, sizeof(char), 1, thiscryfile);
				mm++;
			}
			fclose(thiscryfile);
		}
		else
		{
			printf("\n%s 无法解析\n",file);
		}
		fclose(thisnewfile);
		remove(TEMP_FILE_NAME);
	}
	else {
		printf("\n%s 无法创建\n", newfile);
	}
	mpz_clear(spnKeyGMP);
	mpz_clear(spnKeyCry);
}

void Communicate::decrypt(char * file, char * newfile)
{

	FILE *thisnewfile, *thiscryfile;
	mpz_t spnKeyGMP, spnKeyCry;
	mpz_init(spnKeyGMP);
	mpz_init(spnKeyCry);

	//解密 spn
	char keyCharInput[300] = { 0 };
	fopen_s(&thiscryfile, file, "rb");
	if (thiscryfile) {
		for (int i = 0; i < 256; i++)
		{
			keyCharInput[i] = fgetc(thiscryfile);
		}
		mpz_import(spnKeyCry, 256, 1, 1, 0, 0, keyCharInput);
		myrsa.decrypt_ModS2(spnKeyGMP, spnKeyCry);
		//mpz_export(keyChar, NULL, -1, 1, 0, 0, spnKeyGMP);
		//spnKey = *((unsigned long long *)keyChar);
		spnKey[0] = *((unsigned long long*)spnKeyGMP->_mp_d);
		for (int i = 0; i < 256; i++)
		{
			keyCharInput[i] = fgetc(thiscryfile);
		}
		mpz_import(spnKeyCry, 256, 1, 1, 0, 0, keyCharInput);
		myrsa.decrypt_ModS2(spnKeyGMP, spnKeyCry);
		//mpz_export(keyChar, NULL, -1, 1, 0, 0, spnKeyGMP);
		//spnKey = *((unsigned long long *)keyChar);
		spnKey[1] = *((unsigned long long*)spnKeyGMP->_mp_d);

		myspnplus.setKeyDecrypt((unsigned char *)spnKey);

		fopen_s(&thisnewfile, TEMP_FILE_NAME, "wb");
		if (thiscryfile) {
			char c;
			c = fgetc(thiscryfile);
			while (!feof(thiscryfile)) {
				fputc(c, thisnewfile);
				c = fgetc(thiscryfile);
			}
			fclose(thisnewfile);
			myspnplus.decryptFile(TEMP_FILE_NAME, newfile);
		}
		else
		{
			printf("\n%s 无法解析\n", file);
		}
		fclose(thiscryfile);
		remove(TEMP_FILE_NAME);
	}
	else {
		printf("\n%s 无法创建\n", newfile);
	}
	mpz_clear(spnKeyGMP);
	mpz_clear(spnKeyCry);
}
