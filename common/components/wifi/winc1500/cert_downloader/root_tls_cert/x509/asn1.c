/**
 * \file
 *
 * \brief Implementation for ASN.1 Decoder for Distinguished Encoding Rules DER.
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

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "asn1.h"

/*********************************************************************
Function
	ASN1_GetNextElement

Description
	

Return
	None

Author
	Ahmed Ezzat

Version
	1.0

Date
	7 March 2013
*********************************************************************/
TLS_API uint16 ASN1_GetNextElement
(
tstrAsn1Context 	*pstrAsn1Ctxt,
tstrAsn1Element 	*pstrElement
)
{
	uint16	u16ElemLength = 0;
	
	if((pstrElement != NULL) && (pstrAsn1Ctxt != NULL))
	{
		uint8	u8NLenBytes;

		do
		{
			u8NLenBytes = 0;
			
			/* Get the ASN.1 Element Tag. 
			*/
			TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, 1, &pstrElement->u8Tag);

			/* Get the ASN.1 element length. 
			*/
			TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, 1, (uint8*)&pstrElement->u32Length);
			pstrElement->u32Length &= 0xFF;
			if(pstrElement->u32Length & BIT7)
			{
				uint8	u8Idx;
				uint8	au8Tmp[4];

				/* Multiple Length octets. 
				*/
				u8NLenBytes = pstrElement->u32Length & 0x03;
				pstrElement->u32Length = 0;
				TLS_BufferRead(pstrAsn1Ctxt->pstrTlsBuffer, u8NLenBytes, au8Tmp);
				
				for(u8Idx = 0 ; u8Idx < u8NLenBytes ; u8Idx ++)
					pstrElement->u32Length  += 
						(uint32)(au8Tmp[u8Idx] << ((u8NLenBytes - u8Idx - 1) * 8));
			}
			u16ElemLength += u8NLenBytes + 2 + pstrElement->u32Length;	
		}while(pstrElement->u8Tag == ASN1_NULL);
	}
	return u16ElemLength;
}
/*********************************************************************
Function
	ASN1_GetNextElement

Description
	

Return
	None

Author
	Ahmed Ezzat

Version
	1.0

Date
	25 Feb 2015
*********************************************************************/
TLS_API uint16 ASN1_Read
(
tstrAsn1Context	*pstrAsn1Cxt,
uint32			u32ReadLength,
uint8			*pu8ReadBuffer
)
{
	uint16	u16Read = 0;

	if((pstrAsn1Cxt != NULL) && (u32ReadLength > 0))
	{
		u16Read = TLS_BufferRead(pstrAsn1Cxt->pstrTlsBuffer, u32ReadLength, pu8ReadBuffer);
	}
	return u16Read;
}
