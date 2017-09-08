#include "stdafx.h"
#include "cryptanalysis.h"

Linear::Linear()
{
	An_key = NULL;
	num = 0;
	Linear_setKey = LINEAR_SETKEY;
/*	fopen_s(&file,"PCtext", "rb");
	if (!file) {
		printf("要破解的密钥,0使用默认，非0为输入的密钥\n");
		unsigned short temp;
		std::cin >> temp;
		Linear_setKey = temp == 0 ? Linear_setKey : temp;
		if (!creatFile(Linear_setKey)) {
			printf("fail");
		}
		fopen_s(file,"PCtext", "rb");
	}*/
// 	creatFile(Linear_setKey);
	fopen_s(&file,"PCtext", "rb");
}

Linear::~Linear()
{
	fclose(file);
	struct keyStore *next1;
	struct keyStore *next2;
	next1 = An_key;
	while (next1 != NULL) {
		next2 = next1->next;
		free(next1);
		next1 = next2;
	}
}

void Linear::analyse()
{
	time_t start, end;
	start = clock();
	Hex();
	end = clock();
	printf("%x %x %x %x\n", maxKey[4], maxKey[5], maxKey[6], maxKey[7]);
	std::cout << "破解16位密钥时间" << end - start << std::endl;
	start = clock();
	bigRun();
	end = clock();
	std::cout << "暴力时间" << end - start << std::endl;
	if (num==0)
	{
		std::cout << "fail to find key" << std::endl;
		return;
	}
	printf("得到密钥数量为： %d\n", num);
	struct keyStore *nextKey=An_key;
	for (int i = 0; i < num;++i) {
		printf("%x\n", nextKey->key);
		nextKey = nextKey->next;
	}
}

//ppt选择的是 a=0xB,b=0x4   映射 5 7
//我选的是 5 6 7  对应 1 3 9 11
bool Linear::Hex() 
{
	int count[16][16], count2[16][16];
	memset(count, 0, 16 * 16 * sizeof(int));
	memset(count2, 0, 16 * 16 * sizeof(int));
/*	MySPN myspn;
	myspn.setKey(Linear_setKey);
	myspn.setKeyDecrypt(Linear_setKey);
*/	srand((unsigned)time(NULL));
	for (int i = 0; i < T_NUM; i++)
	{
		unsigned short thisplaintext, thiscrytext, thistext[2];
//		thisplaintext = (unsigned short)rand();
//		thiscrytext = encrypt16(thisplaintext);
		fread(thistext, sizeof(unsigned short), 2, file);
		thisplaintext = thistext[0];
		thiscrytext = thistext[1];
/*		printf("%x\n", thisplaintext);
		printf("%x\n", thiscrytext);
		if (thiscrytext != myspn.encrypt16(thisplaintext)) {
			printf("asfdasdf \n");
		}
		unsigned short xxxx;
		if (thisplaintext != myspn.decrypt16(thiscrytext,xxxx)) {
			printf("asfdasdf \n");
		}
*/		for (int L1 = 0; L1 < 16; ++L1)
		{
			char temp1 = (char)L1;
			char temp[16];
			hex2char(temp1, temp);
			hex2char(temp1, temp+4);
			for (int L2 = 0; L2 < 16; ++L2)
			{
				char  V[16];
				char x[16] = { 0 };
				char temp2 = (char)L2;
				hex2char(temp2, temp + 8);
				hex2char(temp2, temp + 12);
				unsigned short text = thisplaintext;
				for (int i=0; i < 16; i++)
				{
					x[i] = ((char)text) & 0x01;
					text /= 2;
				}
				text = thiscrytext;
				for (int i = 0; i < 16; i++)
				{
					V[i] = ((char)text) & 0x01;
					text /= 2;
				}
				for (int i = 0; i < 16; i++)
				{
					V[i] = V[i] ^ temp[i];
				}
				sboxDecrypt_4(V);
				sboxDecrypt_4(V+4);
				sboxDecrypt_4(V+8);
				sboxDecrypt_4(V+12);
				if (!(x[4] ^ x[6] ^ x[7] ^ V[5] ^ V[7] ^ V[13] ^ V[15])) {
					count[L1][L2] ++;
				}
				if (!(x[5] ^ x[6] ^ x[7] ^ V[1] ^ V[3] ^ V[9] ^ V[11])){
					++count2[L1][L2];
				}
			}
		}
	}
	int max = -1;
	int max2 = -1;
	for (int L1 = 0; L1 < 16; ++L1)
	{
		for (int L2 = 0; L2 < 16; ++L2)
		{
			int x = count[L1][L2] - T_NUM / 2;
			count[L1][L2] = x > 0 ? x : -x;
			int y = count2[L1][L2] - T_NUM / 2;
			count2[L1][L2] = y > 0 ? y : -y;
			if (count[L1][L2] > max) {
				max = count[L1][L2];
				maxKey[5] =(char) L1;
				maxKey[7] =(char) L2;
			}
			if (count2[L1][L2] > max2) {
				max2 = count2[L1][L2];
				maxKey[4] = (char)L1;
				maxKey[6] = (char)L2;
			}
		}
	}
	return true;
}

inline void Linear::hex2char(unsigned char  x, char * y)
{
	y[0] = x & 0x1;
	y[1] = (x >> 1) & 0x1;
	y[2] = (x >> 2) & 0x1;
	y[3] = (x >> 3) & 0x1;
}

inline void Linear::sboxDecrypt_4(char * input)
{
	int j = input[0] * 8 + input[1] * 4 + input[2] * 2 + input[3];
	input[0] = sboxDecrypt_c[j][0];
	input[1] = sboxDecrypt_c[j][1];
	input[2] = sboxDecrypt_c[j][2];
	input[3] = sboxDecrypt_c[j][3];
}

bool Linear::bigRun()
{
	struct keyStore * storeKey;
	An_key = (struct keyStore *)malloc(sizeof(struct keyStore));
	An_key->next = NULL;
	if (An_key==NULL)
	{
		std::cout << "溢出" << std::endl;
		return false;
	}
	storeKey = An_key;
	char key[4];
	key[2] = ((maxKey[4]) & 0xf) | ((maxKey[5] << 4) & 0xf0);
	key[3] = ((maxKey[6]) & 0xf) | ((maxKey[7] << 4) & 0xf0);
	unsigned short thisplaintext, thiscrytext;
	unsigned short thisplaintext2, thiscrytext2;
	thisplaintext = 0x1748;
	thisplaintext2 = 0x7481;
	setKey(Linear_setKey);
	thiscrytext = encrypt16(thisplaintext);
	thiscrytext2 = encrypt16(thisplaintext2);
	for (unsigned short i = 0; i < 0xFFFF; ++i) {
		*((unsigned short *)key) = i;
		threadArgu.inputKey = *((unsigned long *)key);
		roundKeyCreat((LPVOID)&threadArgu);
		if (encrypt16(thisplaintext) == thiscrytext&&encrypt16(thisplaintext2) == thiscrytext2) {
			++num;
			storeKey->next = (struct keyStore *)malloc(sizeof(struct keyStore));
			if (storeKey->next == NULL)
			{
				std::cout << "溢出" << std::endl;
				return false;
			}
			storeKey->next->next = NULL;
			storeKey->key = *((unsigned long *)key);
			std::cout << num << std::endl;
			storeKey = storeKey->next;
		}
	}
	return true;
}

bool Linear::creatFile(unsigned long key)
{
	srand((unsigned)time(NULL));
	FILE *file;
	fopen_s(&file,"PCtext", "wb");
	if (!file) {
		std::cout << "creat fail" << std::endl;
		return false;
	}
	MySPN myspn;
	myspn.setKey(key);
	for (int i = 0; i < T_NUM; i++)
	{
		unsigned short thistext[2];
		thistext[0] = (unsigned short)rand();
		thistext[1] = myspn.encrypt16(thistext[0]);
		fwrite(thistext, sizeof(unsigned short), 2, file);
//		printf("%x\n", thistext[0]);
//		printf("%x\n", thistext[1]);
	}
	fclose(file);
	return true;
}

Differential::Differential()
{
	An_key = NULL;
	num = 0;
	textNum = 0;
	Diff_setKey = DIFF_SETKEY;
}

Differential::~Differential()
{
	struct keyStore *next1;
	struct keyStore *next2;
	next1 = An_key;
	while (next1 != NULL) {
		next2 = next1->next;
		free(next1);
		next1 = next2;
	}
}

void Differential::analyse()
{
	time_t start, end;
	start = clock();
	hex();
	end = clock();
	printf("%x %x %x %x\n", maxKey[4], maxKey[5], maxKey[6], maxKey[7]);
	std::cout << "16位密钥时间" << end - start << std::endl;
	start = clock();
	bigRun();
	end = clock();
	std::cout << "暴力时间" << end - start << std::endl;
	if (num == 0)
	{
		std::cout << "fail to find key" << std::endl;
		return;
	}
	printf("得到密钥数量为： %d\n", num);
	struct keyStore *nextKey = An_key;
	for (int i = 0; i < num; ++i) {
		printf("%x\n", nextKey->key);
		nextKey = nextKey->next;
	}
}

inline void Differential::sboxDecrypt_4(char * input)
{
	switch (*input)
	{
	case 0x0:*input = 0x7; break;
	case 0x1:*input = 0xE; break;
	case 0x2:*input = 0x8; break;
	case 0x3:*input = 0xD; break;
	case 0x4:*input = 0x2; break;
	case 0x5:*input = 0x9; break;
	case 0x6:*input = 0x5; break;
	case 0x7:*input = 0x0; break;
	case 0x8:*input = 0xC; break;
	case 0x9:*input = 0xB; break;
	case 0xa:*input = 0x3; break;
	case 0xb:*input = 0x4; break;
	case 0xc:*input = 0x1; break;
	case 0xd:*input = 0x6; break;
	case 0xe:*input = 0xf; break;
	case 0xf:*input = 0xa; break;
	default:break;
	}
}

bool Differential::hex()
{
	int count[16][16], count2[16][16];
	memset(count, 0, 16 * 16 * sizeof(int));
	memset(count2, 0, 16 * 16 * sizeof(int));
	srand((unsigned)time(NULL));
	MySPN myspn;
	myspn.setKey(Diff_setKey);
	for (int i = 0; i < T_DIFF_NUM; i++)
	{
		unsigned short text[4];
		text[0] = (unsigned short)rand();
		text[1] = text[0] ^ 0x0D00;
		text[2] = myspn.encrypt16(text[0]);
		text[3] = myspn.encrypt16(text[1]);

		char *temp = (char *)(text + 2);
		//ppt的 案例
		if (((temp[0] & 0x0f) == (temp[2] & 0x0f)) && ((temp[1] & 0x0f) == (temp[3] & 0x0f))) {
			for (int L1 = 0; L1 < 16; L1++)
			{
				for (int L2 = 0; L2 < 16; L2++)
				{
					char Vb2, Vb4, Va2, Va4;
					Va2 = (char)L1 ^ ((temp[0] >> 4) & 0xf);
					Va4 = (char)L2 ^ ((temp[1] >> 4) & 0xf);
					Vb2 = (char)L1 ^ ((temp[2] >> 4) & 0xf);
					Vb4 = (char)L2 ^ ((temp[3] >> 4) & 0xf);
					sboxDecrypt_4(&Va2);
					sboxDecrypt_4(&Vb2);
					sboxDecrypt_4(&Va4);
					sboxDecrypt_4(&Vb4);
					if(((Va2^Vb2) == 0x6)&& ((Va4^Vb4) == 0x6))
					{
						++count[L1][L2];
					}

				}
			}
		}
		int max = -1;
		for (int L1 = 0; L1 < 16; L1++)
		{
			for (int L2 = 0; L2 < 16; L2++)
			{
				if (count[L1][L2] > max) {
					max = count[L1][L2];
					maxKey[5] = (char)L1;
					maxKey[7] = (char)L2;
				}
			}
		}
	}
	return true;
}

bool Differential::bigRun()
{
	struct keyStore * storeKey;
	An_key = (struct keyStore *)malloc(sizeof(struct keyStore));
	An_key->next = NULL;
	if (An_key == NULL)
	{
		std::cout << "溢出" << std::endl;
		return false;
	}
	storeKey = An_key;
	char key[4];
	key[2] = ((maxKey[5] << 4) & 0xf0);
	key[3] = ((maxKey[7] << 4) & 0xf0);
	unsigned short thisplaintext, thiscrytext;
	unsigned short thisplaintext2, thiscrytext2;
	thisplaintext = 0x1748;
	thisplaintext2 = 0x7481;
	MySPN myspn;
	myspn.setKey(Diff_setKey);
	thiscrytext = myspn.encrypt16(thisplaintext);
	thiscrytext2 = myspn.encrypt16(thisplaintext2);
	for (unsigned int i = 0; i <= 0xFFFFFF; ++i) {
		//printf("%x\n", i);
		char c = *(((unsigned char *)&i) + 2);
		*((unsigned short *)key) = *((unsigned short *)&i);
		key[2] = (key[2] & 0xf0) | ((c) & 0x0f);
		key[3] = (key[3] & 0xf0) | ((c>>4) & 0x0f);
		myspn.threadArgu.inputKey = *((unsigned long *)key);
		myspn.roundKeyCreat((LPVOID)&myspn.threadArgu);
		if (myspn.encrypt16(thisplaintext) == thiscrytext&&myspn.encrypt16(thisplaintext2) == thiscrytext2) {
			++num;
			//printf("找到第%d组 密钥", num);
			storeKey->next = (struct keyStore *)malloc(sizeof(struct keyStore));
			if (storeKey->next == NULL)
			{
				std::cout << "溢出" << std::endl;
				return false;
			}
			storeKey->next->next = NULL;
			storeKey->key = *((unsigned long *)key);
			storeKey = storeKey->next;
		}
	}
	return true;
}

void Differential::bigRunThread(LPVOID argu)
{
}
