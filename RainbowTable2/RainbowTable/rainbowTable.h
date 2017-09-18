#pragma once

#include "stdafx.h"
#include <iostream>
#include<cstdio>
#include <direct.h>
#include <time.h>

//解释  filename R为彩虹表   AB为索引
#define FILENAME "RainbowTable\\R_X_AB"  
#define NTHASH_LENGTH 16
//执行次数 确保 文件一直
#define RAIN_NUM_RUN 1000
#define TEXT_NUM 91
//最大数据链条数
#define TEXT_NUM_STORE 10010000
using namespace std;
unsigned char PASSCHAR[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+[]\\{}|;':\",./<>?";
int LEN = TEXT_NUM;

/*
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
}*/

/*
inline void RT_mystrcpy(unsigned char *input, unsigned char * output) {
	int mystrcpy_i = 0;
	while (input[mystrcpy_i]) {
		output[mystrcpy_i] = input[mystrcpy_i];
		++mystrcpy_i;
	}
	output[mystrcpy_i] = '\0';
}*/

struct  RainTableTEXT
{
	unsigned char input[16];
	unsigned char output[16];
//	unsigned int value;
}text[TEXT_NUM_STORE],*text_P[TEXT_NUM_STORE];

char *ntlm_hash_lm_password(char *password);//test ok!
void NTHash(unsigned char *password, int length, uint32_t *output);//test ok!

bool RT_writeFile(int num, int length);
bool RT_createFile(int num, int length);
bool cmp_text(struct  RainTableTEXT *a, struct  RainTableTEXT*b, int num);


