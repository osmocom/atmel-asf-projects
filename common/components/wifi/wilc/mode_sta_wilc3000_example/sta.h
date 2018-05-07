/**
 * \file
 *
 * \brief STA mode configuration.
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

#ifndef STA_H_INCLUDED
#define STA_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Define a Security type */
//#define 	STA_MODE_OPEN_SECURITY
/*!< Wi-Fi network is not secured.
*/
//#define  	STA_MODE_WPA_WPA2_PERSONAL_SECURITY
/*!< Wi-Fi network is secured with WPA/WPA2 personal(PSK).
*/
#define 	STA_MODE_WEP_SECURITY
/*!< Security type WEP (40 or 104) OPEN OR SHARED.
*/
//#define 	STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY
 /*!< Wi-Fi network is secured with WPA/WPA2 Enterprise.IEEE802.1x user-name/password authentication.
 */

//#define    P2P_MODE_CONCURRENCY
/** P2P mode Settings */
#define MAIN_WLAN_DEVICE_NAME        "WILC_P2P" /* < P2P Device Name */
#define MAIN_WLAN_P2P_CHANNEL        (M2M_WIFI_CH_11) /* < P2P Channel number */

/** Wi-Fi Settings */
#define MAIN_WLAN_SSID        "DEMO_AP" /* < Destination SSID */
#if defined(STA_MODE_WPA_WPA2_PERSONAL_SECURITY)
#define MAIN_WLAN_PSK         "12345678" /* < Password for Destination SSID */

#elif defined(STA_MODE_WEP_SECURITY)
//#define WEP_SEC_TYPE_KEY_40
#define WEP_SEC_TYPE_KEY_104
#define MAIN_WLAN_WEP_KEY_INDEX         1 /**< WEP key index */
#define MAIN_WLAN_WEP_AUTH_TYPE   	 	WEP_ANY
#if defined(WEP_SEC_TYPE_KEY_40)
#define MAIN_WLAN_WEP_KEY               "1234567890" /**< 64 bit WEP key. In case of WEP64, 10 hexadecimal (base 16) characters (0-9 and A-F) ) */
#elif defined(WEP_SEC_TYPE_KEY_104)
#define MAIN_WLAN_WEP_KEY	            "1234567890abcdef1234567890" /**< 128 bit WEP key. In case of WEP128, 26 hexadecimal (base 16) characters (0-9 and A-F) ) */
#endif

#elif defined(STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)
#define MAIN_WLAN_802_1X_USR_NAME       "atmeluser" /**< RADIUS user account name */
#define MAIN_WLAN_802_1X_PWD            "12345678" /**< RADIUS user account password */
#endif
void mode_sta(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* STA_H_INCLUDED */
