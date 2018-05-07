/**
 *
 * \file
 *
 * \brief This module contains SAMV71 BSP APIs implementation.
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
/**@defgroup  SAMSDIO Defines
 * @ingroup sam_sdio
 * @{
 */
#ifndef _SAM_SDIO_H_
#define _SAM_SDIO_H_

#include "compiler.h"
#include "samv71_xplained_ultra.h"
#include "../include/nm_bus_wrapper.h"

#ifdef CONF_WILC_USE_SDIO 
/**
*	@fn		nmi_sdio_cmd52
*	@brief	SDIO Command 52
*	@return	M2M_SUCCESS in case of success and <0 in case of failure
*/ 
int8_t nmi_sdio_cmd52(tstrNmSdioCmd52* cmd);

/**
*	@fn		nmi_sdio_cmd53
*	@brief	SDIO Command 53
*	@return	M2M_SUCCESS in case of success and <0 in case of failure
*/ 
int8_t nmi_sdio_cmd53(tstrNmSdioCmd53* cmd);

/**
*	@fn		Samv7SDIO_init
*	@brief	SamV7 SDIO Init
*	@return	M2M_SUCCESS in case of success and <0 in case of failure
*/ 
int8_t Samv7SDIO_init(void);

/**
*	@fn		sdio_reset
*	@brief	Reset SDIO
*	@return	N.A
*/ 
void sdio_reset(void);
#endif /* CONF_WILC_USE_SDIO */
#endif	/*_SAM_SDIO_H_*/