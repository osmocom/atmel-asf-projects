/**
 *
 * \file
 *
 * \brief WINC3400 configuration.
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

#ifndef CONF_WINC_H_INCLUDED
#define CONF_WINC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

/* This configuration files comes with empty settings! */
/* Default settings for SAM4S Xplained Pro with WINC on EXT1 are */
/* available in comments for reference. */
#warning Please modify conf_winc.h for WINC module settings!

/*
   ---------------------------------
   ---------- PIN settings ---------
   ---------------------------------
*/

#define CONF_WINC_PIN_RESET				0 /*IOPORT_CREATE_PIN(PIOA, 18)*/
#define CONF_WINC_PIN_CHIP_ENABLE		0 /*IOPORT_CREATE_PIN(PIOA, 6)*/
#define CONF_WINC_PIN_WAKE				0 /*IOPORT_CREATE_PIN(PIOA, 25)*/

/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

#define CONF_WINC_USE_SPI				(1)

/** SPI pin and instance settings. */
#define CONF_WINC_SPI					0 /*SPI*/
#define CONF_WINC_SPI_ID				0 /*ID_SPI*/
#define CONF_WINC_SPI_MISO_GPIO			0 /*SPI_MISO_GPIO */
#define CONF_WINC_SPI_MISO_FLAGS		0 /*SPI_MISO_FLAGS*/
#define CONF_WINC_SPI_MOSI_GPIO			0 /*SPI_MOSI_GPIO*/
#define CONF_WINC_SPI_MOSI_FLAGS		0 /*SPI_MOSI_FLAGS*/
#define CONF_WINC_SPI_CLK_GPIO			0 /*SPI_SPCK_GPIO*/
#define CONF_WINC_SPI_CLK_FLAGS			0 /*SPI_SPCK_FLAGS*/
#define CONF_WINC_SPI_CS_GPIO			0 /*SPI_NPCS0_GPIO*/
#define CONF_WINC_SPI_CS_FLAGS			0 /*PIO_OUTPUT_1*/
#define CONF_WINC_SPI_NPCS				(0)

/** SPI delay before SPCK and between consecutive transfer. */
#define CONF_WINC_SPI_DLYBS				(0)
#define CONF_WINC_SPI_DLYBCT 			(0)

/** SPI interrupt pin. */
#define CONF_WINC_SPI_INT_PIN			0 /*IOPORT_CREATE_PIN(PIOA, 1)*/
#define CONF_WINC_SPI_INT_PIO			0 /*PIOA*/
#define CONF_WINC_SPI_INT_PIO_ID		0 /*ID_PIOA*/
#define CONF_WINC_SPI_INT_MASK			0 /*(1 << 1)*/
#define CONF_WINC_SPI_INT_PRIORITY		(0)

/** Clock polarity & phase. */
#define CONF_WINC_SPI_POL				(0)
#define CONF_WINC_SPI_PHA				(1)

/** SPI clock. */
#define CONF_WINC_SPI_CLOCK				(48000000)

/*
   ---------------------------------
   --------- Debug Options ---------
   ---------------------------------
*/

#define CONF_WINC_DEBUG					(1)
#define CONF_WINC_PRINTF				printf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WINC_H_INCLUDED */
