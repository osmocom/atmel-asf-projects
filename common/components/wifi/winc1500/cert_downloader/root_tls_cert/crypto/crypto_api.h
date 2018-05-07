/**
 * \file
 *
 * \brief Header file for cryptographic algorithms.
 *
 * Copyright (c) 2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef __CRYPTO_API_H__
#define __CRYPTO_API_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "common/include/nm_common.h"
#include "driver/include/m2m_types.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define CRYPTO_SHA1_DIGEST_SIZE			(20)
#define ECC_POINT_MAX					(72)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*!
@enum\
	tenuCertPubKeyType
*/
typedef enum{
	X509_CERT_PUBKEY_RSA	= 1,
	X509_CERT_PUBKEY_ECDSA	= 2
}tenuCertPubKeyType;


/*!
@struct	\
	tstrRsaPrivateKey
*/
typedef struct{
	uint16	u16NSize;
	uint16	u16eSize;
	uint16	u16dSize;
	uint16	u16PSize;
	uint16	u16QSize;
	uint16	u16dPSize;
	uint16	u16dQSize;
	uint16	u16QInvSize;
	uint32	u32Version;
	uint8	*pu8N;
	uint8	*pu8e;
	uint8	*pu8d;
	uint8	*pu8p;
	uint8	*pu8q;
	uint8	*pu8dP;
	uint8	*pu8dQ;
	uint8	*pu8QInv;
}tstrRsaPrivateKey;


/*!
@struct	\
	tstrASN1RsaPrivateKey

@brief	RSA Private Key Elements

	The structure contains the elements of the RSA Private key as decoded from
	the supplied private key file (PEM Format).
*/
typedef struct{
	tstrRsaPrivateKey	strRsaPrivKey;
	uint8				*pu8AllocPtr;
	uint16				u16KeyBufSz;
}tstrASN1RSAPrivateKey;


/*!
@struct	\
	tstrRSAPubKey
*/
typedef struct{
	uint16	u16NSize;
	uint16	u16ESize;
	uint8	*pu8N;
	uint8	*pu8E;
}tstrRSAPubKey;


/*!
@struct	\
	tstrECDSAPubKey
*/
typedef struct{
	uint16	u16CurveID;
	uint16	u16EcPointSz;
	uint8	au8EcPoint[ECC_POINT_MAX * 2];
}tstrECDSAPubKey;


/*!
@struct	\
	tstrX509CertPublicKey
*/
typedef struct{
	tenuCertPubKeyType	enuCertKeyType;
	union{
		tstrRSAPubKey	strRsaPub;
		tstrECDSAPubKey	strEcdsaPub;
	};
}tstrX509CertPublicKey;


/*!
@struct	\
	tstrX520Name

@brief
*/
typedef struct{
	char	acCmnName[64];
	uint8	au8NameSHA1[20];
}tstrX509Name;


/*!
@struct	\
	txtrX509CertInfo
*/
typedef struct{
	uint8					u8SerialNumberLength;
	uint8					au8SerialNo[64];
	tstrX509Name			strIssuer;
	tstrSystemTime			strExpiryDate;
	tstrSystemTime			strStartDate;
	tstrX509Name			strSubject;
	tstrX509CertPublicKey	strPubKey;
	void					*pvPrivate;
}txtrX509CertInfo;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

sint8 CryptoX509CertDecode(uint8 *pu8X509CertBuf, uint16 u16X509CertBuffer, txtrX509CertInfo *pstrCert, uint8 bDumpCert);
void CryptoX509DeleteContext(txtrX509CertInfo *pstrCert);
sint8 CryptoDecodeRsaPrivKey(uint8 *pu8RsaKeyFile, uint32 u32KeySize, tstrASN1RSAPrivateKey *pstrRsaPrivKey);
#endif /* __CRYPTO_API_H__ */