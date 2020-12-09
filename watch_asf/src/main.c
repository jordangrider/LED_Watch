 /**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include <gclk.h>
#include <math.h>
#include <lib/sleep.h>
#include <lib/pwmDriver.c>
#include <lib/ledDriver.c>
#include <lib/timer.c>
#include <lib/i2c.c>
#include <lib/interrupt.c>
#include <lib/rtc.c>
#include <lib/sleep.c>
#include <lib/displayTime.c>
#include <lib/testAnimations.c>
#include <lib/slotmanager.c>
#include <lib/statemachine.c>


static bool interrupt_pin_triggered = false;
static uint8_t startUp = 2;

void timeSlot_1ms(void) {
    driveLEDs();
}

void timeSlot_10ms(void) {
    accelerometerLoop();
}

void timeSlot_10ms_2(void) {
    runStateMachine();
}

void timeSlot_100ms(void) {
}

void timeSlot_1000ms(void) {
	/*
	for (uint8_t i = 0; i < 60; i++){
	uint8_t outputValue = 3;
	if(i % 5 == 0){
		outputValue = 9;
	}
	switchLED(i, outputValue);
	}
	*/
    //ledHeartbeat(30);
    updateTime();

    if(startUp > 0) {
        startUp = startUp - 1;
    }

}



int main (void) {
    system_init();
    ioport_init();
   
    configure_rtc();
    configure_tc();
	configure_tcc();
    setLEDPinsAsOutputs();

    configure_i2c();
    configure_i2c_callbacks();
	eic_init();	
   
    while(1) {
		slotManager();
    }

}
