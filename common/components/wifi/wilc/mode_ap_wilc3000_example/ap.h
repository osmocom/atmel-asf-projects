/**
 * \file
 *
 * \brief AP mode configuration.
 *
 * Copyright (c) 2018 Atmel Corporation. All rights reserved.
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

#ifndef AP_H_INCLUDED
#define AP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//#define    P2P_MODE_CONCURRENCY
/** P2P mode Settings */
#define MAIN_WLAN_DEVICE_NAME        "WILC_P2P" /* < P2P Device Name */
#define MAIN_WLAN_P2P_CHANNEL        (M2M_WIFI_CH_11) /* < P2P Channel number */

/** AP mode Settings */
#define MAIN_WLAN_SSID				"WILC3000" /* < SSID */
#define MAIN_WLAN_CHANNEL			M2M_WIFI_CH_6 /* < Channel number */
#define MAIN_WLAN_MAX_STA_COUNT		0 /* < Max supported stations for AP. Val 0 considered as default 8 stations, Val > 8 are considered upto 8 stations */

/** Security mode. Define one of the below */
//#define USE_SEC_OPEN /* < No Security */
//#define USE_WEP
#define USE_WPA_PSK


#if defined USE_WPA_PSK
#define  WPA_PSK_KEY				"12345678"

#elif defined USE_WEP
#define WEP_KEY_INDEX				M2M_WIFI_WEP_KEY_INDEX_1
#define WEP_KEY						"1234567890"//"0123456789abcdef0123456789"  /* < Security Key in WEP Mode. 10 digit or 26 digit */
#define WEP_AUTH_TYPE				WEP_ANY

#endif

void mode_ap(void *argument);
#ifdef __cplusplus
}
#endif

#endif /* AP_H_INCLUDED */