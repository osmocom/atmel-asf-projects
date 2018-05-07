/**
* \file  mimem.c
*
* \brief Memory Buffer handling for MiWi Protocol implementation
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
#include "compiler.h"
#include "mimem.h"

/************************ MACRO DEFINITIONS ******************************/
/* The size of a single mimem buffer */
#define MIMEM_BUFFER_SIZE      160

/************************ TYPE DEFINITIONS ******************************/
typedef struct MiMemBuffer
{
  uint32_t MiMemBuffer[(MIMEM_BUFFER_SIZE / 4) + 1];
  bool bufferFlag;
}MiMemBuffer_t;

/************************ FUNCTION PROTOTYPES **********************/

/************************ VARIABLES ********************************/
MiMemBuffer_t mimemBuffers[NUMBER_OF_MIMEM_BUFFERS];

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void MiMem_Init()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    none
*
* Overview:		    This function will initialize the Mimem buffers
********************************************************************/
void MiMem_Init(void)
{
    uint8_t loopIndex = 0;
    for (loopIndex = 0; loopIndex < NUMBER_OF_MIMEM_BUFFERS; loopIndex++)
    {
	    mimemBuffers[loopIndex].bufferFlag = false;
    }
}

/*********************************************************************
* Function:         uint8_t* MiMem_Alloc(uint8_t size)
*
* PreCondition:     none
*
* Input:		    size  - Required number of bytes
*
* Output:		    uint8_t*_t - Pointer to the allocated memory or NULL
*
* Side Effects:	    none
*
* Overview:		    This function returns valid pointer if memory is allocated
*  or returns NULL if no memory available
*
* Note:			    none
********************************************************************/
uint8_t* MiMem_Alloc(uint8_t size)
{
    uint8_t loopIndex = 0;

    if (size <= MIMEM_BUFFER_SIZE)
    {
        for (loopIndex = 0; loopIndex < NUMBER_OF_MIMEM_BUFFERS; loopIndex++)
        {
            if (mimemBuffers[loopIndex].bufferFlag == false)
            {
                mimemBuffers[loopIndex].bufferFlag = true;
                //printf("\r\n MiMem Buffer Alloc: %d", loopIndex);
                return ((uint8_t *)mimemBuffers[loopIndex].MiMemBuffer);
            }
        }
    }
    //printf("\r\n MiMem Buffer Full/unavailable for given size");
    return NULL;
}

/*********************************************************************
* Function:         uint8_t MiMem_Free(uint8_t* buffPtr)
*
* PreCondition:     none
*
* Input:		    buffPtr  - Pointer to memory to be deallocated
*
* Output:		    uint8_t - status of the operation
*
* Side Effects:	    none
*
* Overview:		    This function returns SUCCESS if it deallocates
* the given memory else returns error
*
* Note:			    none
********************************************************************/
uint8_t MiMem_Free(uint8_t* buffPtr)
{
	uint8_t loopIndex = 0;
	for (loopIndex = 0; loopIndex < NUMBER_OF_MIMEM_BUFFERS; loopIndex++)
	{
		if ((mimemBuffers[loopIndex].bufferFlag == true) &&
		   ((uint8_t *)mimemBuffers[loopIndex].MiMemBuffer == buffPtr))
		{
			//printf("\r\n MiMem Buffer Free: %d", loopIndex);
			mimemBuffers[loopIndex].bufferFlag = false;
			return 0;
		}
	}
	//printf("\r\n MiMem Buffer Already Free");
	return 0xff;
}


uint8_t MiMem_PercentageOfFreeBuffers(void)
{
	uint8_t loopIndex = 0;
	uint8_t numUsedBuffers = 0;
	for (loopIndex = 0; loopIndex < NUMBER_OF_MIMEM_BUFFERS; loopIndex++)
	{
		if (mimemBuffers[loopIndex].bufferFlag == true)
		{
			numUsedBuffers++;
		}
	}
	return ((NUMBER_OF_MIMEM_BUFFERS - numUsedBuffers) * 100) / NUMBER_OF_MIMEM_BUFFERS;
}