#ifndef HMAC_SHA256_H
#define HMAC_SHA256_H

#ifdef unix
#define SHA256_DIGESTSIZE 32
#else
#define SHA256_DIGESTSIZE 32
#endif
#ifdef  __cplusplus
extern "C" {
#endif
void hmac_sha256
(
	char* k, /* secret key */
	int lk, /* length of the key in bytes */
	char* d, /* data */
	int ld, /* length of data in bytes */
	unsigned char* out /* output buffer, at least 64 bytes */
);


#ifdef  __cplusplus
}
#endif
#endif /*BASE64_ENC_H_*/
