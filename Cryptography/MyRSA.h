#pragma once
#include "stdafx.h"
#include "gmp.h"
#include <time.h>

#pragma comment (lib , "libgmp-6.1.1.lib")

#define SOLOVAY_STRASSEN_NUM 10 //素性检测次数

class  MyRSA
{
public:
	 MyRSA();
	~ MyRSA();
	void CreateKey();
	void ClearKey();
	void printKey();

private:
	mpz_t p, q, n, pub, pri;
	gmp_randstate_t state;
	bool isNotPrime(mpz_t num);//不是素数 输出 true
	bool relaPrime(mpz_t a, mpz_t b);//互素 输出 true
};