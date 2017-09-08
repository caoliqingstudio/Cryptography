#include "stdafx.h"
#include "~~cryptanalysis.h"

Linear::Linear()
{
	for (int i = 0; i < 8000; i++)
	{
		An_text[i].state = false;
	}
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)creattest_run, (LPVOID)(An_text), 0, NULL);
}

Linear::~Linear()
{
}

void Linear::analyse()
{
	Hex57();
	printf("%x      %x", maxKey5, maxKey7);
}

void Linear::Hex57()
{
	int count[16][16];
	memset(count, 0, 16 * 16 * sizeof(int));
	for (int i = 0; i < 8000; i++)
	{
		while (!An_text[i].state);
		for (int L1 = 0; L1 < 16; ++L1)
		{
			char temp1 = (char)L1;
			for (int L2 = 0; L2 < 16; ++L2)
			{
				char  V2[4], V4[4];
				char x[4];
				char temp2;
				hex2char((An_text[i].plaintext >> 4) & 0x0f, x);
				unsigned int xxx = An_text[i].crytext;
				temp2 = (char)L2;
				hex2char((temp1 ^ (xxx >> 4)) & 0xf, V2);
				hex2char((temp2 ^ (xxx >> 12)) & 0xf, V4);
				sboxDecrypt(V2);
				sboxDecrypt(V4);
				if (!(x[0] ^ x[2] ^ x[3] ^ V2[1] ^ V2[3] ^ V4[1] ^ V4[3])) {
					count[L1][L2] ++;
				}
			}
		}
	}
	int max = -1;
	for (int L1 = 0; L1 < 16; ++L1)
	{
		for (int L2 = 0; L2 < 16; ++L2)
		{
			int x = count[L1][L2] - T_NUM / 2;
			count[L1][L2] = x > 0 ? x : -x;
			if (count[L1][L2] > max) {
				max = count[L1][L2];
				maxKey5 = (char)L1;
				maxKey7 = (char)L2;
			}
		}
	}

}

inline void Linear::hex2char(char  x, char * y)
{
	y[0] = x % 2;
	x /= 2;
	y[1] = x % 2;
	x /= 2;
	y[2] = x % 2;
	x /= 2;
	y[3] = x % 2;
}

bool Linear::creattest()
{
	return false;
}

void Linear::creattest_run(LPVOID arguement)
{
	time_t start = clock();
	int j = 0;
	MySPN myspn;
	myspn.setKey(LINEAR_SETKEY);
	struct Analyse_text * argu = (struct Analyse_text *)arguement;
	for (int i = 0; i < T_NUM; ++i)
	{
		srand((unsigned)time(NULL));
		argu[i].plaintext = (unsigned short)rand();
		argu[i].crytext = myspn.encrypt16(argu[j].plaintext);
		argu[i].state = true;
	}
	std::cout << (time_t)(clock() - start) << std::endl;
}
