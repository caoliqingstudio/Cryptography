#pragma once

//#include "myspn.h"
//key  128/8 bytes = 16
class MySPNPlus {
public:
	MySPNPlus();
	~MySPNPlus();
	void setKey(unsigned char * key);
	void setKeyDecrypt(unsigned char * key);
	unsigned long long encrypt(unsigned long long plaintext);
	bool encryptFile(char *filename, char *newfilename);
	unsigned long long decrypt(unsigned long long crytext1);
	bool decryptFile(char *filename, char *newfilename);
private:
	unsigned char sbox[256];
	inline void sbox64(unsigned char *tempS) {
		for (int i_s = 0; i_s < 8; i_s++)
		{
			tempS[i_s] = sbox[tempS[i_s]];
		}
	}

	inline unsigned long long pbox64(unsigned char *tempp);
	unsigned char sboxDecrypt[256];
	//MySPN myspn1, myspn2, myspn3, myspn4, myspn5, myspn6;
	unsigned long long roundkey[17],roundkeyDecrypt[17];
	inline void sboxDecrypt64(unsigned char *tempS) {
		for (int i_s = 0; i_s < 8; i_s++)
		{
			tempS[i_s] = sboxDecrypt[tempS[i_s]];
		}
	}
};