/**
 * \file
 *
 * \brief Bas MultiPeripheral/ Find Me Central declarations
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

/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 * Support</a>
 */

#ifndef __MUTLIROLE_MULTICONNECT_H__
#define __MUTLIROLE_MULTICONNECT_H__



typedef enum bas_notification_state{
	BAS_NOTIFICATION_DISABLED,
	BAS_NOTIFICATION_ENABLED,
	BAS_NOTIFICATION_SENT,
	BAS_NOTIFICATION_CONFIRMED
}bas_notification_state_t;

typedef enum fmp_scan_state{
	FMP_SCANNING,
	FMP_IDLE,
	FMP_CONNECTED,
}fmp_scan_state_t;

typedef struct bas_app_state{
	at_ble_handle_t conn_handle;
	bas_notification_state_t notification_state;
	bool bas_level_notified;
}bas_app_state_t;

typedef struct fmp_app_state {
	at_ble_addr_t fmp_peer_address;
	at_ble_handle_t fmp_conn_handle;
}fmp_app_state_t;

#define FMP_ASCII_TO_DECIMAL_VALUE      ('0')
#define AD_TYPE_COMPLETE_LIST_UUID		0x03
#define BATTERY_UPDATE_INTERVAL	(1) //1 second
#define BATTERY_MAX_LEVEL		(100)
#define BATTERY_MIN_LEVEL		(0)
/** @brief APP_BAS_FAST_ADV between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). */
#define APP_BAS_FAST_ADV				(1600) //1600 ms

/** @brief APP_BAS_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x028F in seconds, 0x0000 disables time-out.*/
#define APP_BAS_ADV_TIMEOUT				(60) // 60 Secs

#define BAS_MAX_DEVICE_CONNECTION (7)

/**@brief Connect to a peer device
*
*/
at_ble_status_t app_connect_request(at_ble_scan_info_t *scan_buffer,
uint8_t index);


/**@brief Find me sending the alert level to peer devices after checking the rssi 
*
*/
static void fmp_simulation_task(void);
 
#endif /*__MUTLIROLE_MULTICONNECT_H__*/
