#include "../inc/sha1.h"
#include "../inc/hmac_sha1.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

//#ifndef SHA_DIGESTSIZE
//#define SHA_DIGESTSIZE 20
//#endif

//#ifndef SHA_BLOCKSIZE
//#define SHA_BLOCKSIZE 64
//#endif

#ifdef unix
#define SHA_DIGESTSIZE 20
#define SHA_BLOCKSIZE 64
#else
#define SHA_DIGESTSIZE 20
#define SHA_BLOCKSIZE 64
#endif

/* Function to print the digest */
/*void pr_sha(FILE* fp, unsigned char* s, int t)
{
    int i ;

	for(i=0;i<t;i++)
		printf("%02X",s[i]);	
	printf("\n");
	/*
	fprintf(fp, "0x") ;
	for (i = 0 ; i < t ; i++)
	{
		fprintf(fp, "%02x", s[i]) ;
		printf("\n");
	}
	fprintf(fp, "0") ;
	*/
//}

//void truncate
//(
//	char* d1, /* data to be truncated */
//	char* d2, /* truncated data */
//	int len /* length in bytes to keep */
//)
//{
//	int i ;
//	for (i = 0 ; i < len ; i++) d2[i] = d1[i];
//}

/* Function to compute the digest */
void hmac_sha
(
	char* k, /* secret key */
	int lk, /* length of the key in bytes */
	char* d, /* data */
	int ld, /* length of data in bytes */
	char* out /* output buffer, at least "t" bytes */
)
{
	SHA1_CTX ictx, octx ;
	char isha[SHA_DIGESTSIZE], osha[SHA_DIGESTSIZE] ;
	char key[SHA_DIGESTSIZE] ;
	char buf[SHA_BLOCKSIZE] ;
	char strosha[2*SHA_BLOCKSIZE+2];
	int i ;

	if (lk > SHA_BLOCKSIZE) {

	SHA1_CTX tctx;

	SHA1Init(&tctx);
	SHA1Update(&tctx, k, lk);
	SHA1Final(key, &tctx);

	k = key;
	lk = SHA_DIGESTSIZE;
}

/**** Inner Digest ****/

	SHA1Init(&ictx) ;

	/* Pad the key for inner digest */
	for (i = 0 ; i < lk ; ++i) buf[i] = k[i] ^ 0x36 ;
	for (i = lk ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x36 ;

	SHA1Update(&ictx, buf, SHA_BLOCKSIZE) ;
	SHA1Update(&ictx, d, ld) ;

	SHA1Final(isha, &ictx) ;

/**** Outter Digest ****/

	SHA1Init(&octx) ;

/* Pad the key for outter digest */

	for (i = 0 ; i < lk ; ++i) buf[i] = k[i] ^ 0x5C ;
	for (i = lk ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x5C ;

	SHA1Update(&octx, buf, SHA_BLOCKSIZE) ;
	SHA1Update(&octx, isha, SHA_DIGESTSIZE) ;

	SHA1Final(osha, &octx) ;

	/* truncate and print the results */
	//t = t > SHA_DIGESTSIZE ? SHA_DIGESTSIZE : t ;
	//truncate(osha, out, t) ;
	
	for (i = 0; i < SHA_DIGESTSIZE; i++)
    {
		sprintf(&(strosha[2*i]),"%02X",(unsigned char)osha[i]);
		sprintf(&(strosha[2*i+1]),"%02X",(unsigned char)(osha[i]<<4));
     }
	strosha[2*SHA_DIGESTSIZE] = '\0';
	strcpy(out, strosha);

	return;
}
