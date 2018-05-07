/**
 *
 * \file
 *
 * \brief This module contains Linux BSP APIs implementation.
 *
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

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include "NMI_WFI_NetDevice.h"
#include "linux_wlan_common.h"
#include "nm_bus_wrapper.h"
#include "m2m_wifi.h"
extern linux_wlan_t* g_linux_wlan;

extern void linux_wlan_free(void* vp);
extern void* linux_wlan_malloc(uint32_t sz);

static tpfNmBspIsr gpfIsr;
#define NM_BUS_MAX_TRX_SZ	512U
tstrNmBusCapabilities egstrNmBusCapabilities = 
{
	NM_BUS_MAX_TRX_SZ	
};


extern struct semaphore m2m_wait;
void linux_wlan_unlock(void* vp);


irqreturn_t isr_uh_routine(int irq, void* user_data)
{

	if (gpfIsr) 
	{
		gpfIsr();
	}
	//kthread_run(m2m_wifi_handle_events,(void*)g_linux_wlan,"K_RXQ_BH");
	//schedule_work(&g_linux_wlan->rx_work_queue);
	//printk("*** isr\n");
	linux_wlan_unlock(&m2m_wait);
	return IRQ_HANDLED;

}
void nm_bsp_reset(void)
{
}

static void init_chip_pins(void)
{
}

/*
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*	@author	M.S.M
*	@date	11 July 2012
*	@version	1.0
*/
sint8 nm_bsp_init(void)
{
	gpfIsr = NULL;

	
	return 0;
}
/*
*	@fn		nm_bsp_sleep
*	@brief	Sleep in units of mSec
*	@param[IN]	u32TimeMsec
*				Time in milliseconds
*	@author	M.S.M
*	@date	28 OCT 2013
*	@version	1.0
*/
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	msleep(u32TimeMsec);
}

/*
*	@fn		nm_bsp_register_isr
*	@brief	Register interrupt service routine
*	@param[IN]	pfIsr
*				Pointer to ISR handler
*	@author	M.S.M
*	@date	28 OCT 2013
*	@sa		tpfNmBspIsr
*	@version	1.0
*/
int init_irq(linux_wlan_t* p_nic);
int isr_bh_routine(void *vp);

void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;
	init_irq(g_linux_wlan);
	kthread_run(isr_bh_routine,NULL,"M2M_RXQ_BH");
	
}
/*
*	@fn		nm_bsp_interrupt_ctrl
*	@brief	Enable/Disable interrupts
*	@param[IN]	u8Enable
*				'0' disable interrupts. '1' enable interrupts
*	@author	M.S.M
*	@date	28 OCT 2013
*	@version	1.0
*/
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	static int Localstatus = 1;

	if(Localstatus == u8Enable)
		return;
	Localstatus = u8Enable;

	if (u8Enable) 
	{
		linux_wlan_enable_irq();		
	}
	else 
	{
		linux_wlan_disable_irq(0);		
	}
}

/*
*	@fn		nm_bsp_malloc
*	@brief	Allocate memory
*	@param [in]   u32Size
*               Size of the requested memory 
*	@return       Pointer to the allocated buffer, or NULL otherwise
*/
void* nm_bsp_malloc(uint32 u32Size)
{
	return linux_wlan_malloc(u32Size);
}

/*
*	@fn		nm_bsp_free
*	@brief	Free memory
*	@param [in]   pvMemBuffer
*               Pointer to the buffer to be freed 
*/
void nm_bsp_free(void* pvMemBuffer)
{
	linux_wlan_free(pvMemBuffer);
}

