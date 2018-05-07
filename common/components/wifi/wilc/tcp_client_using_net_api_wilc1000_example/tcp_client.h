/**
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
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
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
#ifndef APPS_H_INCLUDED
#define APPS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
	#endif

/* Wi-Fi Settings */

/* for WPA Security */
#define MAIN_WLAN_SSID		            "DEMO_AP" /**< Destination SSID */
#define MAIN_WLAN_AUTH					M2M_WIFI_SEC_WPA_PSK /* < Security manner */
#define MAIN_WLAN_PSK                   "12345678" /**< Password for Destination SSID */

/* for WEP Security */
//#define MAIN_WLAN_SSID		        "DEMO_AP" /**< Destination SSID */
//#define MAIN_WLAN_AUTH				M2M_WIFI_SEC_WEP /* < Security manner */
#define MAIN_WLAN_WEP_KEY_INDEX         1 /**< WEP key index */
#define MAIN_WLAN_WEP_KEY_40            "1234567890" /**< 64 bit WEP key. In case of WEP64, 10 hexadecimal (base 16) characters (0-9 and A-F) ) */
#define MAIN_WLAN_WEP_KEY_104           "1234567890abcdef1234567890" /**< 128 bit WEP key. In case of WEP128, 26 hexadecimal (base 16) characters (0-9 and A-F) ) */
#define MAIN_WIFI_M2M_PRODUCT_NAME        "WILC TCP CLIENT"



#define MAIN_WIFI_M2M_SERVER_IP           ((uint32_t)0xc0a80103UL) //0xFFFFFFFF /* 255.255.255.255 */
#define MAIN_WIFI_M2M_SERVER_PORT         (6666)
#define MAIN_WIFI_M2M_REPORT_INTERVAL     (1000)

#define MAIN_WIFI_M2M_BUFFER_SIZE          1460

/** Header Settings */
#define HEADER_VERSION1						0x80000000
#define RUN_NOW								0x00000001

void tcp_client_task(void *argument);


#ifdef __cplusplus
}
#endif

#endif // APPS_H_INCLUDED
