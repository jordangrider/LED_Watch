struct tc_module tc_instance;

void timeSlot_1ms(void);
void timeSlot_10ms(void);
void timeSlot_10ms_2(void);
void timeSlot_100ms(void);
void timeSlot_1000ms(void);

void enable_timer(void);
void disable_timer(void);
void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_method(struct tc_module *const module_inst);

void configure_tc(void) {
    struct tc_config config_tc;
    tc_get_config_defaults(&config_tc);
    config_tc.clock_source = GCLK_GENERATOR_0;
    config_tc.counter_size  = TC_COUNTER_SIZE_16BIT;
    config_tc.run_in_standby = false;
    config_tc.counter_16_bit.compare_capture_channel[0] = 0xBB80;
    tc_init(&tc_instance, TC3, &config_tc);
    enable_timer();
    //tc_register_callback(&tc_instance,tc_callback_method,TC_CALLBACK_CC_CHANNEL0);
    //tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
}

static uint16_t timeslotCounter = 0;
static bool slot_triggered = false;
void tc_callback_method(struct tc_module *const module_inst) {
    tc_set_count_value(module_inst, 0x0000);

    timeslotCounter++;
    if(timeslotCounter == 999) {
        timeslotCounter = 0;
    }
	
	slot_triggered = true;
}

void enable_timer(void) {
    tc_enable(&tc_instance);
    tc_register_callback(&tc_instance,tc_callback_method,TC_CALLBACK_CC_CHANNEL0);
    tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
    timeslotCounter = 0;
}

void disable_timer(void) {
    tc_disable(&tc_instance);
}