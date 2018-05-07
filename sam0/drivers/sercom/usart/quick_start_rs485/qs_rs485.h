/**
 * \file
 *
 * \brief SAM USART RS485 Quick Start
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
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

/**
 * \page asfdoc_sam0_sercom_usart_rs485_use_case Quick Start Guide for SERCOM USART RS485
 *
 * The supported board list:
 *    - SAMC21 Xplained Pro
 *
 * The RS-485 Xplained Pro Board must be connected to extension
 * header 1 on the SAM C21 Xplained Pro.
 
 * This quick start demonstrate how to configure RS485 mode, according to your
 * configuration \c CONF_RS485_MODE.
 * For RS485 master, it can send message to slave.
 * For RS485 salve, it can receive message from slave.
 *
 * To use the example:
 * 1. Install jumper between pins 2 and 3 on J101. This enables the TE pin to manage 
 * both DE and RE for Half-Duplex operation.
 * 2. Install jumper between pins 3 and 4 on J102. This removes the Profibus Termination for A/B.
 * 3. Install jumper between pins 1 and 2 on J103. This removes the Profibus Termination for Z/Y.
 * 4. Install jumpers on J106 and J107. This connects A/Y and B/Z to form half-duplex A/B differential pair.
 * Alternatively, this can be done by jumpering RXP to TXP and RXN to TXN, or pins 2 and 5 and
 * pins 3 and 4 on the screw terminal block.
 *
 * Refer to: http://www.atmel.com/Images/Atmel-42468-RS-485-Communications-using-the-SAM-C21-Xplained-Pro-and-RS-485-Xplained-Pro_ApplicationNote_AT8547.pdf
 * for more information
 */
