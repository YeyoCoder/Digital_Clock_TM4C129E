/*
 * Author: Diego Olvera
 * Date: August 7th, 2022
 * Disclaimer. This software is provided AS IT IS WITHOUT ANY WARRANTY.
 *
 */

/*
 * Digital clock using TIMER A each second. Over time this will have no sync.
 *
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
/*
 * System clock rate in Hz
 */
uint32_t ui32SysClock;

/*
 * Global variables for the clock
 */
uint8_t ui8Hour = 0; /* Hours, range 0 to 23 */
uint8_t ui8Minutes = 0; /* Minutes, range 0 to 59 */
uint8_t ui8Seconds = 0; /* Seconds, range 0 to 59 */

/*
 * The interrupt handler for the timer 0
 */
void Timer0IntHandler(void)
{
    /*
     * Clear the timer interrupt
     */
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /*
     * Increment the clock cycle
     */
    if(ui8Seconds < 59){
        ui8Seconds++;
    }else{
        ui8Seconds = 0;
        if(ui8Minutes < 59){
            ui8Minutes++;
        }else{
            ui8Minutes = 0;
            if(ui8Hour < 23){
                ui8Hour++;
            }else{
                ui8Hour = 0;
            }
        }
    }

}
/**
 * digital_clock.c
 */
int main(void)
{
    /* Set the clock to run from the crystal at 120 MHz */
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_480), 120000000);

    /* Enable peripherals used in this code */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    /* Enable processor interrupts */
    IntMasterEnable();

    /* Configure the 32 bit periodic timer */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32SysClock);

    /* Set the Interrupt */
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* Enable the timer */
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1){

    }

	//return 0;
}
