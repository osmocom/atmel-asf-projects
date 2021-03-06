/**
 *
 * \file
 *
 * \brief This module contains SAMG55 BSP APIs implementation.
 *
 * Copyright (c) 2016-2018 Atmel Corporation. All rights reserved.
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

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "asf.h"
#include "conf_wilc.h"

static tpfNmBspIsr gpfIsr;

static void chip_isr(uint32_t id, uint32_t mask)
{
	if ((id == CONF_WILC_SPI_INT_PIO_ID) && (mask == CONF_WILC_SPI_INT_MASK)) {
		if (gpfIsr) {
			gpfIsr();
		}
	}
}

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
	ioport_init();
	ioport_set_pin_dir(CONF_WILC_PIN_RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CONF_WILC_PIN_RESET, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(CONF_WILC_PIN_CHIP_ENABLE, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CONF_WILC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(CONF_WILC_PIN_WAKE, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CONF_WILC_PIN_WAKE, IOPORT_PIN_LEVEL_HIGH);
}

/*
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*/
sint8 nm_bsp_init(void)
{
	gpfIsr = NULL;

	/* Initialize chip IOs. */
	init_chip_pins();

    /* Make sure a 1ms Systick is configured. */
    if (!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk && SysTick->CTRL & SysTick_CTRL_TICKINT_Msk)) {
	    delay_init();
    }

	/* Perform chip reset. */
	nm_bsp_reset();

	return 0;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset WINC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
	ioport_set_pin_level(CONF_WILC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(CONF_WILC_PIN_RESET, IOPORT_PIN_LEVEL_LOW);
	nm_bsp_sleep(100);
	ioport_set_pin_level(CONF_WILC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_HIGH);
	nm_bsp_sleep(100);
	ioport_set_pin_level(CONF_WILC_PIN_RESET, IOPORT_PIN_LEVEL_HIGH);
	nm_bsp_sleep(100);
}

/*
*	@fn		nm_bsp_sleep
*	@brief	Sleep in units of mSec
*	@param[IN]	u32TimeMsec
*				Time in milliseconds
*/
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	while(u32TimeMsec--) {
		delay_ms(1);
	}
}

/*
*	@fn		nm_bsp_register_isr
*	@brief	Register interrupt service routine
*	@param[IN]	pfIsr
*				Pointer to ISR handler
*/
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;

	/* Configure PGIO pin for interrupt from SPI slave, used when slave has data to send. */
	sysclk_enable_peripheral_clock(CONF_WILC_SPI_INT_PIO_ID);
	pio_configure_pin(CONF_WILC_SPI_INT_PIN, PIO_TYPE_PIO_INPUT);
	pio_pull_up(CONF_WILC_SPI_INT_PIO, CONF_WILC_SPI_INT_MASK, PIO_PULLUP);
//	pio_set_debounce_filter(CONF_WILC_SPI_INT_PIO, CONF_WILC_SPI_INT_MASK, 10);
	pio_handler_set_pin(CONF_WILC_SPI_INT_PIN, PIO_IT_LOW_LEVEL, chip_isr);
	/* The status register of the PIO controller is cleared prior to enabling the interrupt */
	pio_get_interrupt_status(CONF_WILC_SPI_INT_PIO);
	pio_enable_interrupt(CONF_WILC_SPI_INT_PIO, CONF_WILC_SPI_INT_MASK);
	pio_handler_set_priority(CONF_WILC_SPI_INT_PIO, (IRQn_Type)CONF_WILC_SPI_INT_PIO_ID,
			CONF_WILC_SPI_INT_PRIORITY);
}

/*
*	@fn		nm_bsp_interrupt_ctrl
*	@brief	Enable/Disable interrupts
*	@param[IN]	u8Enable
*				'0' disable interrupts. '1' enable interrupts
*/
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	if (u8Enable) {
		/* The status register of the PIO controller is cleared prior to enabling the interrupt */
		pio_get_interrupt_status(CONF_WILC_SPI_INT_PIO);
		pio_enable_interrupt(CONF_WILC_SPI_INT_PIO, CONF_WILC_SPI_INT_MASK);
	}
	else {
		pio_disable_interrupt(CONF_WILC_SPI_INT_PIO, CONF_WILC_SPI_INT_MASK);
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
	return malloc(u32Size);
}

/*
*	@fn		nm_bsp_free
*	@brief	Free memory
*	@param [in]   pvMemBuffer
*               Pointer to the buffer to be freed 
*/
void nm_bsp_free(void* pvMemBuffer)
{
	free(pvMemBuffer);
}

