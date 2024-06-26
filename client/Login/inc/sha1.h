/*
 *  sha1.h
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: sha1.h 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This class implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      Many of the variable names in the SHA1Context, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 *      Please read the file sha1.c for more information.
 *
 */

#ifndef _SHA1_H_
#define _SHA1_H_

#ifdef unix
#define SHA_DIGEST_LENGTH	20
#else
#define SHA_DIGEST_LENGTH	20
#endif

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned int state[5];
	unsigned int count[2];
	unsigned char  buffer[64];
} SHA1_CTX;

void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned long len);
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#ifdef  __cplusplus
}
#endif

#endif
