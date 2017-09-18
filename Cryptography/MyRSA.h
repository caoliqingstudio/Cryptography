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
#define MONTBITS (8*sizeof(mp_limb_t))
//假设蒙哥马利中的T=XY的最大位数不超过MONT_MAX个整数
#define MONT_MAX 128

class  MyRSA
{
public:
	 MyRSA();
	~ MyRSA();
	void CreateKey();
	void CreatePPkey();
	void ClearKey();
	void printKey();
	void encrypt_ModS2(mpz_t &plaintext, mpz_t &crytext);//模重复平方
	void encrypt_Montgomery(mpz_t &plaintext, mpz_t &crytext);//蒙哥马利
	void encrypt_China(mpz_t &plaintext, mpz_t &crytext);//中国剩余定理
	void decrypt_ModS2(mpz_t &plaintext, mpz_t &crytext);//模重复平方
	void decrypt_Montgomery(mpz_t &plaintext, mpz_t &crytext);//蒙哥马利
	void decrypt_China(mpz_t &plaintext, mpz_t &crytext);//中国剩余定理
	mpz_t  n, pub, pri;
	mpz_t xp_ch, xq_ch,p,q;
	//mpz_t xp
private:
	void SquareMultiply(mpz_t &result, mpz_t & text, mpz_t &pub, mpz_t &mod);//模平方
	//void Montgomery(mpz_t &result, mpz_t & text, mpz_t &pub, mpz_t &mod);//蒙哥马利
	void China(mpz_t &result, mpz_t text, mpz_t pub,mpz_t p,mpz_t q, mpz_t mod);//中国剩余定理
	bool ok;
	gmp_randstate_t state;
	struct arguement {
		bool * ok;
		gmp_randstate_t * state;
		mpz_t *p;
	}argue;
	void MontMulti(mpz_t T, const mpz_t x, const mpz_t y, const  mpz_t N, const mp_limb_t N_1);
	void Mont_Exp(mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t N);
	static bool isNotPrime(mpz_t &num);
	bool relaPrime(mpz_t &a, mpz_t &b, mpz_t &c, bool state);
	bool priCreat(mpz_t &a, mpz_t &b);
	static void bigPrime(LPVOID argue);
};