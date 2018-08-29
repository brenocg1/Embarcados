/**
 * \file   dmtimerCounter.c
 *
 * \brief  Sample application for DMTimer. The application will
 *         count down from 9-0.
 *
 *         Application Configuration:
 *
 *             Modules Used:
 *                 DMTimer2
 *                 UART0
 *
 *             Configurable parameters:
 *                 None.
 *
 *             Hard-coded configuration of other parameters:
 *                 Mode of Timer - Timer mode(Auto reload)
 *
 *         Application Use Case:
 *             The application demonstrates DMTimer in Autoreload mode
 *             of operation. In the example for every overflow of DMTimer
 *             the counter register is reloaded with contents of overflow
 *             register. This sequence is continued 10 times and at each
 *             overflow a decrementing value is printed on the serial
 *             console showcasing the DMTimer as a down counter.
 *
 *         Running the example:
 *             On execution, the example will count down from 9 - 0 and stop.
 *             The time interval between each count is approximate to 700ms.
 *
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
*/
/* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "consoleUtils.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "interrupt.h"
#include "dmtimer.h"
#include "error.h"

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define TIMER_INITIAL_COUNT            (0xFF000000u)
#define TIMER_RLD_COUNT                (0xFFFFFF83u) //(0xFF000000u)

#define T_1MS_COUNT                     (0x5DC0u) 
#define OVERFLOW                        (0xFFFFFFFFu)
#define TIMER_1MS_COUNT                 (0x5DC0u) 


/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void DMTimerSetUp(void);
static void Delay(volatile unsigned int mSec);


/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int cntValue = 10;

/******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/
int main(void){
    int mSec=5000;

    /* This function will enable clocks for the DMTimer2 instance */
    DMTimer2ModuleClkConfig();

    /* Initialize the UART console */
    ConsoleUtilsInit();

    /* Select the console type based on compile time check */
    ConsoleUtilsSetType(CONSOLE_UART);

    /* Perform the necessary configurations for DMTimer */
    DMTimerSetUp();

    ConsoleUtilsPrintf("\n\r");
    ConsoleUtilsPrintf("intCounter: ");
    
    while(cntValue){
     ConsoleUtilsPrintf(" %d", (cntValue-1));
     Delay(mSec);
     cntValue--;
    }

    PRINT_STATUS(S_PASS);

    /* Halt the program */
    while(1);
}

/*
** Setup the timer for one-shot and compare mode.
*/
static void DMTimerSetUp(void){
    DMTimerReset(SOC_DMTIMER_2_REGS);

    /* Load the counter with the initial count value */
    //DMTimerCounterSet(SOC_DMTIMER_2_REGS, TIMER_INITIAL_COUNT);

    /* Load the load register with the reload count value */
    //DMTimerReloadSet(SOC_DMTIMER_2_REGS, TIMER_RLD_COUNT);

    /* Configure the DMTimer for Auto-reload and compare mode */
    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
}

/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void Delay(volatile unsigned int mSec){
   while(mSec != 0){
        DMTimerCounterSet(SOC_DMTIMER_2_REGS, 0);
        DMTimerEnable(SOC_DMTIMER_2_REGS);
        while(DMTimerCounterGet(SOC_DMTIMER_2_REGS) < TIMER_1MS_COUNT);
        DMTimerDisable(SOC_DMTIMER_2_REGS);
        mSec--;
    }
}

