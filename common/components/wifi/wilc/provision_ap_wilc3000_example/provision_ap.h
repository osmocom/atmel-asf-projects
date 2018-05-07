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

/** Wi-Fi Settings */

/** AP mode Settings */
#define MAIN_WLAN_SSID                 "WILC3000_AP" /* < SSID */
#define MAIN_WLAN_AUTH                 M2M_WIFI_SEC_OPEN /* < Security manner */
#define MAIN_WLAN_CHANNEL              M2M_WIFI_CH_6 // (6) /* < Channel number */
#define MAIN_WLAN_PSK				   "12345678" /* Replace Assigned < Password for Destination SSID > */

#define MAIN_WIFI_M2M_PRODUCT_NAME        "PROVISION AP TEST"

#define MAIN_WIFI_M2M_SERVER_IP			0xFFFFFFFF //((uint32_t)0xc0a80102UL) /* 255.255.255.255 */
#define MAIN_WIFI_M2M_SERVER_PORT		80

/** Using IP address. */
#define IPV4_BYTE(val, index)          ((val >> (index * 8)) & 0xFF)

/** Receive buffer size. */
#define MAIN_WIFI_M2M_BUFFER_SIZE      1024


typedef sint8			SOCKET;


void provision_ap_task(void *argument);


/** Header Settings */
#define HEADER_VERSION1					0x80000000
#define RUN_NOW							0x00000001



#ifdef __cplusplus
}
#endif

#endif // APPS_H_INCLUDED
