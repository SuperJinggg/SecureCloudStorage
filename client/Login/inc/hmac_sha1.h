#ifndef HMAC_SHA1_H
#define HMAC_SHA1_H

#ifdef  __cplusplus
extern "C" {
#endif

void hmac_sha
(
	char* k, /* secret key */
	int lk, /* length of the key in bytes */
	char* d, /* data */
	int ld, /* length of data in bytes */
	char* out /* output buffer, at least 41 bytes */
);

#ifdef  __cplusplus
}
#endif
#endif /*BASE64_ENC_H_*/
