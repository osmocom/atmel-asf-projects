/**
 * \file
 *
 * \brief Implementation for TLS buffer manager.
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

#include "tls_buffer_manager.h"


/**********************************************************************
Function
	TLS_BufferRead

Description
	
Return
	u16Read
***********************************************************************/
uint16 TLS_BufferRead(tstrTlsBuffer	*pstrTlsBuff, uint16 u16ReadByteCount, uint8 *pu8ReadBuffer)
{
	uint16	u16Read = 0;
	
	if(pstrTlsBuff != NULL)
	{
		if((u16ReadByteCount + pstrTlsBuff->u16ReadOffset) < pstrTlsBuff->u16BuffSz)
		{
			u16Read = u16ReadByteCount;
		}
		else
		{
			u16Read = pstrTlsBuff->u16BuffSz - pstrTlsBuff->u16ReadOffset;
		}

		if(pu8ReadBuffer != NULL)
			M2M_MEMCPY(pu8ReadBuffer, &pstrTlsBuff->pu8Buff[pstrTlsBuff->u16ReadOffset], u16Read);
		
		pstrTlsBuff->u16ReadOffset += u16Read;
	}
	return u16Read;
}

/**********************************************************************
Function
	TLS_BufferReadCurrEntry

Description
	Reads the current entry from TLS buffer
Return
	u16Read
***********************************************************************/
uint16 TLS_BufferReadCurrEntry(tstrTlsBuffer *pstrTlsBuff, uint16 u16ReadByteCount, uint8 **ppu8Buffer, BOOL_T bUpdateCursor)
{
	uint16	u16Read = 0;
	
	if(pstrTlsBuff != NULL)
	{
		uint16	u16nBytes = pstrTlsBuff->u16BuffSz - pstrTlsBuff->u16ReadOffset;
		*ppu8Buffer = &pstrTlsBuff->pu8Buff[pstrTlsBuff->u16ReadOffset];
		if(u16nBytes > u16ReadByteCount)
		{				
			u16Read = u16ReadByteCount;
		}
		else
		{
			u16Read = u16nBytes;
		}
		if(bUpdateCursor == BTRUE)
			pstrTlsBuff->u16ReadOffset += u16Read;
	}
	return u16Read;
}

/**********************************************************************
Function
	TLS_BufferSetPos

Description
	
Return
	None
***********************************************************************/
void TLS_BufferSetPos(tstrTlsBuffer *pstrTlsBuff, tstrTLSBufferPos *pstrNewPos)
{
	if(pstrTlsBuff != NULL)
	{
		pstrTlsBuff->u16ReadOffset = pstrNewPos->u16BuffOffset;
	}
}

/**********************************************************************
Function
	TLS_BufferGetPos

Description
	
Return
	None
***********************************************************************/
void TLS_BufferGetPos(tstrTlsBuffer *pstrTlsBuff, tstrTLSBufferPos *pstrCurrPos)
{
	if(pstrTlsBuff != NULL)
	{
		pstrCurrPos->u16BuffOffset = pstrTlsBuff->u16ReadOffset;
	}	
}
