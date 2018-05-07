/**
 * \file
 *
 * \brief Custom Serial Chat Application declarations
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
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel micro controller product.
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

/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 *Support</a>
 */

/**
 * \mainpage
 * \section preface Preface
 * This example demonstrates on how to implement Custom profile on WINC3400 with the SAM Xplained Pro
 * Wi-Fi/BLE module. <br>
 * It uses the following hardware:
 * - SAM Xplained Pro.
 * - the WIN3400 module on EXT1.
 * - thirdparty Android device.
 *
 * \section files Main Files
 * - csc_app.c : Initialize the WINC3400 and runs custom serial chat profile
 *
 * \section usage Usage
 * -# As a first step, it is required that the user install the Atmel Smart Connect application
 * -#  available in google play store on an Android device.
 * -# Then, power up the SAM board programmed with customer serial chat application and run the 
 * -# Android application: select Bluetooth Smart and select start scan. In the Scan result, select AT-CSC.
 * -# The android appliction will ask to enter the pin to pair with WINC3400. Enter the PIN 123456.
 * -# Select Custome Serial Chat in service list.
 * \endcode
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application as the follows.
 * \code
 *    Baud Rate : 115200
 *    Data : 8bit
 *    Parity bit : none
 *    Stop bit : 1bit
 *    Flow control : none
 * \endcode
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 * \code
 * -- Custom Serial chat demo --
 * -- SAMD21_XPLAINED_PRO --
 * -- Compiled: May  2 2017 14:50:52 --
 * 
 * (APP)(INFO)Chip ID 3400d1
 * (APP)(INFO)Curr driver ver: 1.0.6
 * (APP)(INFO)Curr driver HIF Level: (2) 1.1
 * (APP)(INFO)Fw HIF: 8101
 * (APP)(INFO)Firmware HIF (2) : 1.1
 * (APP)(INFO)Firmware ver   : 1.1.4
 * (APP)(INFO)Firmware Build Apr 11 2017 Time 13:32:08
 * (APP)(INFO)Ota HIF: 0000
 * (APP)(INFO)No valid Ota image
 * (APP)(INFO)OTP MAC
 * (APP)(INFO)MAC Address: F8:F0:05:F2:52:30
 * (APP)(INFO)M2M_NO_PS
 * (APP)(INFO)POWER SAVE 0
 * 
 * Initializing Custom Serial Chat Application
 * BLE is initializing
 * 
 * Device Name: ATMEL-CSC
 * 
 * Device Started Advertisement
 * Connected to peer device with address 4f:01:d5:b7:b8:33
 * 
 * Remote device request pairing
 * Sending pairing response
 * please enter the following pass-code on the other device:123456
 * Pairing procedure completed successfully
 * Rx: Message from android
 * TX:Message from WINC3400
 * \endcode
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 6.2
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 */

/*- Includes -----------------------------------------------------------------------*/
#include <asf.h>
#include "at_ble_api.h"
#include "ble_manager.h"
#include "csc_app.h"
#include "cscp.h"
#include "cscs.h"
#include "conf_extint.h"
#include "sio2host.h"
#include "bsp/include/nm_bsp.h"
#include "m2m_ble.h"
#include "driver/include/m2m_periph.h"

#define STRING_HEADER "-- Custom Serial chat demo --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --\r\n"

/* =========================== GLOBALS ============================================================ */

/* Received notification data structure */
csc_report_ntf_t recv_ntf_info;

static at_ble_event_parameter_t gu8BleParam __aligned(4);

/* Data length to be send over the air */
uint16_t send_length = 0;

/* Buffer data to be send over the air */
uint8_t send_data[APP_TX_BUF_SIZE];

static void app_wifi_handle_event(uint8 u8MsgType, void * pvMsg)
{
	return;
}

static void app_wifi_init(tpfAppWifiCb wifi_cb_func)
{
	tstrWifiInitParam param;
	sint8 ret;
	uint8 mac_addr[6];
	uint8 u8IsMacAddrValid;
	sint8 deviceName[] = M2M_DEVICE_NAME;
	uint32 pinmask = (
			M2M_PERIPH_PULLUP_DIS_HOST_WAKEUP|
			M2M_PERIPH_PULLUP_DIS_SD_CMD_SPI_SCK|
			M2M_PERIPH_PULLUP_DIS_SD_DAT0_SPI_TXD);


#ifdef _STATIC_PS_
	nm_bsp_register_wake_isr(wake_cb, PS_SLEEP_TIME_MS);
#endif
	
	m2m_memset((uint8*)&param, 0, sizeof(param));
	param.pfAppWifiCb = wifi_cb_func;

	ret = m2m_ble_wifi_init(&param);
	if (M2M_SUCCESS != ret){
		M2M_ERR("Driver Init Failed <%d>\n",ret);
		M2M_ERR("Reseting\n");
		// Catastrophe - problem with booting. Nothing but to try and reset
		system_reset();
		while (1){
		}
	}

	m2m_periph_pullup_ctrl(pinmask, 0);

	m2m_wifi_get_otp_mac_address(mac_addr, &u8IsMacAddrValid);
	if (!u8IsMacAddrValid) {
		uint8 DEFAULT_MAC[] = MAC_ADDRESS;
		M2M_INFO("Default MAC\n");
		m2m_wifi_set_mac_address(DEFAULT_MAC);
	} else {
		M2M_INFO("OTP MAC\n");
	}
	m2m_wifi_get_mac_address(mac_addr);
	M2M_INFO("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
	         mac_addr[0],mac_addr[1],mac_addr[2],
	         mac_addr[3],mac_addr[4],mac_addr[5]);

	/* Name must be in the format WINC3400_00:00 */
	{
		#define HEX2ASCII(x) (((x)>=10)? (((x)-10)+'A') : ((x)+'0'))

		uint16 len;
		len = m2m_strlen((uint8_t *) deviceName);
		if (len >= 5) {
			deviceName[len-1] = HEX2ASCII((mac_addr[5] >> 0) & 0x0f);
			deviceName[len-2] = HEX2ASCII((mac_addr[5] >> 4) & 0x0f);
			deviceName[len-4] = HEX2ASCII((mac_addr[4] >> 0) & 0x0f);
			deviceName[len-5] = HEX2ASCII((mac_addr[4] >> 4) & 0x0f);
		}
	}
	m2m_wifi_set_device_name((uint8*)deviceName, (uint8)m2m_strlen((uint8*)deviceName));

#ifdef _DYNAMIC_PS_
	{
		tstrM2mLsnInt strM2mLsnInt;
		M2M_INFO("M2M_PS_DEEP_AUTOMATIC\r\n");
		m2m_wifi_set_sleep_mode(M2M_PS_DEEP_AUTOMATIC, 1);
		strM2mLsnInt.u16LsnInt = M2M_LISTEN_INTERVAL;
		m2m_wifi_set_lsn_int(&strM2mLsnInt);
	}
#elif (defined _STATIC_PS_)
	M2M_INFO("M2M_PS_MANUAL\r\n");
	m2m_wifi_set_sleep_mode(M2M_PS_MANUAL, 1);
#else
	M2M_INFO("M2M_NO_PS\r\n");
	m2m_wifi_set_sleep_mode(M2M_NO_PS, 1);
#endif
}

/* Function used for receive data */
static void csc_app_recv_buf(uint8_t *recv_data, uint8_t recv_len)
{
	uint16_t ind = 0;
	if (recv_len){
		DBG_LOG("Rx: ");
		for (ind = 0; ind < recv_len; ind++){
			sio2host_putchar(recv_data[ind]);
		}
		DBG_LOG("\r\n");
	}
}

/* Callback called for new data from remote device */
static void csc_prf_report_ntf_cb(csc_report_ntf_t *report_info)
{
	csc_app_recv_buf(report_info->recv_buff, report_info->recv_buff_len);
}

/* Function used for send data */
static void csc_app_send_buf(void)
{
	uint16_t ind = 0;
	uint16_t len = 0;
	uint8_t buff = 0;
	len = sio2host_rx(&buff, 1);
	if (len){
		for (ind = 0; ind < len; ind++)
		{
			if(buff != ENTER_BUTTON_PRESS)
			{
				if (!send_length)
				{
					DBG_LOG("TX:");
				}
				sio2host_putchar(buff);
				if (buff == BACKSPACE_BUTTON_PRESS)
				{
					sio2host_putchar(SPACE_BAR);
					sio2host_putchar(buff);
					if(send_length)
						send_length--;
				}
				  
				if(send_length < APP_TX_BUF_SIZE)
				{
					if(buff != BACKSPACE_BUTTON_PRESS)
						send_data[send_length++] = buff;
				}
				else
				{
					csc_prf_send_data(&send_data[0], send_length);
					send_length = 0;
				}
			}
			else
			{ // User press enter to send data
				if(send_length)
				{
					ind = send_length;
					send_length = 0;
					csc_prf_send_data(&send_data[0], ind);
					DBG_LOG("\r\n");
				}
			}
		}
	}
}

/*bool app_exec = true;*/
void ble_csc_init(void )
{
	at_ble_events_t ble_event;
	
	DBG_LOG("Initializing Custom Serial Chat Application");
	
	/* Initialize the buffer address and buffer length based on user input */
	csc_prf_buf_init(&send_data[0], APP_TX_BUF_SIZE);
	
	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);

	/* Register the notification handler */
	notify_recv_ntf_handler(csc_prf_report_ntf_cb);
	
	/* Capturing the events  */
	while(1){
		if (m2m_ble_event_get(&ble_event, &gu8BleParam) == AT_BLE_SUCCESS)
		{
			if (ble_event != AT_BLE_UNDEFINED_EVENT)
			{
				// Feed the received event into BlueSDK stack.
				ble_event_manager(ble_event, &gu8BleParam);
			}
		}		
		csc_app_send_buf();
	}
}

int main (void)
{
	system_init();
	//configure_console();
	sio2host_init();
	puts(STRING_HEADER);

	nm_bsp_init();

	// Initialize WiFi interface first.
	// 3400 WiFi HIF is used to convey BLE API primitives. 
	app_wifi_init(app_wifi_handle_event);

	// Initialize BLE stack on 3400.
	m2m_ble_init();
	ble_csc_init();
}

