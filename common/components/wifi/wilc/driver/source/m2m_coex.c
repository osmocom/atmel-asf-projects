/**
 *
 * \file
 *
 * \brief This module contains WILC Coexistence APIs implementation.
 *
 * Copyright (c) 2016-2018 Atmel Corporation. All rights reserved.
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
 #include "common/include/nm_common.h"
 
#ifdef CONF_WILC_USE_3000_REV_A

#include "driver/include/m2m_coex.h"
#include "driver/include/m2m_wifi.h"
#include "driver/source/m2m_hif.h"
#include "nmasic.h"

extern tuCtrlStruct guCtrlStruct;

NMI_API sint8  m2m_coex_set_mode(tenuCoexMode enuCoexMode)
{
	sint8 ret = M2M_SUCCESS;
	tstrM2mCoexMode* pstrM2mCoexMode = &guCtrlStruct.strM2mCoexMode;
	tstrM2mCoexNullFramesMode* pstrM2mCoexNullFramesMode = &guCtrlStruct.strM2mCoexNullFramesMode;

	switch(enuCoexMode)
	{
		case M2M_COEX_MODE_WIFI:
			pstrM2mCoexMode->enuNmiCoexMode = NMI_COEX_MODE_WIFI;
			ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_MODE, (uint8*)pstrM2mCoexMode,sizeof(tstrM2mCoexMode), NULL, 0, 0);
			if(ret == M2M_SUCCESS)
			{
				pstrM2mCoexNullFramesMode->enuNmiCoexNullFramesMode = NMI_COEX_NULL_FRAMES_OFF;
				ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_NULL_FRAMES_MODE, (uint8*)pstrM2mCoexNullFramesMode,sizeof(tstrM2mCoexNullFramesMode), NULL, 0, 0);
			}			
			break;
		case M2M_COEX_MODE_BT:
			pstrM2mCoexMode->enuNmiCoexMode = NMI_COEX_MODE_BT;
			ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_MODE, (uint8*)pstrM2mCoexMode,sizeof(tstrM2mCoexMode), NULL, 0, 0);
			if(ret == M2M_SUCCESS)
			{
				pstrM2mCoexNullFramesMode->enuNmiCoexNullFramesMode = NMI_COEX_NULL_FRAMES_OFF;
				ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_NULL_FRAMES_MODE, (uint8*)pstrM2mCoexNullFramesMode,sizeof(tstrM2mCoexNullFramesMode), NULL, 0, 0);
			}			
			break;
		case M2M_COEX_MODE_COMBO:
			pstrM2mCoexMode->enuNmiCoexMode = NMI_COEX_MODE_COMBO;
			ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_MODE, (uint8*)pstrM2mCoexMode,sizeof(tstrM2mCoexMode), NULL, 0, 0);
			if(ret == M2M_SUCCESS)
			{
				pstrM2mCoexNullFramesMode->enuNmiCoexNullFramesMode = NMI_COEX_NULL_FRAMES_ON;
				ret = hif_send(M2M_REQ_GRP_WIFI, M2M_WIFI_REQ_SET_COEX_NULL_FRAMES_MODE, (uint8*)pstrM2mCoexNullFramesMode,sizeof(tstrM2mCoexNullFramesMode), NULL, 0, 0);
			}			
			break;
	}

	return ret;
}

#endif /* CONF_WILC_USE_3000 */

