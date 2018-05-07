/**
 *
 * \file
 *
 * \brief start STA mode.
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

#include "asf.h"
#include "sta.h"
#include "FreeRTOS.h"
#include "osprintf.h"
#include "os/include/net_init.h"
#include "os/include/m2m_wifi_ex.h"
#include "lwip/def.h"
#include <string.h>
#include <stdio.h>

/* preprocessor define error check */
#if defined( STA_MODE_OPEN_SECURITY )
	#if defined(STA_MODE_WEP_SECURITY) || defined(STA_MODE_WPA_WPA2_PERSONAL_SECURITY) || defined(STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)
	#error "Error : Multiple Security types defined along with STA_MODE_OPEN_SECURITY"
	#endif
#elif defined(STA_MODE_WEP_SECURITY)
	#if defined(STA_MODE_WPA_WPA2_PERSONAL_SECURITY) || defined(STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)
	#error "Error : Multiple Security types defined along with STA_MODE_WEP_SECURITY"
	#endif
	#if (defined( WEP_SEC_TYPE_KEY_40 ) && defined ( WEP_SEC_TYPE_KEY_104 )) || !( defined( WEP_SEC_TYPE_KEY_40 ) || defined ( WEP_SEC_TYPE_KEY_104 ) )
	#error "Error : WEP Security - Define either WEP_SEC_TYPE_KEY_40 or WEP_SEC_TYPE_KEY_104"
	#endif
#elif defined(STA_MODE_WPA_WPA2_PERSONAL_SECURITY) 
	#if defined(STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)
	#error "Error : Multiple Security types defined(STA_MODE_WPA_WPA2_PERSONAL_SECURITY and STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY) "
	#endif
#elif !defined(STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)
	#error "Error : Define Security type in sta.h (STA_MODE_OPEN_SECURITY or STA_MODE_WPA_WPA2_PERSONAL_SECURITY or STA_MODE_WEP_SECURITY or STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY)"
#endif

#if defined(P2P_MODE_CONCURRENCY)
/**
 * \brief P2P mode
 *
 * Initializes the P2P mode.
 */
static int8_t enable_disable_p2p_mode(void)
{
	int8_t ret;

	osprintf("P2P mode, start\r\n");

	/* Set device name. */
	ret = os_m2m_wifi_set_device_name((uint8_t *)MAIN_WLAN_DEVICE_NAME, strlen(MAIN_WLAN_DEVICE_NAME));
	if (M2M_SUCCESS != ret) {
		osprintf("P2P mode, set device name failed\r\n");
		return ret;
	}
	
	os_m2m_wifi_set_p2p_control_ifc(P2P_STA_CONCURRENCY_INTERFACE); // Required if STA-P2P concurrent mode
	
	/* Start P2P with channel number. */
	ret = os_m2m_wifi_p2p(MAIN_WLAN_P2P_CHANNEL);
	if (M2M_SUCCESS != ret) {
		osprintf("start P2P failed\r\n");
		return ret;
	}
	return ret;
}
#endif

/* Connect to defined AP */
void wifi_connect(){
	#if defined( STA_MODE_OPEN_SECURITY )
	osprintf("Security : OPEN\r\n");
	os_m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), M2M_WIFI_SEC_OPEN, NULL, M2M_WIFI_CH_ALL);

	#elif defined( STA_MODE_WPA_WPA2_PERSONAL_SECURITY )
	osprintf("Security : WPA/WPA2 Personal\r\n");
	os_m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), M2M_WIFI_SEC_WPA_PSK, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);

	#elif defined( STA_MODE_WEP_SECURITY )
	#if defined ( WEP_SEC_TYPE_KEY_40 )
	osprintf("Security : WEP with 10 digit WEP key\r\n");
	#elif defined ( WEP_SEC_TYPE_KEY_104 )
	osprintf("Security : WEP with 26 digit WEP key\r\n");
	#endif
	/** Security parameters for 64 bit/128 bit WEP Encryption @ref m2m_wifi_connect */
	tstrM2mWifiWepParams wep_parameters = { MAIN_WLAN_WEP_KEY_INDEX, sizeof(MAIN_WLAN_WEP_KEY), MAIN_WLAN_WEP_KEY, MAIN_WLAN_WEP_AUTH_TYPE};
	m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), M2M_WIFI_SEC_WEP,&wep_parameters, M2M_WIFI_CH_ALL);

	#elif defined( STA_MODE_WPA_WPA2_ENTERPRISE_SECURITY )
	/** credential parameter for 802.1x */
	static CONST tstr1xAuthCredentials gstrCred1x = { MAIN_WLAN_802_1X_USR_NAME, MAIN_WLAN_802_1X_PWD};
	/* Connect to the enterprise network. */
	m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), M2M_WIFI_SEC_802_1X, (char *)&gstrCred1x, M2M_WIFI_CH_ALL);
	#endif
}

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType type of Wi-Fi notification. Possible types are:
 *  - [M2M_WIFI_RESP_CON_STATE_CHANGED](@ref M2M_WIFI_RESP_CON_STATE_CHANGED)
 *  - [M2M_WIFI_REQ_DHCP_CONF](@ref M2M_WIFI_REQ_DHCP_CONF)
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_cb(uint8_t u8MsgType, void *pvMsg)
{

	switch (u8MsgType) {
		case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
			if (pstrWifiState->u8IfcId == STATION_INTERFACE) {
				if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
					osprintf("Wi-Fi connected\r\n");
					net_interface_up(NET_IF_STA);
					m2m_wifi_request_dhcp_client_ex();
				} else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
					osprintf("Wi-Fi disconnected\r\n");
					osprintf("wifi_cb: reconnecting...\r\n");
					net_interface_down(NET_IF_STA);
					/* Connect to defined AP. */
					wifi_connect();
				}
			}
#if defined(P2P_MODE_CONCURRENCY)			
			else if (pstrWifiState->u8IfcId == P2P_INTERFACE) {
				if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
					osprintf("wifi_cb: P2P: M2M_WIFI_CONNECTED\r\n");
				}
				if(pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
					osprintf("wifi_cb: P2P: M2M_WIFI_DISCONNECTED\r\n");
				}
			}
#endif			
			break;
		}

		case NET_IF_REQ_DHCP_CONF :
		{
			tstrM2MIPConfig2 *strIpConfig = pvMsg;
			uint16_t *a = (void *)strIpConfig->u8StaticIPv6;
			osprintf("wifi_cb: STA IPv4 addr: %d.%d.%d.%d\r\n", strIpConfig->u8StaticIP[0], strIpConfig->u8StaticIP[1],
			strIpConfig->u8StaticIP[2], strIpConfig->u8StaticIP[3]);
			osprintf("wifi_cb: STA IPv6 addr: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\r\n",
			htons(a[0]), htons(a[1]), htons(a[2]), htons(a[3]),
			htons(a[4]), htons(a[5]), htons(a[6]), htons(a[7]));
			
		#if defined(P2P_MODE_CONCURRENCY)
			int ret;
			ret = enable_disable_p2p_mode();
			if (M2M_SUCCESS != ret) {
				osprintf("enable_disable_ap_mode call error!\r\n");
			}
		#endif
		}
		break;

		default:
		{
			break;
		}
	}
}

/**
 * \brief STA mode main function.
 */
void mode_sta(void *argument)
{
	/* Initialize the network stack. */
	net_init();
	
	/* Initialize the WILC driver. */
	tstrWifiInitParam param;
	memset(&param, 0, sizeof(param));
	param.pfAppWifiCb = wifi_cb;
	os_m2m_wifi_init(&param);

	osprintf("Connecting to %s\r\n", (char *)MAIN_WLAN_SSID);
	/* Connect to defined AP. */
	wifi_connect();
	while(1){
	}
}
