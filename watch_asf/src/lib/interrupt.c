#define INTERRUPT_PIN 10 //PA10

static volatile bool wake_interrupt_triggered = false;
struct extint_chan_conf wake_interrupt;
struct extint_nmi_conf wake_interrupt_nmi;

volatile struct Eic *returnEic;

#define WAKE_INTERRUPT_CHANNEL 10

void wake_interrupt_callback(void) {
    wake_interrupt_triggered = true;
	extint_chan_clear_detected(WAKE_INTERRUPT_CHANNEL);
    exitSleepMode();
}

#include "hal_gpio.h"
HAL_GPIO_PIN(BUTTON,   A, 10)

void eic_init(void)
{
	HAL_GPIO_BUTTON_in();
	//HAL_GPIO_BUTTON_pullup();
	HAL_GPIO_BUTTON_pmuxen(PORT_PMUX_PMUXE_A_Val);

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(EIC_GCLK_ID) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	EIC->CTRL.reg = EIC_CTRL_ENABLE;

	EIC->CONFIG[1].reg = EIC_CONFIG_SENSE2_RISE;

	EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN10;
	EIC->INTENSET.reg = EIC_INTENSET_EXTINT10;

	NVIC_EnableIRQ(EIC_IRQn);
}

void irq_handler_eic(void)
{
	EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT10;
	
}


void configure_interrupt(void) {

    //_extint_get_eic_from_channel(0);
	
	/*
	extint_nmi_get_config_defaults(&wake_interrupt_nmi);
	wake_interrupt_nmi.detection_criteria = EXTINT_DETECT_LOW;
	wake_interrupt_nmi.gpio_pin = PIN_PA10A_EIC_EXTINT10;
	wake_interrupt_nmi.gpio_pin_mux = MUX_PA10A_EIC_EXTINT10;
	wake_interrupt_nmi.gpio_pin_pull = EXTINT_PULL_NONE;

	returnEic = _extint_get_eic_from_channel(WAKE_INTERRUPT_CHANNEL);

	extint_nmi_set_config(WAKE_INTERRUPT_CHANNEL, &wake_interrupt);
	
	struct extint_events eventsList;
	for(uint8_t i = 0; i <= 31; i++){
		eventsList.generate_event_on_detect[i] = true;
	}
	extint_enable_events(&eventsList);
	
	extint_register_callback(wake_interrupt_callback,WAKE_INTERRUPT_CHANNEL,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(WAKE_INTERRUPT_CHANNEL,EXTINT_CALLBACK_TYPE_DETECT);
	extint_nmi_clear_detected(WAKE_INTERRUPT_CHANNEL);
	*/

	/*
    extint_chan_get_config_defaults(&wake_interrupt);
    wake_interrupt.detection_criteria = EXTINT_DETECT_HIGH;
    wake_interrupt.gpio_pin = PIN_PA10A_EIC_EXTINT10;
    wake_interrupt.gpio_pin_mux = PINMUX_PA10A_EIC_EXTINT10;
    wake_interrupt.gpio_pin_pull = EXTINT_PULL_NONE;
    wake_interrupt.wake_if_sleeping = true;

    //returnEic = _extint_get_eic_from_channel(WAKE_INTERRUPT_CHANNEL);
    

    extint_chan_set_config(WAKE_INTERRUPT_CHANNEL, &wake_interrupt);
	struct extint_events eventsList;
	for(uint8_t i = 0; i <= 31; i++){
	eventsList.generate_event_on_detect[i] = true;
	}
	extint_enable_events(&eventsList);

    extint_register_callback(wake_interrupt_callback,WAKE_INTERRUPT_CHANNEL,EXTINT_CALLBACK_TYPE_DETECT);
    extint_chan_enable_callback(WAKE_INTERRUPT_CHANNEL,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_clear_detected(WAKE_INTERRUPT_CHANNEL);
	*/
	eic_init();
	
}

bool checkWakeInterrupt(void) {
    if(wake_interrupt_triggered) {
        wake_interrupt_triggered = false;
        return true;
    } else {
        return false;
    }
}