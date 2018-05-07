/**
 * \file
 *
 * \brief iBeacon Application Declarations
 *
 * Copyright (c) 2016-2017 Atmel Corporation. All rights reserved.
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
#ifndef __IBEACON_APP_H__
#define __IBEACON_APP_H__


/****************************************************************************************
*							        Macros	                                     		*
****************************************************************************************/

/* Beacon identifier value */
#define BEACON_IDENTIFIER (0x46)

/* Beacon advertisement Interval */
#define BEACON_ADV_INTERVAL (1600) //in 0.625ms

/* Beacon Advertisement timeout */
#define BEACON_ADV_TIMEOUT	(0) //in 0.625ms

/* Beacon Advertise at exact interval */
#define BEACON_ABSOLUTE_INTERVAL_ADV (true)

/* Beacon Advertisement data */
#define ADV_TYPE_MANUFACTURER_SPECIFIC_DATA			(0xff)
#define COMPANY_IDENTIFIER_CODE						("\x4c\x00")
#define COMPANY_IDENTIFIER_CODE_LENGTH				(0x02)
#define ADDTIONAL_MANUFACTURER_SPECIFIC_DATA		("\x02\x15\x21\x8a\xf6\x52\x73\xe3\x40\xb3\xb4\x1c\x19\x53\x24\x2c\x72\xf4\x00\xbb\x00\x45\xc5")
#define ADDTIONAL_MANUFACTURER_SPECIFIC_DATA_LENGTH	(0x17)
#define BEACON_ADV_LENGTH							(0x1a)
#define BEACON_ADV_TYPE								ADV_TYPE_MANUFACTURER_SPECIFIC_DATA
#define BEACON_ADV_TYPE_LENGTH						(0x01)
#define BEACON_SCAN_RESPONSE_LENGTH					(0x00)

#endif /* __IBEACON_APP_H__ */
