#include "../inc/sha256.h"
#include "../inc/hmac_sha256.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifndef SHA_BLOCKSIZE
#define SHA_BLOCKSIZE 64
#endif

/* Function to compute the digest */
void hmac_sha256
(
	char* k, /* secret key */
	int lk, /* length of the key in bytes */
	char* d, /* data */
	int ld, /* length of data in bytes */
	unsigned char* out /* output buffer, at least "t" bytes */
)
{
	sha256_context ictx, octx;
	char *pk = NULL;
	unsigned char isha[SHA256_DIGESTSIZE], osha[SHA256_DIGESTSIZE] ;
	char key[SHA256_DIGESTSIZE] ;
	char buf[SHA_BLOCKSIZE] ;
	char strosha[2*SHA_BLOCKSIZE+16];
	int i, keylen;

	keylen = lk;
	pk = (char *)malloc(keylen*sizeof(char*));

	if (keylen > SHA_BLOCKSIZE) 
	{
		sha256_context tctx;

		//SHA1Init(&tctx);
		//SHA1Update(&tctx, k, lk);
		//SHA1Final(key, &tctx);
		sha256_starts(&tctx);
		sha256_update(&tctx,k,keylen);
		sha256_finish(&tctx, key);
		
		//k = key;
		memcpy(pk, key, SHA256_DIGESTSIZE);
		keylen = SHA256_DIGESTSIZE;
	}
	else
	{
		memcpy(pk, k, keylen);
	}

	/**** Inner Digest ****/

	sha256_starts(&ictx);

	/* Pad the key for inner digest */
	for (i = 0 ; i < keylen ; ++i) buf[i] = pk[i] ^ 0x36;
	for (i = keylen ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x36 ;

	sha256_update(&ictx,buf, SHA_BLOCKSIZE);
	sha256_update(&ictx,d, ld);
	sha256_finish(&ictx, isha);
	/*for(i = 0;i<SHA256_DIGESTSIZE;i++)
	{
		printf("%02X", isha[i]);
	}
	printf("\n");*/

	/**** Outter Digest ****/

	sha256_starts(&octx);

	/* Pad the key for outter digest */
	for (i = 0 ; i < keylen ; ++i) buf[i] = pk[i] ^ 0x5C ;
	for (i = keylen ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x5C ;

	sha256_update(&octx, buf, SHA_BLOCKSIZE);
	sha256_update(&octx, isha, SHA256_DIGESTSIZE);
	sha256_finish(&octx, osha);
	
	/*for(i = 0;i<SHA256_DIGESTSIZE;i++)
	{
		printf("%02X", osha[i]);
	}
	printf("\n");*/
	
	/*for (i = 0; i < SHA256_DIGESTSIZE; i++)
    {
		sprintf(&(strosha[2*i]),"%02X",osha[i]);
		sprintf(&(strosha[2*i+1]),"%02X",(unsigned char)(osha[i]<<4));
     }
	strosha[2*SHA256_DIGESTSIZE] = '\0';
	strcpy(out, strosha);*/

	memcpy(out, osha, SHA256_DIGESTSIZE);

	if(pk != NULL)
	{
		free(pk);
		pk = NULL;
	}

	return;
}
