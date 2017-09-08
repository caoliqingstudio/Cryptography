#pragma once

#include "myspn.h"

class MySPNPlus {
public:
	MySPNPlus();
	~MySPNPlus();
	void setKey(unsigned long long key);
	void setKeyDecrypt(unsigned long long key);
	unsigned short encrypt(unsigned short plaintext);
	bool encryptFile(char *filename, char *newfilename);
	unsigned short decrypt(unsigned short crytext);
	bool decryptFile(char *filename, char *newfilename);
private:
	MySPN myspn1,myspn2,myspn3;
};