/**
 * \file
 *
 * \brief Battery service
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
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/* can only have one mainpage/preface for doxygen see ble_sdk\ble_services\tx_power\tx_power.c */
/****************************************************************************************
*							        Includes	                                     	*
****************************************************************************************/

#include "battery.h"

/** Initial value of the battery level characteristic value */
uint8_t battery_init_value = 10;

/** Presentation format of the batery level */
at_ble_char_presentation_t presentation_format;

extern at_ble_connected_t ble_connected_dev_info[MAX_DEVICE_CONNECTED];

/**@brief Initialize the service with its included service, characteristics, and descriptors
 *
 * @param[in] battery_serv gatt service information
 *
 * @return none
 */
void bat_init_service(gatt_service_handler_t *battery_serv, uint8_t *battery_value)
{
	battery_serv->serv_handle = 0;
	battery_serv->serv_uuid.type = AT_BLE_UUID_16;
	battery_serv->serv_uuid.uuid[0] = (uint8_t) BAT_SERVICE_UUID;
	battery_serv->serv_uuid.uuid[1] = (uint8_t) (BAT_SERVICE_UUID >> 8);
	
	//Battery service characteristic
	battery_serv->serv_chars[0].char_val_handle = 0;          /* handle stored here */
	battery_serv->serv_chars[0].uuid.type = AT_BLE_UUID_16;
	battery_serv->serv_chars[0].uuid.uuid[0] = (uint8_t) BAT_CHAR_BAT_LEVEL_UUID;          /* UUID : Manufacturer Name String */
	battery_serv->serv_chars[0].uuid.uuid[1] = (uint8_t) (BAT_CHAR_BAT_LEVEL_UUID >> 8);          /* UUID : Manufacturer Name String */
	battery_serv->serv_chars[0].properties = (AT_BLE_CHAR_READ | AT_BLE_CHAR_NOTIFY); /* Properties */
	battery_serv->serv_chars[0].init_value = &battery_init_value;             /* value */
	battery_serv->serv_chars[0].value_init_len = sizeof(uint8_t);
	battery_serv->serv_chars[0].value_max_len = sizeof(uint8_t);
	battery_serv->serv_chars[0].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);   /* permissions */
	battery_serv->serv_chars[0].user_desc = NULL;           /* user defined name */
	battery_serv->serv_chars[0].user_desc_len = 0;
	battery_serv->serv_chars[0].user_desc_max_len = 0;
	battery_serv->serv_chars[0].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	battery_serv->serv_chars[0].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	battery_serv->serv_chars[0].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	battery_serv->serv_chars[0].user_desc_handle = 0;             /*user desc handles*/
	battery_serv->serv_chars[0].client_config_handle = 0;         /*client config handles*/
	battery_serv->serv_chars[0].server_config_handle = 0;         /*server config handles*/
	
	presentation_format.format = BAT_CHAR_PRESENTATION_FORMAT_VALUE;
	presentation_format.exponent = BAT_CHAR_PRESENTATION_FORMAT_EXPONENT;
	presentation_format.unit =  BAT_CHAR_PRESENTATION_FORMAT_UNIT;
	presentation_format.name_space = BAT_CHAR_PRESENTATION_FORMAT_NAMESPACE;
	presentation_format.description = BAT_CHAR_PRESENTATION_FORMAT_DESCRIPTOR;
	
	battery_serv->serv_chars[0].presentation_format = &presentation_format;       /* presentation format */
}

/**@brief defining a initialized service 
 *
 * @param[in] battery_service gatt service information
 *
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t bat_primary_service_define(gatt_service_handler_t *battery_service)
{
	return(at_ble_primary_service_define(&battery_service->serv_uuid,
	&battery_service->serv_handle,
	NULL, 0,
	battery_service->serv_chars, 1));
}

/**@brief Function used to update characteristic value
 *
 * @param[in] battery_serv gatt service information
 * @param[in] char_len length of the new characteristic value
 * @param[in] char_data new characteristic value information
 * @return @ref AT_BLE_SUCCESS operation completed successfully
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t bat_update_char_value (gatt_service_handler_t *battery_serv , uint8_t char_data)
{
	/* Updating the att data base */
	if ((at_ble_characteristic_value_set(battery_serv->serv_chars[0].char_val_handle, &char_data, 0 , sizeof(uint8_t))) == AT_BLE_FAILURE){
		DBG_LOG("updating the characteristic failed");
		return AT_BLE_FAILURE;
	} else {
		DBG_LOG_DEV("updating the characteristic value is successful");
	}

	/* sending notification to the peer about change in the battery level */ 
	if((at_ble_notification_send(ble_connected_dev_info[0].handle, battery_serv->serv_chars[0].char_val_handle)) == AT_BLE_FAILURE) {
		DBG_LOG("sending notification to the peer failed");
		return AT_BLE_FAILURE;
	}
	else {
		DBG_LOG_DEV("sending notification to the peer successful");
		return AT_BLE_SUCCESS;
	}
}
