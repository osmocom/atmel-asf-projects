/**
 * \file
 *
 * \brief Heart Rate Sensor Profile declarations
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
 *Support</a>
 */


// <<< Use Configuration Wizard in Context Menu >>>
// <h> Heart Rate Sensor Configuration
// =======================
#ifndef __HR_SENSOR_H__
#define __HR_SENSOR_H__

/****************************************************************************************
*							        Macro                                               *
****************************************************************************************/
#define APP_HRS_FAST_ADV				(100) //100 ms
/** @brief APP_HRS_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x3FFF in seconds, 0x0000 disables time-out.*/
#define APP_HRS_ADV_TIMEOUT				(1000) // 100 Secs
/** @brief scan_resp_len is the length of the scan response data */
#define SCAN_RESP_LEN					(10)
/** @brief ADV_DATA_LEN */
#define ADV_DATA_LEN					(18)
/** @brief ADV_TYPE_LEN */
#define ADV_TYPE_LEN					(0x01)
/** @brief HRS_ADV_DATA_UUID_LEN the size of  HRS service uuid */
#define HRS_ADV_DATA_UUID_LEN			(2)
/** @brief HRS_ADV_DATA_UUID_TYPE the total sizeof HRS service uuid*/
#define HRS_ADV_DATA_UUID_TYPE			(0x03)
/** @brief HRS_ADV_DATA_NAME_TYPE the gap ad data type */
#define HRS_ADV_DATA_NAME_TYPE			(0x09)
/* @brief HRS_ADV_DATA_NAME_DATA the actual name of device */
#define HRS_ADV_DATA_NAME_DATA			("ATMEL-HRT")
/** @brief HRS_ADV_DATA_NAME_LEN the length of the device name */
#define HRS_ADV_DATA_NAME_LEN			(9)

/**@brief time limit of last activity*/
#define START_OF_FIRST_ACTIVITY												(0)
#define END_OF_LAST_ACTIVITY												(200)

/**@brief Activities of user */
#define ACTIVITY_NORMAL										                 (0)
#define ACTIVITY_WALKING								                     (1)
#define ACTIVITY_BRISK_WALKING						                         (2)
#define ACTIVITY_RUNNING						                             (3)
#define ACTIVITY_FAST_RUNNING				                                 (4)
#define DEFAULT_ACTIVITY                                                     (0xFF)

/**@brief energy expended for various activities*/
#define ENERGY_EXP_NORMAL                                                    (3)
#define ENERGY_EXP_WALKING                                                   (10)
#define ENERGY_EXP_BRISK_WALKING                                             (20)
#define ENERGY_EXP_RUNNING                                                   (40)
#define ENERGY_EXP_FAST_RUNNING                                              (60)

/**@brief Heart rate increment value*/
#define HEART_RATE_INCREMENT_VALUE											(1)

/**@brief Heart Rate maximum and minimum values for various activities*/
#define HEART_RATE_MIN_NORM                                                 (50)
#define HEART_RATE_MAX_NORM                                                 (70)

#define HEART_RATE_MIN_WALKING                                              (71)
#define HEART_RATE_MAX_WALKING                                              (90)

#define HEART_RATE_MIN_BRISK_WALK                                           (91)
#define HEART_RATE_MAX_BRISK_WALK                                           (110)

#define HEART_RATE_MIN_RUNNING                                              (111)
#define HEART_RATE_MAX_RUNNING                                              (130)

#define HEART_RATE_MIN_FAST_RUNNING                                         (130)
#define HEART_RATE_MAX_FAST_RUNNING                                         (151)

/**@brief RR interval  maximum and minimum values for simulation of rr values */
#define RR_VALUE_MAX                                                        (1000)
#define RR_VALUE_MIN                                                        (100)

#define ENERGY_RESET                                                        (0)

/**@brief The maximum length of the hr measurement characteristic data*/
#define HR_CHAR_VALUE_LEN                                                   (10)

/**@brief HR_VALUE_FORMAT_UINT16 the bit to represent hr value sent is 16 bit*/
#define HR_VALUE_FORMAT_UINT16												(0x1 << 0)

/**@brief SENSOR_CONTACT_FTR_NOT_SPRTD_NOT_CONTACT to represent the sensor
 *contact info*/
#define SENSOR_CONTACT_FTR_NOT_SPRTD_NOT_CONTACT							(0x1 << 2)
/**@brief SENSOR_CONTACT_FTR_NOT_SPRTD_NOT_CONTACT to represent the sensor
 *contact info*/
#define SENSOR_CONTACT_FTR_NOT_SPRTD_CONTACT_DTD							(0x3 << 1)

/**@brief ENERGY_EXPENDED_FIELD_PRESENT represents energy expended inclusion*/
#define ENERGY_EXPENDED_FIELD_PRESENT										(0x1 << 3)

/**@brief RR_INTERVAL_VALUE_PRESENT represents the rr value inclusion*/
#define RR_INTERVAL_VALUE_PRESENT                                           (0x1 <<	4)

/**@brief Notification interval to determine the frequency of notifications */
#define NOTIFICATION_INTERVAL									            (1000)

extern uint8_t activity;
extern uint16_t energy_expended_val;
extern uint8_t energy_inclusion;
extern uint16_t energy_incrementor;
extern uint16_t heart_rate_value;
extern uint8_t hr_min_value;
extern uint8_t hr_max_value;
extern uint8_t inc_changer;
extern uint8_t notification_flag;
extern uint8_t prev_activity;
extern uint8_t second_counter;
extern bool volatile timer_cb_done;
extern uint8_t time_operator ;
/****************************************************************************************
*							        Function Prototypes	                                *                                                        *
****************************************************************************************/
at_ble_status_t ble_heart_rate_start_advertise(void);

/** @brief hr_sensor_init initializes and defines the services of the hr profile*/
void ble_heart_rate_profile_init(void);

void heart_rate_send_notification(uint8_t * char_data,uint8_t length);

void ble_heart_rate_handle_paired_event(at_ble_handle_t conn_handle);

void ble_heart_rate_handle_disconnect_event(at_ble_handle_t conn_handle);

#endif /*__HR_SENSOR_H__ */
