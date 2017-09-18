#include "stdafx.h"
#include "MyRSA.h"

MyRSA::MyRSA()
{
	argue.ok = &ok;
	argue.state = &state;
	mpz_init(p);
	mpz_init(xp_ch);
	mpz_init(q);
	mpz_init(xq_ch);
	mpz_init(n);
	mpz_init(pri);
	mpz_init(pub);
	gmp_randinit_default(state);
}

MyRSA::~MyRSA()
{
	mpz_clears(p, xq_ch, xp_ch,q, n, pri, pub, NULL);
}

void MyRSA::CreateKey()
{
	time_t seed;
	time(&seed);
	gmp_randseed_ui(state,unsigned long(seed));
	gmp_randinit_lc_2exp_size(state, 128);
	//随机数 得到 p  q
	argue.p = &p;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)bigPrime, (LPVOID)&argue, 0, NULL);
	do {
		mpz_rrandomb(q, state, 1024);
		mpz_setbit(q, 0);	//设置为奇数
	} while (isNotPrime(q));
	while (!ok);

//	mpz_set_ui(p, 47);
//	mpz_set_ui(q, 71);
//	mpz_set_ui(pub, 79);
	//得到 n
	mpz_mul(n, p, q);
	CreatePPkey();
}

void MyRSA::CreatePPkey()
{
	mpz_t psub, qsub;
	mpz_init(psub);
	mpz_init(qsub);
	mpz_t euler;
	mpz_init(euler);
	//欧拉数
	mpz_sub_ui(psub, p, 1);
	mpz_sub_ui(qsub, q, 1);
	mpz_mul(euler, psub, qsub);
	do {//得到私钥
		//得到公钥
		do {
			mpz_rrandomb(pub, state, 1024);
			mpz_setbit(pub, 0);	//设置为奇
		} while (!relaPrime(pub, psub, qsub, true));
	} while (!priCreat(pub, euler)); //如果 公约数非 1 则不互素
	mpz_fdiv_r(pri, pri, euler);
	mpz_clears(psub, qsub, euler, NULL);

	mpz_invert(xq_ch, q, p);
	mpz_invert(xp_ch, p, q);// xq = q ^ -1 mod p  中国剩余定理 预处理
}

//目前啥都没做 我感觉 没什么可以做的 倒是可以加个reset
void MyRSA::ClearKey()
{
}

void MyRSA::printKey()
{
	gmp_printf("p   = : %Zd \n", p);
	gmp_printf("q   = : %Zd \n", q);
	gmp_printf("n   = : %Zd \n", n);
	printf("公钥= :");
	gmp_printf(" %Zd \n", pub);
	printf("私钥= :");
	gmp_printf(" %Zd \n", pri);
}

void MyRSA::encrypt_ModS2(mpz_t & plaintext, mpz_t & crytext)
{
	//mpz_powm(crytext, plaintext, pub, n);
	SquareMultiply(crytext, plaintext, pub, n);
}

void MyRSA::encrypt_Montgomery(mpz_t & plaintext, mpz_t & crytext)
{
	Mont_Exp(crytext, plaintext, pub, n);
}

void MyRSA::encrypt_China(mpz_t & plaintext, mpz_t & crytext)
{
	//China(crytext, plaintext, pub, p, q, n);
	std::cout << "no use" << std::endl;
}

void MyRSA::decrypt_ModS2(mpz_t & plaintext, mpz_t & crytext)
{
//	mpz_powm(plaintext, crytext, pri, n);
	SquareMultiply(plaintext, crytext, pri, n);
}

void MyRSA::decrypt_Montgomery(mpz_t & plaintext, mpz_t & crytext)
{
	Mont_Exp(plaintext, crytext, pri, n);
}

void MyRSA::decrypt_China(mpz_t & plaintext, mpz_t & crytext)
{
	China(plaintext, crytext, pri, p, q, n);
}

void MyRSA::SquareMultiply(mpz_t & result, mpz_t & text,mpz_t &pub, mpz_t & mod)
{
	mpz_set_ui(result, 1);
	int length;
	length = mpz_popcount(pub);
//	printf("%d\n", length);
	int j = 0;
	for (int i = 0; i < length; ++i)
	{
		while (!mpz_tstbit(pub, j++));
	}
	for (j--; j >= 0; --j)
	{
		while (!mpz_tstbit(pub, j)) {
			--j;
			mpz_powm_ui(result, result, 2, mod);
			if (j < 0) return;
		}
		mpz_powm_ui(result, result, 2, mod);
		mpz_mul(result, result, text);
		mpz_mod(result, result, mod);
	}
	return;
}
/*
void MyRSA::Montgomery(mpz_t & result, mpz_t & text, mpz_t & pub, mpz_t & mod)
{
//	mpz_t C, E;
//	mpz_inits(C, E, NULL);
//	mpz_set_ui(result, 1);
//	mpz_set(E, pub);
//	mpz_set(C, text);
//	while (mpz_cmp_ui(E, 0) > 0) {
//		if (mpz_odd_p(E)) {
//			mpz_mul(result, C, result);
//			mpz_mod(result, result, mod);
//
//			mpz_sub_ui(E, E, 1);
//		}
//		else {
//			mpz_powm_ui(C, C, 2, mod);
//			mpz_div_ui(E, E, 2);
//		}
//	}
	mpz_t key,r;
	mpz_init(r);
	mpz_set_ui(result, 1);
	mpz_init_set(key, pub);
	mpz_mod(r, text, mod);
	while(mpz_cmp_ui(key,1)>0)
	{
		if (mpz_odd_p(key)) {
			mpz_mul(result, result, r);
			mpz_mod(result, result, mod);
		}
		mpz_powm_ui(r, r, 2, mod);
		mpz_div_ui(key, key, 2);
	}
	mpz_mul(result, result, r);
	mpz_mod(result, result, mod);
	mpz_clears(key, r, NULL);
	return;
}*/

void MyRSA::China(mpz_t &result, mpz_t text, mpz_t pub, mpz_t p, mpz_t q, mpz_t mod)
{
	mpz_t xp, xq, dp, dq, yp, yq, psub, qsub;
	mpz_inits(xp, xq, dp, dq, yp, yq, psub, qsub, NULL);

	mpz_mod(xp, text, p);
	mpz_mod(xq, text, q);//xp== x mod p

	mpz_sub_ui(psub, p, 1);
	mpz_sub_ui(qsub, q, 1);
	mpz_mod(dp, pub, psub);
	mpz_mod(dq, pub, qsub);// dp = pub  mod  p-1

	mpz_powm(yp, xp, dp, p);
	mpz_powm(yq, xq, dq, q);// yp = xp ^ dp mod p
	
	//mpz_set(xp, xp_ch);
	//mpz_set(xq, xq_ch);

	mpz_mul(xq, xq_ch, q);
	mpz_mul(xq, xq, yp);// xq = xq * q * yp
	mpz_mul(xp, xp_ch, p);
	mpz_mul(xp, xp, yq);

	mpz_add(result, xp, xq);
	mpz_mod(result, result, mod);
	mpz_clears(xp, xq, dp, dq, yp, yq, psub, qsub, NULL);
}

// 采用 Miller-Rabin 算法
bool MyRSA::isNotPrime(mpz_t &num) {
	//注释部分为使用 系统的方法 合数返回0,,不自己写了
	return (mpz_probab_prime_p(num, PRIME_NUM)==0)?true:false;
}

bool MyRSA::relaPrime(mpz_t& a, mpz_t& b,mpz_t& c, bool state)
{
	/*
	mpz_t r0, r1, q;
	mpz_init(q);
	if (mpz_cmp(a, b) > 0) {
		mpz_init_set(r0, a);
		mpz_init_set(r1, b);
	}
	else {
		mpz_init_set(r0, b);
		mpz_init_set(r1, a);
	}
	while (mpz_cmp_ui(r1, 0)!=0) {
		mpz_set(q, r1);
		mpz_fdiv_r(r1, r0, q);
		mpz_set(r0, q);
	}
	if (mpz_cmp_ui(r0, 1)) {
		mpz_clears(r0, r1, q, NULL);
		return true;
	}
	else {
		mpz_clears(r0, r1, q, NULL);
		return false;
	}*/
	mpz_t result;
	mpz_init(result);
	mpz_gcd(result , a, b);
	if (mpz_cmp_ui(result,1) != 0) {
		mpz_clear(result);
		return false;
	}
	else {
		mpz_clear(result);
		if (state) {
			return relaPrime(a, c, b, false);
		}
		else {
			return true;
		}
	}
	return true;//前面应用，防止问题
}

bool MyRSA::priCreat(mpz_t & a, mpz_t & b)
{
	mpz_t a0, b0, t0, t, temp, s0, s, q, r, temp2;
	mpz_inits(a0, b0, t0, t, temp, s0, s, q, r, temp2, NULL);
	mpz_set(a0, a);
	mpz_set(b0, b);
	mpz_set_ui(t0, 0);
	mpz_set_ui(t, 1);
	mpz_set_ui(s0, 1);
	mpz_set_ui(s, 0);
	mpz_fdiv_q(q, a0, b0);
	mpz_mul(temp, q, b0);
	mpz_sub(r, a0, temp);
	while (mpz_cmp_ui(r, 0) > 0) {
		mpz_mul(temp2, q, t);
		mpz_sub(temp, t0, temp2);
		mpz_set(t0, t);
		mpz_set(t, temp);
		mpz_mul(temp2, q, s);
		mpz_sub(temp, s0, temp2);
		mpz_set(s0, s);
		mpz_set(s, temp);
		mpz_set(a0, b0);
		mpz_set(b0, r);
		mpz_fdiv_q(q, a0, b0);
		mpz_mul(temp, q, b0);
		mpz_sub(r, a0, temp);
	}
	if (mpz_cmp_ui(b0, 1)) {
		mpz_clears(a0, b0, t0, t, temp, s0, s, q, r, temp2, NULL);
		return false;
	}
	else {
		mpz_set(pri, s);
		mpz_clears(a0, b0, t0, t, temp, s0, s, q, r, temp2, NULL);
		return true;
	}
}

//生成大素数
void MyRSA::bigPrime(LPVOID argue)
{
	struct arguement *infor = (struct arguement *)argue;
	do {
		mpz_rrandomb(*(infor->p), *(infor->state), 1024);
		mpz_setbit(*(infor->p), 0);//设置为奇数
	} while (isNotPrime(*(infor->p)));
	*(infor->ok) = true;
}

void MyRSA::MontMulti(mpz_t T, const mpz_t x, const mpz_t y, const  mpz_t N, const mp_limb_t N_1)
{
	/*  
	功能：计算x和y的蒙哥马利乘积，结果保存在T中，其中 0<=x，y<N
	N：模
	N_1:满足N*N_1=-1(mod 2^32)的整数
	*/

	unsigned int i;
	mp_limb_t num, carry, res[MONT_MAX] = { 0 };
	mp_limb_t *temp, t[MONT_MAX] = { 0 };

	//计算x和y的乘积，保存在t中，这里假设x和y均为蒙哥马利数
	if (x->_mp_size > y->_mp_size)
		mpn_mul(t, x->_mp_d, x->_mp_size, y->_mp_d, y->_mp_size);
	else
		mpn_mul(t, y->_mp_d, y->_mp_size, x->_mp_d, x->_mp_size);

	temp = t;
	for (i = 0; i < N->_mp_size; i++) {
		num = temp[0] * N_1;//num=t[0]*N_1
		res[i] = mpn_addmul_1(temp, N->_mp_d, N->_mp_size, num);//t=t+N*num,但是加法只做了N->_mp_size次，超出N->_mp_size长度的保存在res[i]中
		temp++;//相当于整除2^32
	}

	carry = mpn_add_n(temp, temp, res, N->_mp_size);//将上面步骤中所有没有处理的进位res[i]一次性地加到t上
	if (carry != 0 || mpn_cmp(temp, N->_mp_d, N->_mp_size) >= 0)//判断是否需要-N
		mpn_sub_n(temp, temp, N->_mp_d, N->_mp_size);

	mpz_import(T, N->_mp_size, -1, sizeof(mp_limb_t), 0, 0, temp);//将得到的结果保存在T中
}
void MyRSA::Mont_Exp(mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t N)
{
	/*  
	功能：利用蒙哥马利模乘，计算base^exp(mod N)，结果保存在rop中
	*/
	mp_limb_t N_1;
	mpz_t K, P, R, temp, N_inv, b;
	mp_bitcnt_t index;
	unsigned int bitnum = mpz_sizeinbase(exp, 2);
	unsigned int rbit = N->_mp_size*MONTBITS;//蒙哥马利中r的选择是和模的位数相关的，r=2^rbit。此处并不要求N一定是1024比特，1023，1022等，得到的rbit值都是一样的
	mpz_inits(K, P, R, temp, N_inv, b, NULL);

	mpz_setbit(b, MONTBITS);	//b = 2^32
	mpz_invert(N_inv, N, b);//
	mpz_sub(N_inv, b, N_inv);
	N_1 = *(N_inv->_mp_d);	//N*N_1 =-1 (mod 2^32)

	mpz_set_ui(temp, 1);
	mpz_setbit(K, 2 * rbit);//K=r^2
	mpz_mod(K, K, N);//保证0<=K<N

	MontMulti(P, K, base, N, N_1);//将base变成蒙哥马利数，P=K*base*r^-1=r^2*base*r^-1=base*r(mod N)，所以大家直接计算base<<rbit(modN)会更快一点
	MontMulti(R, K, temp, N, N_1);//将1变成蒙哥马利数，R==r^2*1*r^-1=r(mod N),所以大家直接计算1<<rbit(modN)会更快一点
	for (index = 0; index < bitnum; index++) {
		if (mpz_tstbit(exp, index) == 1)
			MontMulti(R, R, P, N, N_1);
		MontMulti(P, P, P, N, N_1);
	}
	MontMulti(rop, temp, R, N, N_1);//将R还原成普通整数，rop=1*R*r^-1=base^exp(mod N)

	mpz_clears(K, P, R, temp, N_inv, b, NULL);
}
