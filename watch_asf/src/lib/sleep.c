#include <ASF/sam0/drivers/system/clock/gclk.h>

void configure_sleep(void) {
    enum sleepmgr_mode current_sleep_mode = SLEEPMGR_STANDBY;
    sleepmgr_init();
    sleepmgr_lock_mode(current_sleep_mode);
}


void enterSleepMode(void) {
    prepareLEDsForSleep();

    disable_timer();
    volatile uint32_t clock = system_cpu_clock_get_hz();
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);
    system_clock_source_disable(SYSTEM_CLOCK_SOURCE_OSC8M);
    struct system_gclk_gen_config mainClock;
    system_gclk_gen_get_config_defaults(&mainClock);
    mainClock.source_clock = GCLK_SOURCE_XOSC32K;
    mainClock.run_in_standby = true;
	mainClock.division_factor = 32;
    system_gclk_gen_set_config(0,&mainClock);
    clock = system_cpu_clock_get_hz();
    system_clock_source_disable(SYSTEM_CLOCK_SOURCE_DFLL);
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	//NVIC_DisableIRQ(EIC_IRQn);
	NVIC_ClearPendingIRQ(EIC_IRQn);
	//NVIC_EnableIRQ(EIC_IRQn);
	
	//PM->SLEEP.reg = PM_SLEEP_IDLE(2); // Stop CPU, AHB and APB clocks
	PM->SLEEP.reg = PM_SLEEP_IDLE(2); // Stop CPU, AHB and APB clocks
	  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		  __WFI();

	
	//sleepmgr_sleep(SLEEPMGR_STANDBY); 
    //sleepmgr_enter_sleep();
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	//turnOnLED(0);

    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL);

    mainClock.source_clock = GCLK_SOURCE_DFLL48M;
    mainClock.division_factor = 1;
    system_gclk_gen_set_config(0,&mainClock);
    system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC8M);
    exitSleepMode();
	//turnOffLED(0);
}

void exitSleepMode(void) {
    enable_timer();
    getRTCTime();
}
