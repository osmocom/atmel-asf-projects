/**
* \file  symbol.c
*
* \brief Symbol Ticks for MiWi P2P/Star implementation
*
* Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries. 
*
* \asf_license_start
*
* \page License
*
* Subject to your compliance with these terms, you may use Microchip
* software and any derivatives exclusively with Microchip products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that 
* may accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, 
* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE 
* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL 
* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY 
* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
* \asf_license_stop
*
*/

/************************ HEADERS **********************************/
#include "system.h"
#include "symbol.h"
#include "common_hw_timer.h"

/************************ DEFINITIONS ******************************/

/************************ FUNCTION PROTOTYPES **********************/

/************************ VARIABLES ********************************/

volatile uint8_t timerExtension1,timerExtension2;

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void InitSymbolTimer()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Overview:		    This function will configure the hardware timer to 1MHz
*                   for tick calculation
********************************************************************/
void InitSymbolTimer()
{
    timerExtension1 = 0;
    timerExtension2 = 0;
	common_tc_init();
	set_common_tc_overflow_callback(hw_overflow_cb);
}

/*********************************************************************
* Function:         void InitSymbolTimer()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    MIWI_TICK - the current symbol time
*
* Side Effects:	    The timer interrupt is disabled
*                   for several instruction cycles while the 
*                   timer value is grabbed.  This is to prevent a
*                   rollover from incrementing the timer extenders
*                   during the read of their values
*
* Overview:		    This function returns the current time
*
* Note:			    This function should never be called 
*                   when interrupts are disabled if interrupts are 
*                   disabled when this is called then the timer 
*                   might rollover and the byte extension would not 
*                   get updated.
********************************************************************/
uint32_t MiWi_TickGet(void)
{
    MIWI_TICK currentTime;
	uint8_t current_timerExtension1 = timerExtension1;
    
    /* disable the timer to prevent roll over of the lower 16 bits 
	while before/after reading of the extension */
	tmr_disable_ovf_interrupt();

	currentTime.word.w0 = common_tc_read_count();
    /* enable the timer*/
	tmr_enable_ovf_interrupt();

	/* NOP to allow the interrupt to process before copying timerExtension1 */
	nop();

	/* If interrupt occurred during interrupt disable  read count again*/
	if (current_timerExtension1 != timerExtension1)
	{
		currentTime.word.w0 = common_tc_read_count();
	}
    /* copy the byte extension */
    currentTime.byte.b2 = timerExtension1;
    currentTime.byte.b3 = timerExtension2;

    return currentTime.Val;
}

/*********************************************************************
* Function:         MIWI_TICK MiWi_TickGetDiff()
*
* PreCondition:     none
*
* Input:		    current_tick  - Recent tick read from MiWi_TickGet
*                   previous_tick - Tick read prior to current_tick for 
*                                   calculation.
*
* Output:		    uint32_t - Difference in current_tick to previous_tick
*
* Side Effects:	    none
*
* Overview:		    This function returns difference between current_tick
*                   and previous_tick
*
* Note:			    none
********************************************************************/
uint32_t MiWi_TickGetDiff(MIWI_TICK current_tick, MIWI_TICK previous_tick)
{
	uint32_t ret_val;
	if (current_tick.Val > previous_tick.Val)
	{
		/* If current > previous, no overflow in running timer*/
		ret_val = current_tick.Val - previous_tick.Val;
	}
	else
	{
		/* Handling Overflow as current tick < previous tick */
		ret_val = (0xFFFFFFFF - previous_tick.Val) + current_tick.Val;
	}
	return ret_val;
}

/*********************************************************************
* Function:         void hw_overflow_cb
*
* PreCondition:     none
*
* Input:		    none 
*
* Output:		    none
*
* Side Effects:	    none
*
* Overview:		    This is callback function for hardware timer overflow.
*
* Note:			    none
********************************************************************/
void hw_overflow_cb(void)
{
    timerExtension1++;
    if(timerExtension1 == 0)
    {
	    timerExtension2++;
    }
}