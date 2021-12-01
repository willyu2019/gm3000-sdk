
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "mac.h"
#include "sm4.h"

int	mac_pboc_data_padding(unsigned  char *input, int  len, int pad_len)
{
	int padding = len % pad_len;
	if(padding == 0)
	{
		memcpy(input + len, "\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",16);
		len += pad_len;
	}
	else
	{
		input[len] = 0x80;
		len ++;
		padding = len % pad_len;
		if(padding > 0)
		{
			memset(input + len,'\0', pad_len - padding);
			len = len + pad_len - padding;
		}
	}

	return(len);
}

int mac_pboc_data_unpadding(unsigned char *input, int len)
{
	int pos = -1;
	int i = 0;

	for( i = len - 1; i > 0 ; i --)
	{
		if(input[i] == 0x80) //padding start flag
		{
			pos = i;
			break;
		}
		if(input[i] != 0) //invalid data
		{
			return -1;
		}
	}

	return i; //original data length
}

void mac_sm4mac_gen(unsigned char key[16], unsigned char iv[16],
					unsigned char * data, int len,
					unsigned char ret[17] )
{
	int i,j;
	unsigned char  tmp[32] = {0};
	unsigned char tmp_data[10240] = {0};
	int newLen = 0;

	memcpy(tmp_data, data, len);

	newLen = mac_pboc_data_padding(tmp_data, len, 16);

	memcpy(tmp, iv, 16);

	for(i = 0 ; i < newLen ; i += 16)
	{
		for(j = 0 ; j < 16; j ++)
		{
			tmp[j] ^= tmp_data[i+j];
		}

		sm4_ecb(key,tmp, 16, SM4_ENCRYPT);
	}

	memcpy(ret, tmp, 4);

	return;
}
