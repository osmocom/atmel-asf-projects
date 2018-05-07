/**
 * \file
 *
 * \brief Certificate Deecoder/parser
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

#include "crypto.h"
#include "tls_buffer_manager.h"
#include <string.h>
#include <stdlib.h>
#include "root_tls_cert/x509/x509_cert.h"
#include "crypto_api.h"

typedef struct __tstrCryptoMemHdr{
	struct __tstrCryptoMemHdr	*pstrNext;
}tstrCryptoMemHdr;

void* CryptoInternalMalloc(void *hdl, uint32 u32AllocSz);

/**********************************************************************
Function
	CryptoInternalMalloc

Description
	Allocates memory for crypto operation.

Return
	None
***********************************************************************/
void* CryptoInternalMalloc(void *hdl, uint32 u32AllocSz)
{
	txtrX509CertInfo	*pstrCert	= (txtrX509CertInfo*)hdl;
	tstrCryptoMemHdr	*pstrNew;
	uint8				*pu8Alloc;

	pu8Alloc = (uint8*)malloc(WORD_ALIGN(u32AllocSz) + sizeof(tstrCryptoMemHdr));
	if(pu8Alloc != NULL)
	{
		pstrNew = (tstrCryptoMemHdr*)pu8Alloc;

		if(pstrCert->pvPrivate == NULL)
		{
			pstrNew->pstrNext	= NULL;
		}
		else
		{
			pstrNew->pstrNext	= (tstrCryptoMemHdr*)pstrCert->pvPrivate;
		}
		pstrCert->pvPrivate	= (void*)pstrNew;

		pu8Alloc += sizeof(tstrCryptoMemHdr);
	}
	return (uint8*)pu8Alloc;
}

/**********************************************************************
Function
	CryptoX509CertDecode

Description
	Crypto certificate decoding

Return
	Status
***********************************************************************/
sint8 CryptoX509CertDecode(uint8 *pu8X509CertBuf, uint16 u16X509CertBuffer, txtrX509CertInfo *pstrCert, uint8 bDumpCert)
{
	sint8			ret			= M2M_ERR_FAIL;
	tstrMemPool		strPool;
	tstrX509Cert	strX509Cer;
	tstrTlsBuffer	strX509Buff;

	strX509Buff.pu8Buff			= pu8X509CertBuf;
	strX509Buff.u16BuffSz		= u16X509CertBuffer;
	strX509Buff.u16ReadOffset	= 0;

	strPool.fpAlloc			= CryptoInternalMalloc;
	strPool.pvPoolHandle	= pstrCert;
	pstrCert->pvPrivate		= NULL;
	if(X509Cert_Decode(&strX509Buff, u16X509CertBuffer, &strPool, &strX509Cer , bDumpCert) == X509_SUCCESS)
	{
		pstrCert->u8SerialNumberLength	= strX509Cer.u8SerialNumberLength;
		memcpy(pstrCert->au8SerialNo, strX509Cer.au8SerialNo, sizeof(strX509Cer.au8SerialNo));
		memcpy(&pstrCert->strExpiryDate, &strX509Cer.strExpiryDate, sizeof(tstrSystemTime));
		memcpy(&pstrCert->strStartDate, &strX509Cer.strStartDate, sizeof(tstrSystemTime));
		memcpy(&pstrCert->strIssuer, &strX509Cer.strIssuer, sizeof(tstrX520Name));
		memcpy(&pstrCert->strSubject, &strX509Cer.strSubject, sizeof(tstrX520Name));
		if(strX509Cer.strPubKey.enuType == PUBKEY_ALG_RSA)
		{
			pstrCert->strPubKey.enuCertKeyType = X509_CERT_PUBKEY_RSA;
			memcpy(&pstrCert->strPubKey.strRsaPub, &strX509Cer.strPubKey.strRSAKey, sizeof(tstrRSAPubKey));
		}
		else if(strX509Cer.strPubKey.enuType == PUBKEY_ALG_ECC)
		{
			pstrCert->strPubKey.enuCertKeyType				= X509_CERT_PUBKEY_ECDSA;
			pstrCert->strPubKey.strEcdsaPub.u16CurveID		= strX509Cer.strPubKey.strEccKey.pstrCurve->enuType;
			pstrCert->strPubKey.strEcdsaPub.u16EcPointSz	= strX509Cer.strPubKey.strEccKey.strQ.u16Size;
			memcpy(&pstrCert->strPubKey.strEcdsaPub.au8EcPoint, &strX509Cer.strPubKey.strEccKey.strQ.X, pstrCert->strPubKey.strEcdsaPub.u16EcPointSz * 2);
		}
		ret = M2M_SUCCESS;
	}
	return ret;
}

/**********************************************************************
Function
	CryptoX509DeleteContext

Description
	Free the allocated memory for certificate decoding

Return
	None
***********************************************************************/
void CryptoX509DeleteContext(txtrX509CertInfo *pstrCert)
{
	tstrCryptoMemHdr	*pstrCur = (tstrCryptoMemHdr*)pstrCert->pvPrivate;
	tstrCryptoMemHdr	*pstrDel;

	while(pstrCur != NULL)
	{
		pstrDel = pstrCur;
		pstrCur = pstrCur->pstrNext;
		free(pstrDel);
	}
}
