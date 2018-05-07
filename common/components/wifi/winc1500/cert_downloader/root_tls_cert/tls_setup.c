/**
 *
 * \file
 *
 * \brief TLS certificate and private key parser and downloader
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
INCLUDESS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
#include "programmer/programmer.h"
#include "tls_setup.h"

/**********************************************************************
Function
	WriteTlsCertificate

Description
	Update the tls certificate to WINC SPI flash

Return
	Status
***********************************************************************/
int WriteTlsCertificate(uint8 *pu8PrivKey, uint32 u32PrivKeySz, tstrFileInfo *astrCertList, uint8 u8nCerts)
{
	int	ret = -1;
	uint32_t pu32SecSz;

	/* Read already loaded TLS Certificates from Flash.
	*/ 
	memset(gau8CertMem, 0, M2M_TLS_SERVER_FLASH_SIZE);
	ret = programmer_read_tls_cert_store(gau8CertMem);
	if(M2M_SUCCESS != ret) goto END;
	
	/*Modify the TLS Certificate Store Contents.
	*/
	ret = TlsSrvSecWriteCertChain(pu8PrivKey, u32PrivKeySz, astrCertList, u8nCerts, gau8CertMem, &pu32SecSz, TLS_SRV_SEC_MODE_APPEND);	
	if(M2M_SUCCESS != ret) goto END;

	/*
	Erase the TLS Certificate Section
	*/
	ret = programmer_erase_tls_cert_store();
	if(M2M_SUCCESS != ret) goto END;

	/*
	Write the TLS Certificate Section buffer to WINC stacked flash directly.
	*/
	printf("\r\n>> Writing the TLS Certificate to SPI flash...\r\n");
	ret = programmer_write_tls_cert_store(gau8CertMem);
	if(M2M_SUCCESS != ret) goto END;
	printf("--- TLS Certificate written to SPI flash ---\r\n\r\n");
#ifdef ENABLE_VERIFICATION //Enable verification

		{
			uint32			u32Idx;

			/* Verify Sector 1 */
			memset(gau8Verify, 0, FLASH_SECTOR_SZ);
			programmer_read(gau8Verify,M2M_TLS_SERVER_FLASH_OFFSET,FLASH_SECTOR_SZ);
	        
			for(u32Idx = 0; u32Idx < FLASH_SECTOR_SZ; u32Idx ++)
			{
				if(gau8CertMem[u32Idx] != gau8Verify[u32Idx])
				{
					printf(">> ERROR TLS Cert verification failed at Sector 1\n");
					ret = -1;
					break;
				}
			}

			/* Verify Sector 2 */
			memset(gau8Verify, 0, FLASH_SECTOR_SZ);
			programmer_read(gau8Verify,M2M_TLS_SERVER_FLASH_OFFSET+FLASH_SECTOR_SZ,FLASH_SECTOR_SZ);
			
			for(u32Idx = 0; u32Idx < FLASH_SECTOR_SZ; u32Idx ++)
			{
				if(gau8CertMem[u32Idx+FLASH_SECTOR_SZ] != gau8Verify[u32Idx])
				{
					printf(">> ERROR TLS Cert verification failed at Sector 2\n");
					ret = -1;
					break;
				}
			}
			printf(">> TLS Certificates Verified OK\r\n");
		}

#endif //Enable verification
	
END:
	return ret;
}
