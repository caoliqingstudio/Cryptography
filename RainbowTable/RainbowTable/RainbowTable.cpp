#include "stdafx.h"
#include "rainbowTable.h"

// 32 -- 126 可打印 ASCII

int main() {
	RT_createFile(250000, 16);
	system("pause");
	return 0;
}

bool myRfunction(unsigned char *input,unsigned char *output,int s,int &num) {
	num = (int)((unsigned char)input[0] % 16)+1;
	//num = 4;
	int length = NTHASH_LENGTH / num;
	unsigned char key[20];
	int i;
	key[0] = input[0];
	for (i = 1; i < NTHASH_LENGTH; ++i) {
		key[i] = (unsigned char)(s%i + (unsigned char)input[i]);
	}
	for (i = 0; i<num; ++i) {
		unsigned char y = 0;
		for (int j = 0; j<length; ++j) {
			y += key[i*num + j];
		}
		output[i] = y % 95 + 32;
	}
	output[i] = 0;
	return true;
}

bool RT_createFile(int num , int length) {//生成彩虹表
	RTT_t Rbegin = &rainbowTableStart;
	Rbegin->door = NULL;
	Rbegin->text = NULL;
	srand((unsigned)time(NULL));
	RT_creatFile_child(Rbegin, num, length);
	RT_writeFile();
	return true;
}

void RT_creatFile_child(RTT_t Rbegin,int num,int length) {
	for (int i = 0; i < num; i++)
	{
		unsigned char input[20], output[20];
		int length_text;
		uint32_t temp[4];
		int j;
		RTT_t R_temp = Rbegin;
		for (j = 0; j < length; j++)
		{
			input[j] = ((unsigned char)rand()) % 92 + 32;
		}
		input[j] = '\0';
		NTHash(input, length, temp);
		myRfunction((unsigned char *)&temp, output, 0, length_text);
		for (j = 1; j < RAIN_NUM_RUN; j++)
		{
			NTHash(output, length_text, temp);
			myRfunction((unsigned char *)&temp, output, 0, length_text);
		}
		j = 0;
		while (output[j] != '\0')
		{
			int k = output[j] - 32;
			if (!(R_temp->door)) {
				R_temp->door = RTmalloc();
				if (R_temp->door == NULL) {
					cout << "溢出 RTT" << endl;
					system("pause");
					return;
				}
				for (int m = 0; m < TEXT_NUM; m++)
				{
					R_temp->door[m].text = NULL;
					R_temp->door[m].door = NULL;
				}
			}
			R_temp = &(R_temp->door[k]);
			j++;
		}
		RTT_C_t* RC_temp = &(R_temp->text);
		while (*RC_temp)
		{
			RC_temp = &((*RC_temp)->next);
		}
		(*RC_temp) = (RTT_C_t)malloc(sizeof(RTT_C));
		if (!(*RC_temp)) {
			cout << "溢出 RTT_C" << endl;
			return;
		}
		(*RC_temp)->next = NULL;
		(*RC_temp)->text = (unsigned char*)malloc(strlen((char *)input) + 1);
		if (!((*RC_temp)->text))
		{
			cout << "溢出 TEXT" << endl;
			return;
		}
		RT_mystrcpy(input, (*RC_temp)->text);
	}
}

bool RT_readFile(int startNum, int endNum) {//读取彩虹表
	RTT_t Rbegin = &rainbowTableStart;
	Rbegin->door = NULL;
	Rbegin->text = NULL;
	FILE *file;
	char filename[] = FILENAME;
	Rbegin->door = RTmalloc();
	if (Rbegin->door == NULL) {
		cout << "溢出 RTT" << endl;
		system("pause");
		return false;
	}
	for (int m = 0; m < TEXT_NUM; m++)
	{
		Rbegin->door[m].text = NULL;
		Rbegin->door[m].door = NULL;
	}
	for (int i = 0; i < TEXT_NUM; i++)
	{
		filename[15] = (((i >> 4) & 0x0000000f) + 'A');
		filename[16] = (((i) & 0x0000000f) + 'A');
		fopen_s(&file, filename, "w");
		if (!file) {
			cout <<filename<< "  文件打开失败" << endl;
			continue;
		}
		RT_firstRead(&(Rbegin->door[i]), file);
		fclose(file);
	}

	return false;
}
bool RT_findFile(char *input, char *output, int startNum, int endNum) {//查找
	return false;
}

bool RT_writeFile() {
	_mkdir("RainbowTable");
	RTT_t Rbegin = &rainbowTableStart;
	FILE *file;
	char filename[] = FILENAME;
	for (int i = 0; i < TEXT_NUM; i++)
	{
		filename[15] = (((i >> 4) & 0x0000000f) + 'A');
		filename[16] = (((i) & 0x0000000f) + 'A');
		fopen_s(&file, filename, "w");
		if (!file) {
			cout << "文件打开失败" << endl;
			return false;
		}
		RT_firstWrite(&(Rbegin->door[i]), file);
		fclose(file);
	}
	return true;
}


void RT_firstWrite(RTT_t fwx, FILE * thisfile) {
	RTT_C_t temp = fwx->text;
	while (temp)
	{
		int i = 0;
		while (temp->text[i]!='\0')
		{
			fputc(temp->text[i], thisfile);
			++i;
		}
		fputc(TEXT_END, thisfile);
		temp = temp->next;
	}
	fputc(TEXT_ALL_END, thisfile);
	if (fwx->door) {
		for (int i = 0; i < TEXT_NUM; i++)
		{
			RT_firstWrite(&(fwx->door[i]), thisfile);
		}
	}
	else {
		fputc(DOOR_NULL, thisfile);
	}
	return;
}

void RT_firstRead(RTT_t frx, FILE * thisfile) {
	unsigned char input[20];
	char text;
	RTT_t R_temp = frx;
	text = fgetc(thisfile);
	
	if (text != TEXT_NULL) {
		while (text!= TEXT_ALL_END) {
			int j = 0;
			while (text != TEXT_NULL) {
				input[j++] = text;
				text = fgetc(thisfile);
			}
			input[j] = '\0';

			RTT_C_t* RC_temp = &(R_temp->text);
			while (*RC_temp)
			{
				RC_temp = &((*RC_temp)->next);
			}
			(*RC_temp) = (RTT_C_t)malloc(sizeof(RTT_C));
			if (!(*RC_temp)) {
				cout << "溢出 RTT_C" << endl;
				return;
			}
			(*RC_temp)->next = NULL;
			(*RC_temp)->text = (unsigned char*)malloc(strlen((char *)input) + 1);
			if (!((*RC_temp)->text))
			{
				cout << "溢出 TEXT" << endl;
				return;
			}
			RT_mystrcpy(input, (*RC_temp)->text);
			text = fgetc(thisfile);
		}
	}
/*	while ()
	{
		int k = output[j] - 32;
		if (!(R_temp->door)) {
			R_temp->door = RTmalloc();
			if (R_temp->door == NULL) {
				cout << "溢出 RTT" << endl;
				system("pause");
				return;
			}
			for (int m = 0; m < TEXT_NUM; m++)
			{
				R_temp->door[m].text = NULL;
				R_temp->door[m].door = NULL;
			}
		}
		R_temp = &(R_temp->door[k]);
		j++;
	}
	*/
}