/**
 * @file timer.c
 *
 * @brief D21 implementation of the timer interface.
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
#include "AWS_SDK/aws_iot_src/protocol/mqtt/aws_iot_embedded_client_wrapper/timer_interface.h"
extern volatile uint32_t ms_ticks;
/**
 * \brief Get time in milliseconds.
 *
 * \return milli second ticks count.
 */
static uint32_t getTimeInMillis(void)
{
	return ms_ticks;
}
/**
 * \brief Timer expiry check function.
 *
 * \param[in] timer Timer to be checked.
 *
 * \return time to expiry.
 */
char expired(Timer *timer)
{
	return ((timer->end_time > 0)
	&& ((getTimeInMillis() + timer->offset) > timer->end_time));
}
/**
 * \brief count down timer function.
 *
 * \param[in] timer Timer to be started.
 * \param[in] timeout timeout in milliseconds.
 *
 * \return None.
 */
void countdown_ms(Timer *timer, unsigned int timeout)
{
	uint32_t timems = getTimeInMillis();

	timer->end_time = timems + timeout;

	if (timer->end_time < timems) {
		timer->offset = ~0 - timems + 1;
		timer->end_time += timer->offset;
	}
	else {
		timer->offset = 0;
	}
}
/**
 * \brief count down timer function.
 *
 * \param[in] timer Timer to be started.
 * \param[in] timeout timeout in seconds.
 *
 * \return None.
 */
void countdown(Timer *timer, unsigned int timeout)
{
	uint32_t timems = getTimeInMillis();

	timer->end_time = timems + (timeout * 1000);

	if (timer->end_time < timems) {
		timer->offset = ~0 - timems + 1;
		timer->end_time += timer->offset;
	}
	else {
		timer->offset = 0;
	}
}
/**
 * \brief Time avaible for timer expiry function.
 *
 * \param[in] timer Timer to be started.
 *
 * \return Time for expiry in milliseconds.
 */
int left_ms(Timer *timer)
{
	int diff = timer->end_time - (getTimeInMillis() + timer->offset);
	return (diff > 0 ? diff : 0);
}
/**
 * \brief Timer intialization function.
 *
 * \param[in] timer Timer to be intialized.
 *
 * \return None.
 */
void InitTimer(Timer *timer)
{
	timer->end_time = 0;
	timer->offset = 0;
}