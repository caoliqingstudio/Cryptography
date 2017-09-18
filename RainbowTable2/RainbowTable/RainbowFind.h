#pragma once
#include "stdafx.h"
#include <iostream>


#define TEXT_NUM 91
#define TEXT_START 32
#define RAIN_NUM_RUN 1000

#define FILENAME "RainbowTable\\R_X_AB"  

using namespace std;


struct  RainTEXTF
{
	unsigned char input[16];
	unsigned char output[16];
	//	unsigned int value;
};

int readFile(int length, int num);
void NTHash(unsigned char *password, int length, uint32_t *output);//test ok!

int rainfind(unsigned char *input,int length,int num);
