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

//Include files here 
#include "asf.h"
#include "FreeRTOS.h"
#include "osprintf.h"
#include "lwip/ip.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "os/include/net_init.h"
#include "os/include/m2m_wifi_ex.h"
#include "tinyservices.h"
#include "conf_wilc.h"
#include "driver/include/m2m_wifi.h"
#include "common/include/nm_common.h"
#include "lwip/sockets.h"
#include <string.h>
#include <stdio.h>
#include <udp_client.h>

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */

/** Message format definitions. */
typedef struct s_msg_wifi_product {
	uint8_t name[9];
} t_msg_wifi_product;

typedef struct s_msg_wifi_product_main {
	uint8_t name[9];
} t_msg_wifi_product_main;

/** Message format declarations. */
static t_msg_wifi_product msg_wifi_product = {
	.name = MAIN_WIFI_M2M_PRODUCT_NAME1,					// modified by SP123
};

static t_msg_wifi_product_main msg_wifi_product_main = {
	.name = MAIN_WIFI_M2M_PRODUCT_NAME2,					// modified by SP123
};

/////////////////////////////////

/** Security parameters for 64 bit WEP Encryption @ref m2m_wifi_connect */
tstrM2mWifiWepParams wep64_parameters = { MAIN_WLAN_WEP_KEY_INDEX, sizeof(MAIN_WLAN_WEP_KEY_40), MAIN_WLAN_WEP_KEY_40};
/** Security parameters for 128 bit WEP Encryption @ref m2m_wifi_connect */
tstrM2mWifiWepParams wep128_parameters = { MAIN_WLAN_WEP_KEY_INDEX, sizeof(MAIN_WLAN_WEP_KEY_104), MAIN_WLAN_WEP_KEY_104};


/////////////////////////////////
// no need to initialize socket 

////////////////////////////////

/** IP address of host. */
uint32_t gu32HostIp = 0;

/** Wi-Fi status variable. */
static volatile bool gbConnectedWifi = false;

/** Get host IP status variable. */
static volatile bool gbHostIpByName = false;

/** Socket for UDP */
static SOCKET udp_socket = -1;

#define BUFSIZE                         10

/** UDP MAX packet count */
#define MAIN_WIFI_M2M_PACKET_COUNT         10

/** Number of STA connected. */
uint32_t sta_connected = 0;

/** UDP packet count */
static uint8_t packetCnt = 0;

//////////////////////////////////////////////////////////////////

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType Type of Wi-Fi notification.
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters.
 *
 * \return None.
 */
static void wifi_cb(uint8 msg_type, void *msg)
{
	switch (msg_type) {
		case M2M_WIFI_RESP_CON_STATE_CHANGED : {
			tstrM2mWifiStateChanged *ctx = (tstrM2mWifiStateChanged*)msg;
			if (ctx->u8IfcId == STATION_INTERFACE) {
				if (ctx->u8CurrState == M2M_WIFI_CONNECTED) {
					osprintf("wifi_cb: M2M_WIFI_CONNECTED\n\r");
					net_interface_up(NET_IF_STA);				// need to check
					m2m_wifi_request_dhcp_client_ex();
				}
				if(ctx->u8CurrState == M2M_WIFI_DISCONNECTED) {
					osprintf("wifi_cb: M2M_WIFI_DISCONNECTED\n\r");
					osprintf("wifi_cb: reconnecting...\n\r");
					net_interface_down(NET_IF_STA);				// need to check
					//m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID),
					//		MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
				}
			}
		}
		break;

		case NET_IF_REQ_DHCP_CONF : {
			tstrM2MIPConfig2 *strIpConfig = msg;
			uint16_t *a = (void *)strIpConfig->u8StaticIPv6;
			osprintf("wifi_cb: STA M2M_WIFI_REQ_DHCP_CONF\n\r");
			osprintf("wifi_cb: STA IPv4 addr: %d.%d.%d.%d\n\r", strIpConfig->u8StaticIP[0], strIpConfig->u8StaticIP[1],
			strIpConfig->u8StaticIP[2], strIpConfig->u8StaticIP[3]);
			osprintf("wifi_cb: STA IPv6 addr: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
			htons(a[0]), htons(a[1]), htons(a[2]), htons(a[3]),
			htons(a[4]), htons(a[5]), htons(a[6]), htons(a[7]));
			gbConnectedWifi = true;
		}
		break;

		default:
		break;
	}
}

/**
 * \brief UDP client task function.
 */
void udp_client_task(void *v)
{
		//ip_addr_t remote_ip;
		int8_t ret;
		struct sockaddr_in addr;

		/* Initialize the network stack. */
		net_init();

		/* Initialize the WILC driver. */ /* Initialize Wi-Fi parameters structure. */
		tstrWifiInitParam param;
		memset(&param, 0, sizeof(param));

		/* Initialize Wi-Fi driver with data and status callbacks. */
		param.pfAppWifiCb = wifi_cb;
		ret = os_m2m_wifi_init(&param);

		/* Connect to defined AP. */
		/* Case 1. Connect to AP with security type WPA. */
		os_m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);

		/* Case 2. Connect to AP with security type WEP. */
		//os_m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, &wep64_parameters, M2M_WIFI_CH_ALL); // use &wep64_parameters or &wep128_parameters

		while(!gbConnectedWifi){
			vTaskDelay(200);
		}

		/* Initialize socket address structure. */
		addr.sin_family = AF_INET;
		addr.sin_port = htons(MAIN_WIFI_M2M_SERVER_PORT);
		addr.sin_addr.s_addr = htonl(MAIN_WIFI_M2M_SERVER_IP);

		/* Create and connect client socket. */
		udp_socket= socket(AF_INET, SOCK_DGRAM /*for UDP*/, 0/*flag*/);  //UDP socket creation

		/* To check remote ip. */
		//remote_ip.addr= PP_HTONL(MAIN_WIFI_M2M_SERVER_IP);
		//osprintf("display server addr:%x\n\n",(remote_ip.addr));

	
	while (1) {
		if (packetCnt == MAIN_WIFI_M2M_PACKET_COUNT) {
			close(udp_socket);
			udp_socket = -1;
			continue;
		}

		if (gbConnectedWifi) {
			/* Create socket for Client UDP */
				if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
					osprintf("main : failed to create UDP client socket error!\n\r");
					continue;
				}
			
			/* Send client frame. */
			//sendto(udp_socket, &msg_wifi_product, sizeof(t_msg_wifi_product), 0, (struct sockaddr *)&addr, sizeof(addr));
			ret = sendto(udp_socket, &msg_wifi_product_main, sizeof(t_msg_wifi_product_main), 0, (struct sockaddr *)&addr, sizeof(addr));
			vTaskDelay(20);
			
				// 	Alternatively check for message success macro
			if ( ret >= 0 ) {
					packetCnt += 1;
					osprintf("main: packets sent :%d\r\n",(packetCnt));
				if (packetCnt == MAIN_WIFI_M2M_PACKET_COUNT) {
					osprintf("UDP Client Test Complete!\n\r");
					while(1);									
					} 
				} else {
					osprintf("main: failed to send status report - error!\n\r");
				
			}
		}
	}

	
}

#endif /* LWIP_SOCKET  don't build if not configured for use in lwipopts.h */