#pragma once

#include "stdafx.h"
#include <iostream>
#include<cstdio>
#include <direct.h>
#include <time.h>

//解释  filename R为彩虹表   AB为索引
#define FILENAME "RainbowTable\\R_AB"  
#define NTHASH_LENGTH 16
#define RAIN_NUM_RUN 8
#define TEXT_NUM 95
#define TEXT_NULL 1
#define DOOR_NULL 2
#define TEXT_END 4
#define TEXT_ALL_END 3

using namespace std;

typedef struct rainbowTableTextChild {
	unsigned char * text;
	struct rainbowTableTextChild *next;
} RTT_C , *RTT_C_t;
typedef struct rainbowTableText
{
	struct rainbowTableText * door;
	RTT_C_t text;
} RTT,* RTT_t;

RTT rainbowTableStart;

inline RTT_t  RTmalloc() {
	return (RTT_t )malloc(sizeof(RTT) * TEXT_NUM);
}

inline void RT_mystrcpy(unsigned char *input, unsigned char * output) {
	int mystrcpy_i = 0;
	while (input[mystrcpy_i]) {
		output[mystrcpy_i] = input[mystrcpy_i];
		++mystrcpy_i;
	}
	output[mystrcpy_i] = '\0';
}

void RT_creatFile_child(RTT_t Rbegin, int num, int length);
void RT_firstWrite(RTT_t fwx,FILE * thisfile);
void RT_firstRead(RTT_t frx, FILE * thisfile);
char *ntlm_hash_lm_password(char *password);//test ok!
void NTHash(unsigned char *password, int length, uint32_t *output);//test ok!
bool myRfunction(unsigned char *input, unsigned char *output,int s,int &num);

bool RT_writeFile();
bool RT_createFile(int num, int length);
bool RT_readFile(int startNum, int endNum);
bool RT_findFile(char *input, char *output, int startNum, int endNum);