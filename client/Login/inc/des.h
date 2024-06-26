//#ifndef DES_H
//#define DES_H

#ifdef unix
#define DES_KEY_LEN_ERROR		0x1000001
#define DES_IN_DATA_LEN_ERROR	0x1000002
#else
#define DES_KEY_LEN_ERROR		0x1000001
#define DES_IN_DATA_LEN_ERROR	0x1000002
#endif

int DES_Encrypt(const char *key, unsigned char *in, unsigned int len, unsigned char *out);

int DES_Decrypt(const char *key, unsigned char *in,unsigned int len, unsigned char *out);