#include "stdafx.h"
#include "RainbowFind.h"


struct  RainTEXTF textF[20000000];
unsigned char PASSCHARF[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+[]\\{}|;':\",./<>?";
int LENF = TEXT_NUM;


void myRfunctionF(unsigned char *decrypt, unsigned char *encrypt, int k, int length)
{
	int i;
	unsigned char x, y;
	k = k%RAIN_NUM_RUN;
	for (i = 0; i<length; i++) {
		x = decrypt[i] ^ decrypt[(i + 1) & 0xf] ^ decrypt[(i + 2) & 0xf] ^ decrypt[(i + 3) & 0xf];
		y = decrypt[k & 0xf] + decrypt[(k - 1) & 0xf] + decrypt[(k - 2) & 0xf] + decrypt[(k - 3) & 0xf];
		encrypt[i] = PASSCHARF[(x + y) % LENF];
		k = k + k + k + 1;
	}
}
int readFile(int length, int num) {
	FILE *file;
	int i=0;
	int textnum;
	char filename[] = FILENAME;
	filename[15] = length + 'A';
	filename[17] = num / 52 + 'A';
	filename[18] = num % 52 + 'A';
	fopen_s(&file, filename, "r");
	if (!file) {
		cout << "文件打开失败" << endl;
		return false;
	}
	fread(&textnum, sizeof(int), 1, file);
	for (i = 0; i < textnum; i++){
		fread(textF[i].output, sizeof(unsigned char), length, file);
		fread(textF[i].input, sizeof(unsigned char), length, file);
	}
	return i;
}


bool cmp_text_high(struct  RainTEXTF *a, unsigned char*b, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (a->output[i] > b[i]) {
			return true;
		}
		else if (a->output[i] < b[i])
		{
			return false;
		}
	}
	return false;
}
bool cmp_text_low(struct  RainTEXTF *a, unsigned char*b, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (a->output[i] < b[i])
		{
			return true;
		}
		else if (a->output[i] > b[i]) {
			return false;
		}
	}
	return false;
}
bool cmp_text_equ(struct  RainTEXTF *a, unsigned char*b, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (a->output[i] != b[i]) {
			return false;
		}
	}
	return true;
}

bool cmp_text_char(unsigned char *a, unsigned char*b, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (a[i] > b[i]) {
			return false;
		}
		else if (a[i] < b[i])
		{
			return false;
		}
	}
	return true;
}
int rainfind(unsigned char *input,int length,int num) {
	//for (int i = RAIN_NUM_RUN - 1; i >= 0; --i)
	for (int i = 0; i < RAIN_NUM_RUN; ++i)
	{
		uint32_t temp[4];
		unsigned char output[20];
		myRfunctionF((unsigned char *)input, output, i, length);
		for (int j = i+1; j < RAIN_NUM_RUN; j++)
		{
			NTHash(output, length, temp);
			myRfunctionF((unsigned char *)temp, output, j, length);
		}
		int low = 0;
		int high = num-1;
		int mid;
		bool state = false;
		while (low <= high) {
			mid = (low + high) / 2;
			if (cmp_text_high(&textF[mid], output, length)) {
				high = mid - 1;
			}
			else if (cmp_text_low(&textF[mid], output, length)) {
				low = mid + 1;
			}
			else {
				state = true;
				break;
			}
		}
		if (state) {
			int first = mid;
			while (first>=0&&cmp_text_equ(&textF[first], output, length)) {
				first--;
			}
			first++;
			while (first<num&&cmp_text_equ(&textF[first], output, length)) {
				unsigned char  coutput[20];
				for (int outi = 0; outi <16; outi++)
				{
					coutput[outi] = textF[first].input[outi];
				}
				NTHash(coutput, length, temp);
				for (int j = 0; j < i; j++)
				{
					myRfunctionF((unsigned char *)temp, coutput, j, length);
					NTHash(coutput, length, temp);
				}
				if (cmp_text_char((unsigned char *)temp,input,16))
				{
					for (int k = 0; k < length; k++)
					{
						cout<<coutput[k];
					}
					cout << endl;
					return true;
				}
				first++;
			}
		}
	}
	cout << "fail" << endl;
	return false;
}