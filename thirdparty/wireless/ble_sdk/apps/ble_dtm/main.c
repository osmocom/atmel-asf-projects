/**
 * \file
 *
 * \brief BLE DTM
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

 /**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the BLE DTM
 */
/*- Includes ---------------------------------------------------------------*/

#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "at_ble_trace.h"
#include "console_serial.h"
#include "serial_fifo.h"
#include "ble_utils.h"
#include "conf_serialdrv.h"
#include "serial_bridge.h"
#include "conf_extint.h"
#include "serial_drv.h"
#include "timer_hw.h"

#ifndef HOST_UART_BAUDRATE_CONFIG_VALUE
#define HOST_UART_BAUDRATE_CONFIG_VALUE 115200
#endif
			
/* Initialize the BLE */
static void ble_init(void);
/* Initialie the DTM Mode of BLE */
static void ble_dtm_init(void);

/* Critical Alert for BLE Failures */
static void ble_critical_alert(uint32_t on_ms, uint32_t off_ms);

volatile bool button_pressed = false;

at_ble_init_config_t pf_cfg;

uint32_t event_pool_memory[256] = {0};
uint32_t event_params_memory[256] = {0};
uint32_t rx_fifo_memory[256] = {0};
	
/* Alert the user when something is failed */
static void ble_critical_alert(uint32_t on_ms, uint32_t off_ms)
{
	while(1)
	{
		delay_ms(off_ms);
		LED_On(LED0);
		delay_ms(on_ms);
		LED_Off(LED0);
	}
}

/* Button Callback function */
void button_cb(void)
{
	button_pressed = true;
}

/* Initialize the BLE */
static void ble_init(void)
{
	pf_cfg.memPool.memSize = 0;
	pf_cfg.memPool.memStartAdd = NULL;
	
	pf_cfg.event_mem_pool.memStartAdd        = (uint8_t *)event_pool_memory;
	pf_cfg.event_mem_pool.memSize            = sizeof(event_pool_memory);
	pf_cfg.event_params_mem_pool.memStartAdd = (uint8_t *)event_params_memory;
	pf_cfg.event_params_mem_pool.memSize     = sizeof(event_params_memory);
	pf_cfg.rx_fifo_mem_pool.memStartAdd      = (uint8_t *)rx_fifo_memory;
	pf_cfg.rx_fifo_mem_pool.memSize          = sizeof(rx_fifo_memory);		

	/* BTLC1000 Wakeup Pin */
	#if (BLE_MODULE == BTLC1000_ZR)
	pf_cfg.btlc1000_module_version			 = AT_BTLC1000_ZR;
	#elif (BLE_MODULE == BTLC1000_MR)
	pf_cfg.btlc1000_module_version			 = AT_BTLC1000_MR;
	#else
	#error "Select a valid BLE_MODULE"
	#endif
	 
	/*Bus configuration*/
	pf_cfg.bus_info.bus_type = AT_BLE_UART;

	#if UART_FLOWCONTROL_6WIRE_MODE == true
	/* Enable Hardware Flow-control on BTLC1000 */
	pf_cfg.bus_info.btlc1000_uart_pinout_switch = true; // enable flow control
	#else
	/* Disable Hardware Flow-control on BTLC1000 */
	pf_cfg.bus_info.btlc1000_uart_pinout_switch = false; // Disable flow control
	#endif
	
	/// UART baudrate value one of @ref at_ble_uart_baudrate_tag values
	pf_cfg.bus_info.bus_baudrate = HOST_UART_BAUDRATE_CONFIG_VALUE;
	pf_cfg.bus_info.host_external_wakeup_gpio = BTLC1000_EXT_PIN9;
	
	/* Register Platform callback API's */
	pf_cfg.platform_api_list.at_ble_create_timer = platform_create_timer;
	pf_cfg.platform_api_list.at_ble_delete_timer = platform_delete_timer;
	pf_cfg.platform_api_list.at_ble_start_timer = platform_start_timer;
	pf_cfg.platform_api_list.at_ble_stop_timer = platform_stop_timer;
	pf_cfg.platform_api_list.at_ble_sleep = platform_sleep;
	pf_cfg.platform_api_list.at_ble_gpio_set = platform_gpio_set;
	pf_cfg.platform_api_list.at_ble_send_sync = platform_send_sync;
	pf_cfg.platform_api_list.at_ble_recv_async = platform_recv_async;
	pf_cfg.platform_api_list.at_ble_wait_for_signal = platform_wait_for_signal;
	pf_cfg.platform_api_list.at_ble_create_signal = platform_create_signal;
	pf_cfg.platform_api_list.at_ble_delete_signal = platform_delete_signal;
	pf_cfg.platform_api_list.at_ble_trigger_signal = platform_trigger_signal;
	pf_cfg.platform_api_list.at_ble_reset_signal = platform_reset_signal;
	
	pf_cfg.platform_api_list.at_ble_reconfigure_usart = pf_cfg.bus_info.btlc1000_uart_pinout_switch ? platform_configure_hw_fc_uart : platform_configure_primary_uart;
	
	platform_init(pf_cfg.bus_info.bus_type, pf_cfg.btlc1000_module_version);
	
	/*Trace Logs*/
	trace_set_level(TRACE_LVL_DISABLE);
	
	/* Initialize BLE device */
	if(at_ble_init(&pf_cfg) != AT_BLE_SUCCESS)
	{
		ble_critical_alert(40, 250);
	}
		
	/* Change the UART Rx Callback Pointer */
	platform_recv_async(platform_dtm_interface_receive);
}

/* Initialize the direct test mode */
static void ble_dtm_init(void)
{
	/* initialize the BLE chip */
	ble_init();	
	LED_On(LED0);
}

/* main entry to the Direct Test mode Application */
int main (void)
{	

#if SAMG55 || SAM4S
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
#elif SAM0
	system_init();
#endif	

	delay_init();

	/* Button Init */
	button_init();
	
	/* DTM Initialization */
	ble_dtm_init();
	
	platform_stop_stack_timers();
		
	pf_cfg.platform_api_list.at_ble_create_timer = NULL;
	pf_cfg.platform_api_list.at_ble_delete_timer = NULL;
	pf_cfg.platform_api_list.at_ble_start_timer = NULL;
	pf_cfg.platform_api_list.at_ble_stop_timer = NULL;
	pf_cfg.platform_api_list.at_ble_sleep = NULL;
	pf_cfg.platform_api_list.at_ble_gpio_set = NULL;
	pf_cfg.platform_api_list.at_ble_send_sync = NULL;
	pf_cfg.platform_api_list.at_ble_recv_async = NULL;
	pf_cfg.platform_api_list.at_ble_reconfigure_usart = NULL;
	
	platform_gpio_set(AT_BLE_EXTERNAL_WAKEUP, AT_BLE_HIGH);	
	
	/* Initialize serial bridge */
	serial_bridge_init();
	
	platform_start_rx();
	
	/* Task handle in while loop */
	while(1)
	{
		/* Serial bridge task */ 
		serial_bridge_task();
	}
}
