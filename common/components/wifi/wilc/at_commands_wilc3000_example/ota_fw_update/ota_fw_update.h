/**
 * \file
 *
 * \brief OTA Firmware Update Configuration
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

#ifndef OTA_FW_UPDATE_H_INCLUDED
#define OTA_FW_UPDATE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if SAM4SD32
/** Sector details of flash1 */
#define SECT0_SMALLERSECTOR_SIZE		8192  //8KB
#define SECT0_LARGERSECTOR_SIZE			49152 //48KB
#define SECTOR_SIZE						65536 //64KB
#define SMALLERSECTOR_COUNT				2
#define NO_OF_SECTORS					16

#define SUCCESS							0
#define FAILURE							-1

void ota_firmware_update(char *fileURL);
int write_to_flash(char *data, uint32_t length, uint16_t is_last_chunk);
#endif
#ifdef __cplusplus
}
#endif

#endif /* OTA_FW_UPDATE_H_INCLUDED */
