#include "stdafx.h"
#include "rainbowTable.h"
#include "RainbowFind.h"

// 32 -- 126 可打印 ASCII
int numstate;
int stateFile;

int main() {
	int kkk;
	kkk = readFile(5, 0);
	int num = 0,realnum=0;
//	unsigned long long xxx[100];//1505388644
//	for (int i_time = 0; i_time < 100; i_time++)
//	{
		num = 0;
//		xxx[i_time] = (unsigned)time(NULL);
//		srand(xxx[i_time]);
		//srand(1505388644);
		srand((unsigned)time(NULL));
		for (int i = 0; i < 100; i++)
		{
			unsigned char input[20];
			for (int j = 0; j < 5; j++)
			{
				input[j] = PASSCHAR[(rand()) % LEN];
			}
			uint32_t temp[4];
			NTHash(input, 5, temp);
			if (rainfind((unsigned char *)temp, 5, kkk)) num++;
		}
		cout << num << endl;
//		cout << xxx[i_time] << endl;
/*		if (num > 60) {
			cout << "#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$        "<<i_time << endl;
			realnum++;
		}
	}*/
	system("pause");
	return 0;
}

int cmain() {
	int x, y;
	cout << "输入 x<=  < y" << endl;
	cin >> x >> y;
	for (int i = x; i < y; i++)
	{
		int m,x;
		switch (i)
		{
		case 1: m = 1; x = 1; break;
		case 2: m = 100; x = 1; break;
		case 3: m = 500; x = 1; break;
		case 4: m = 1000000; x = 1; break;//722//10000次的情况
		case 5: m = 10000000; x = 1; break;//65748.
		case 6: m = 10000000; x = 1; break;//5983099
		case 7: m = 20000000; x = 28; break;//544461944
		default:
			break;
		}
		int y = 0;
		while (y<x)
		{
			stateFile = y;
			RT_createFile(m, i);//有待研究
			cout << i <<"  "<<y<< "  OK" << endl;
			y++;
		}
	}
	system("pause");
	return 0;
}

void myRfunction(unsigned char *decrypt, unsigned char *encrypt, int k, int length)
{
	int i;
	unsigned char x, y;
	k = k%RAIN_NUM_RUN;
	for (i = 0; i<length; i++) {
		x = decrypt[i] ^ decrypt[(i + 1) & 0xf] ^ decrypt[(i + 2) & 0xf] ^ decrypt[(i + 3) & 0xf];
		y = decrypt[k & 0xf] + decrypt[(k - 1) & 0xf] + decrypt[(k - 2) & 0xf] + decrypt[(k - 3) & 0xf];
		encrypt[i] = PASSCHAR[(x + y) % LEN];
		k = k + k + k + 1;
	}
}

bool RT_createFile(int num , int length) {//生成彩虹表
	srand((unsigned)time(NULL));
	numstate = num;
	if (num >= TEXT_NUM_STORE) num = TEXT_NUM_STORE;
	for (int i = 0; i < num; i++)
	{
		text_P[i] = &text[i];
	}
	for (int i = 0; i < num; i++)
	{
		unsigned char *input, *output;
		input = text_P[i]->input;
		output = text_P[i]->output;
		uint32_t temp[4];
		int j;
		for (j = 0; j < length; j++)
		{
			input[j] = PASSCHAR[(rand()) % LEN];
		}
		NTHash(input, length, temp);
		myRfunction((unsigned char *)temp, output, 0, length);
		for (j = 1; j < RAIN_NUM_RUN; j++)
		{
			NTHash(output, length, temp);
			myRfunction((unsigned char *)temp, output, j, length);
		}
	}

	time_t start, tend;
	start = clock();
	int stateNUM = 10000;
	for (int i = 0; i < num - 1; i++) {
		if (i == stateNUM) {
			tend = clock();
			cout << stateNUM << "  用时  " << tend - start << endl;
			stateNUM += 10000;
			start = clock();
		}
		for (int j = 0; j < num - i - 1; j++) {
			if (cmp_text(text_P[j],text_P[j + 1],num)) {
				struct RainTableTEXT* temp = text_P[j];
				text_P[j] = text_P[j + 1];
				text_P[j + 1] = temp;
			}
		}
	}
	RT_writeFile(num,length);
	return true;
}

//a> b true  
bool cmp_text(struct  RainTableTEXT *a, struct  RainTableTEXT*b, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (a->output[i] > b->output[i]) {
			return true;
		}
		else if (a->output[i] < b->output[i])
		{
			return false;
		}
	}
	if (i==num)
	{
		int j;
		for (j = 0; j < num; ++j) {
			if (a->input[j]!=b->input[j])
			{
				break;
			}
		}
		if (j == num) {
			b->input[0] = '\0';
			--numstate;
		}
	}
	return false;
}


bool RT_writeFile(int num, int length) {
	FILE *file;
	char filename[] = FILENAME;
	filename[15] = length + 'A';
	filename[17] = stateFile / 52 + 'A';
	filename[18] = stateFile % 52 + 'A';
	fopen_s(&file, filename, "w");
	if (!file) {
		cout << "文件打开失败" << endl;
		return false;
	}
	fwrite(&numstate, sizeof(unsigned int), 1, file);
	for (int i = 0; i < num; i++)
	{
		if (text_P[i]->input[0] != '\0') {
			fwrite(text_P[i]->output, sizeof(char), length, file);
			fwrite(text_P[i]->input, sizeof(char), length, file);
		}
	}
	fclose(file);
	return true;
}
