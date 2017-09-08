#pragma once

#include "MySPNPlus.h"
#include "MyRSA.h"

#define TEMP_FILE_NAME "temp.clq"

class Communicate {
public:
	Communicate();
	~Communicate();
	void createRSAkey();
	void createSPNkey();
	mpz_t publicKey, n;
	unsigned long long spnKey;
	void encrypt(char *file,char *newfile);
	void decrypt(char *file, char *newfile);
private:
	mpz_t privateKey, p, q;
	bool rsaKeyState, spnKeyState;
	MyRSA myrsa;
	MySPNPlus myspnplus;
};
