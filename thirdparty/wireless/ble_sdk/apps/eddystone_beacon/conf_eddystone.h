/**
 * \file
 *
 * \brief Eddystone beacon configuration
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

#ifndef CONF_EDDYSTONE_H_INCLUDED
#define CONF_EDDYSTONE_H_INCLUDED

/****************************************************************************************
*				        Default Configuration Defines	                                     	     *
****************************************************************************************/

/** Eddystone-UID defines */

/** @brief  Namespace ID - 10 bytes of the Eddystone-UID */
#define CONF_ES_NAMESPACE_ID						("\xB3\x75\xE9\x7F\x93\x66\xE7\xC2\xA6\xE1")

/** @brief  Instance ID - 6 bytes of the Eddystone-UID */
#define CONF_ES_INSTANCE_ID							("\x00\x00\x00\x00\x00\x01")

/** @brief  TX power level at 0 meters [-100 dBm to +20 dBm] - used for beacon ranging */
#define CONF_ES_CALIB_TX_PWR_0m						(-4)

/** @brief  Radio TX power setting for the device to achieve @ref CONF_ES_CALIB_TX_PWR_0m level */
#define CONF_ES_RADIO_TX_PWR_0m						(AT_BLE_TX_PWR_LVL_ZERO_DB)


/** Eddystone-URL defines */

/** @brief  Default URL value in encoded form 
 *  \x02goo.gl/SJmAjy = Shortened URL for "http://www.atmel.com/products/Wireless/bluetooth/default.aspx"
 */
#define CONF_ES_DEFAULT_URL							("\x02goo.gl/SJmAjy")

/** @brief  Number of bytes in the default URL */
#define CONF_ES_DEFAULT_URL_SIZE					(14)

/** @brief  Beacon interval in milliseconds [100ms to 10240ms (10.24s)] */
#define CONF_ES_DEFAULT_BEACON_PERIOD_MS			(200)

/** @brief  Advertised TX Power levels at 0 meters in dBm [-100 dBm to +20 dBm] */
#define CONF_ES_DEFAULT_ADV_TX_PWR_HIGH				(-1)
#define CONF_ES_DEFAULT_ADV_TX_PWR_MEDIUM			(-4)
#define CONF_ES_DEFAULT_ADV_TX_PWR_LOW				(-10)
#define CONF_ES_DEFAULT_ADV_TX_PWR_LOWEST			(-18)

/** @brief  Radio TX Power levels @ref at_ble_tx_power_level_t to achieve advertised TX power */
#define CONF_ES_DEFAULT_RADIO_TX_PWR_HIGH			(AT_BLE_TX_PWR_LVL_POS_03_DB)
#define CONF_ES_DEFAULT_RADIO_TX_PWR_MEDIUM			(AT_BLE_TX_PWR_LVL_ZERO_DB)
#define CONF_ES_DEFAULT_RADIO_TX_PWR_LOW			(AT_BLE_TX_PWR_LVL_NEG_06_DB)
#define CONF_ES_DEFAULT_RADIO_TX_PWR_LOWEST			(AT_BLE_TX_PWR_LVL_NEG_14_DB)

/** @brief  Enable/disable the lock feature.  
 * This controls the inclusion of the optional 'lock' and 'unlock' characteristics in the Eddystone-URL configuration service 
 *  0 - Disable lock feature;  1 - Enable lock feature.
 */
#define ES_URL_CONF_ENABLE_LOCK						(1)

#if (ES_URL_CONF_ENABLE_LOCK == 1)
	/** brief  Default lock state (1 = LOCKED;  0 = UNLOCKED) when lock feature is enabled */
	#define CONF_ES_DEFAULT_LOCK_STATE					(0)
#else
	/** brief  Default lock state when lock feature is disabled; Must be set to UNLOCKED always. */ 
	#define CONF_ES_DEFAULT_LOCK_STATE					(0)
#endif

/** @brief  Default lock code of 16 bytes */
#define CONF_ES_DEFAULT_LOCK_CODE					"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

/** @brief  Number of bytes in the lock code */
#define CONF_ES_DEFAULT_LOCK_CODE_SIZE				(16)

/** @brief  Local name advertised in the Eddystone-URL configuration  mode */
#define CONF_ES_CONF_ADV_DATA_COMPLETE_LOCAL_NAME	"Atmel-Eddystone"

#endif /* CONF_EDDYSTONE_H_INCLUDED */