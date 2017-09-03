/*
经过测试，参数生成的主要时间产生在了大素数的生成，
 由于要生成两个大素数，所以将开一个线程用来产生一个大素数
 生成时间由350 左右降到了 200左右
*/
#pragma once
#include "stdafx.h"
#include "gmp.h"
#include <time.h>
#include <Windows.h>

#pragma comment (lib , "libgmp-6.1.1.lib")

#define PRIME_NUM 10 //素性检测次数

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
	bool ok;
	gmp_randstate_t state;
	struct arguement {
		bool * ok;
		gmp_randstate_t * state;
		mpz_t *p;
	}argue;
	static bool isNotPrime(mpz_t &num);
	bool relaPrime(mpz_t &a, mpz_t &b, mpz_t &c, bool state);
	bool priCreat(mpz_t &a, mpz_t &b);
	static void bigPrime(LPVOID argue);
};