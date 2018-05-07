/**
 * \file
 *
 * \brief AltBeacon Application Declarations
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
#ifndef __ALT_BEACON_APP_H__
#define __ALT_BEACON_APP_H__

/****************************************************************************************
*                     Includes                                        *
****************************************************************************************/
#include "at_ble_api.h"
#include <asf.h>
#include <string.h>
#include <conf_console_serial.h>
#include "uart.h"
#include "stddef.h"     // standard definition
#include "stdarg.h"
#include "stdio.h"
#include "platform.h"
#include "spi_flash.h"


/****************************************************************************************
*							        Macros	                                     		*
****************************************************************************************/

/* Beacon Advertisement timeout */
#define ALT_BEACON_ADV_TIMEOUT	(0) 

/* Beacon Advertise at exact interval */
#define ALT_BEACON_ABSOLUTE_INTERVAL_ADV (1)

/* Beacon Advertisement data */
#define MANUFACTURER_SPECIFIC_TYPE					(0xff)
#define MANUFACTURER_ID                             ("\x13\x00")
#define MANUFACTURER_ID_LENGTH						(0x02)
#define ALT_BEACON_CODE								("\xbe\xac")
#define ALT_BEACON_CODE_LENGTH						(0x02)
#define ALT_BEACON_16BYTE_UUID						("\x00\x00\x00\x00\x00\x00\x10\x00\x80\x00\x00\x80\x5f\x9b\x34\xfb")
#define ALT_BEACON_16BYTE_UUID_LENGTH				(0x10)
#define ALT_BEACON_MAJOR_NUM						("\x00\x01")
#define ALT_BEACON_MINOR_NUM						("\x00\x01")
#define ALT_BEACON_2BYTE_LEN						(0x02)
#define REF_RSSI									(0xd3)
#define MFG_RSVD								    (0x00)
#define ALT_BEACON_ADV_LENGTH						(0x1b)
#define ALT_BEACON_ADV_TYPE_LENGTH					(0x01)
#define ALT_BEACON_SCAN_RESPONSE_LENGTH				(0x00)

/**
 * @brief Initialize the ble module
 */				
static void ble_init(void);

/**
 * @brief Initialize the AltBeacon advertisement data and start advertisement.
 */	
static void altbeacon_init(void);

/**
 * @brief Set BLE address 
 * @param[in] at_ble_addr_t If address is NULL then it will use BD public address.
 */
static void ble_set_address(at_ble_addr_t *addr);				

#endif /* __ALT_BEACON_APP_H__ */
