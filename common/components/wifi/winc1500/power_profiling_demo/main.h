/**
 * \file
 *
 * \brief MAIN configuration.
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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/** Wi-Fi Settings */
#define TEST_WLAN_SSID                  "Demo_AP"  /*Destination SSID */
#define TEST_WLAN_AUTH                  M2M_WIFI_SEC_WPA_PSK  /*Security Type*/
#define TEST_WLAN_PSK                   "password"  /* Password for SSID */
/*Server or Client IP settings*/
#define TEST_WINC_RECEIVE_BUFFER          1460    /*Receive Buffer for WINC1500*/
#define TEST_WINC_SERVER_PORT			  6666    /*WINC1500 Listen Port or destination port if UDP is used*/
#define TEST_WINC_SERVER_IP			      "255.255.255.255" /*WINC1500 Listen IP address*/
#define TEST_SSL_TCP_IP					  "192.168.1.122"   /*SSL or TCP IP address of the server*/
#define TEST_SSL_SERVER_PORT			  443				/*SSL server port number*/
#define TEST_TCP_SERVER_PORT			  6666              /*TCP server port number*/
/*Default Profile Settings*/
#define TX_PACKET_SIZE					  64				/*Packet size used in profiling*/
#define TX_DELAY_MS						  100				/*Delay between packets in TX profiling*/
#define DEFAULT_TX_POWER				  TX_PWR_HIGH		/*Default TX power mode in the test*/
#define DEFAULT_RX_POWER			      PWR_HIGH			/*Default RX sensitivity mode in the test*/
#define DEFAULT_DTIM_FLAG                 1
#define TEST_SSL_UPLOAD_SIZE			  1	                /*Number of time "TX_PACKET_SIZE" bytes are sent out*/

//Only one can be enabled at a time, probe on PB30 on SAMD21 Xplained Pro
#define AP_CONNECTION_TOGGLE              1 
#define TCP_CONNECTION_TOGGLE             0
#define TCP_UPLOAD_TOGGLE				  0

typedef struct{
	uint8_t			   dtim;
	uint16_t		   listen_interval;
	tenuPowerSaveModes ps_mode;
	tenuM2mTxPwrLevel  tx_pwr_mode;
	tenuM2mPwrMode	   rx_pwr_mode;
}low_power_test_param;

typedef enum{
	IDLE_STATE = 0,
	WIFI_CONNECTION_STATE,
	SOCKET_CONNECTION_STATE
}app_states;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */
