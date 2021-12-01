
#ifndef _MAC_H_
#define _MAC_H_

#ifdef __cplusplus
extern "C" {
#endif

	int mac_pboc_data_padding(unsigned char *input, int len, int pad_len);

	int mac_pboc_data_unpadding(unsigned char *input, int len);


	void mac_sm4mac_gen(unsigned char key[16], unsigned char iv[16], unsigned char * data, int len, unsigned char ret[17] );


#ifdef __cplusplus
}
#endif


#endif
