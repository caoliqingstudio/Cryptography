#include "stdafx.h"
#include "xcrypt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MIN(a,b) ((a<b)?(a):(b))
#define VAL(var, type, offset)	*((type *)(var+offset))
#define MEM(var, type, offset)	(type *)(var+offset)

/*
 * Shortcut for malloc/memset zero.
 */
char *newbuf(size_t size) {
	char *tmp;

	tmp = (char*)malloc(size);
	memset(tmp, 0, size);

	return tmp;
}

/*
 * Self-explanatory.
 */
char *lowercase(char *str) {
	unsigned int i;

	for (i = 0; i < strlen(str); ++i)
		str[i] = tolower(str[i]);

	return str;
}

/*
 * Self-explanatory.
 */
char *uppercase(char *str) {
	unsigned int i;

	for (i = 0; i < strlen(str); ++i)
		str[i] = toupper(str[i]);

	return str;
}

int unicode(char **dst, char *src) {
	char *tmp;
	int l, i;

	if (!src) {
		*dst = NULL;
		return 0;
	}

	l = MIN(64, strlen(src));
	tmp = newbuf(2*l);
	for (i = 0; i < l; ++i)
		tmp[2*i] = src[i];

	*dst = tmp;
	return 2*l;
}
static void ntlm_set_key(unsigned char *src, gl_des_ctx *context) {
	char key[8];

	key[0] = src[0];
	key[1] = ((src[0] << 7) & 0xff) | (src[1] >> 1);
	key[2] = ((src[1] << 6) & 0xff) | (src[2] >> 2);
	key[3] = ((src[2] << 5) & 0xff) | (src[3] >> 3);
	key[4] = ((src[3] << 4) & 0xff) | (src[4] >> 4);
	key[5] = ((src[4] << 3) & 0xff) | (src[5] >> 5);
	key[6] = ((src[5] << 2) & 0xff) | (src[6] >> 6);
	key[7] = (src[6] << 1) & 0xff;

	gl_des_setkey(context, key);
}
char *ntlm_hash_lm_password(char *password) {
	char magic[8] = {0x4B, 0x47, 0x53, 0x21, 0x40, 0x23, 0x24, 0x25};
	gl_des_ctx context;
	char *keys, *pass;

	keys = newbuf (21 + 1);
	pass = newbuf (14 + 1);
	strncpy_s(pass, 15, password, MIN(14, strlen(password)));
	uppercase(pass);

	ntlm_set_key(MEM(pass, unsigned char, 0), &context);
	gl_des_ecb_encrypt(&context, magic, keys);

	ntlm_set_key(MEM(pass, unsigned char, 7), &context);
	gl_des_ecb_encrypt(&context, magic, keys+8);

	memset(keys+16, 0, 5);
	memset(pass, 0, 14);
	free(pass);

	return keys;
}

char *ntlm_hash_nt_password(char *password) {
	char *u16, *keys;
	int len;

	keys = newbuf (21 + 1);
	len = unicode(&u16, password);
	md4_buffer(u16, len, keys);

	memset(keys+16, 0, 5);
	memset(u16, 0, len);
	free(u16);

	return keys;
}

char *ntlm2_hash_password(char *username, char *domain, char *password) {
	char *tmp, *buf, *passnt, *passnt2;
	int len;

	passnt = ntlm_hash_nt_password(password);

	buf = newbuf (strlen(username)+strlen(domain) + 1);
	strcat_s(buf,sizeof(buf),username);
	strcat_s(buf, sizeof(buf), domain);
	uppercase(buf);
	len = unicode(&tmp, buf);

	passnt2 = newbuf (16 + 1);
	hmac_md5(passnt, 16, tmp, len, passnt2);

	free(passnt);
	free(tmp);
	free(buf);

	return passnt2;
}
const uint32_t INIT_A = 0x67452301;
const uint32_t INIT_B = 0xefcdab89;
const uint32_t INIT_C = 0x98badcfe;
const uint32_t INIT_D = 0x10325476;
const uint32_t SQRT_2 = 0x5a827999;
const uint32_t SQRT_3 = 0x6ed9eba1;

void NTHash(unsigned char *password, int length, uint32_t *output)
{
	uint32_t nt_buffer[16] = { 0 };
	uint8_t i = 0;

	for (; i < length / 2; i++)
		nt_buffer[i] = password[2 * i] | (password[2 * i + 1] << 16);

	// Padding
	if (length % 2 == 1)
		nt_buffer[i] = password[length - 1] | 0x800000;
	else
		nt_buffer[i] = 0x80;

	nt_buffer[14] = length << 4;

	uint32_t a = INIT_A;
	uint32_t b = INIT_B;
	uint32_t c = INIT_C;
	uint32_t d = INIT_D;

	/* Round 1 */
	a += (d ^ (b & (c ^ d))) + nt_buffer[0]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[1]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[2]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[3]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[4]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[5]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[6]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[7]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[8]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[9]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[10]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[11]; b = (b << 19) | (b >> 13);

	a += (d ^ (b & (c ^ d))) + nt_buffer[12]; a = (a << 3) | (a >> 29);
	d += (c ^ (a & (b ^ c))) + nt_buffer[13]; d = (d << 7) | (d >> 25);
	c += (b ^ (d & (a ^ b))) + nt_buffer[14]; c = (c << 11) | (c >> 21);
	b += (a ^ (c & (d ^ a))) + nt_buffer[15]; b = (b << 19) | (b >> 13);

	/* Round 2 */
	a += ((b & (c | d)) | (c & d)) + nt_buffer[0] + SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[4] + SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[8] + SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[12] + SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[1] + SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[5] + SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[9] + SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[13] + SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[2] + SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[6] + SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[10] + SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[14] + SQRT_2; b = (b << 13) | (b >> 19);

	a += ((b & (c | d)) | (c & d)) + nt_buffer[3] + SQRT_2; a = (a << 3) | (a >> 29);
	d += ((a & (b | c)) | (b & c)) + nt_buffer[7] + SQRT_2; d = (d << 5) | (d >> 27);
	c += ((d & (a | b)) | (a & b)) + nt_buffer[11] + SQRT_2; c = (c << 9) | (c >> 23);
	b += ((c & (d | a)) | (d & a)) + nt_buffer[15] + SQRT_2; b = (b << 13) | (b >> 19);

	/* Round 3 */
	a += (d ^ c ^ b) + nt_buffer[0] + SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[8] + SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[4] + SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[12] + SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[2] + SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[10] + SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[6] + SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[14] + SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[1] + SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[9] + SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[5] + SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[13] + SQRT_3; b = (b << 15) | (b >> 17);

	a += (d ^ c ^ b) + nt_buffer[3] + SQRT_3; a = (a << 3) | (a >> 29);
	d += (c ^ b ^ a) + nt_buffer[11] + SQRT_3; d = (d << 9) | (d >> 23);
	c += (b ^ a ^ d) + nt_buffer[7] + SQRT_3; c = (c << 11) | (c >> 21);
	b += (a ^ d ^ c) + nt_buffer[15] + SQRT_3; b = (b << 15) | (b >> 17);

	output[0] = a + INIT_A;
	output[1] = b + INIT_B;
	output[2] = c + INIT_C;
	output[3] = d + INIT_D;
}
