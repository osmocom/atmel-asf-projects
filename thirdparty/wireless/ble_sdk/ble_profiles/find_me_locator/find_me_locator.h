 /**
  * \file
  *
  * \brief Find Me Locator Profile declarations
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
 
 // <<< Use Configuration Wizard in Context Menu >>>
 // <h> Find Me Target Profile Configuration
 // =======================
 
 #ifndef __FIND_ME_TARGET_H__
 #define __FIND_ME_TARGET_H__
 
 
 /****************************************************************************************
 *							        Includes	
 *                                                                                       *
 ****************************************************************************************/
 
 
 /****************************************************************************************
 *							        Macros	
 *                                                                                       *
 ****************************************************************************************/
#define DISCOVER_SUCCESS				(10)

//   <o> Low Alert Range Value
//   <i> Defines inteval at which Low Alert get triggers.
//   <i> Default: -70
#define FMP_LOW_ALERT_RANGE             (-70)

//   <o> High Alert Range Value
//   <i> Defines inteval at which High Alert get triggers.
//   <i> Default: -90
#define FMP_HIGH_ALERT_RANGE            (-90)

typedef enum fmp_locator_alert_level {
	FMP_NO_ALERT = 0,
	FMP_MID_ALERT,
	FMP_HIGH_ALERT
} fmp_locator_alert_level_t;
 
 /****************************************************************************************
 *							        Function
 *									Prototypes	                                        *
 ****************************************************************************************/
 
 /** @brief Initialize the profile, includes all initializations and start up
  * routines of the profiles
  *
  *
  * @param[in] void
  *
  * @pre Must be called after @ref ble_init
  *
  * @return void
  */
void fmp_locator_init(void);
 
 /** @brief connection state handler
  * called when device goes on connected state
  * @param[in] at_ble_connected_t which includes the connection parameters
  *
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
 /**
  * \Find me profile connected state handler function
  */
 at_ble_status_t fmp_locator_connected_state_handler(void * params);

 
 /** @brief disconnection event handler
  *
  * @param[in] at_ble_disconencted_t which includes the connection parameters
  *like handle,reason for disconnection
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
 at_ble_status_t fmp_locator_disconnect_event_handler(void * params);
 
 at_ble_status_t fmp_locator_pair_done_handler(void *params);
 
 /**
 * @brief Service found handler invoked by ble manager
 */
at_ble_status_t fmp_locator_service_found_handler(void *param);

/**@brief Handles all Discovered characteristics of a given handler in a
* connected device
*
* @param[in] characteristic_found Discovered characteristics params of a
*connected device
*
*/
at_ble_status_t fmp_locator_characteristic_found_handler(void *params);

/**@brief Discover all Characteristics supported for Immediate alert Service of a
* connected device
*
* @param[in] discover_status discovery status of each handle
*
*/
at_ble_status_t fmp_locator_discovery_complete_handler(void *params);

 /**@brief Discover all services
 *
 * @param[in] connection handle.
 * @return @ref AT_BLE_SUCCESS operation programmed successfully.
 * @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
 * @return @ref AT_BLE_FAILURE Generic error.
 */
at_ble_status_t fmp_locator_service_discover(at_ble_handle_t handle);
 
 #endif /* __FIND_ME_TARGET_H__ */
 // </h>
 
 // <<< end of configuration section >>>
