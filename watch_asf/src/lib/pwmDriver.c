struct tcc_module tcc_instance;
struct tcc_module tcc_instance1;

static void configure_tcc(void)
{
	struct tcc_config config_tcc;
	struct tcc_config config_tcc1;

	tcc_get_config_defaults(&config_tcc, TCC0);
	config_tcc.counter.period = 100;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	//config_tcc.compare.match[0] = 0xFFFF;
	config_tcc.compare.match[1] = 1*100/5;
	config_tcc.compare.match[2] = 4*100/5;
	config_tcc.compare.match[3] = 5*100/5;
	
	//COMPARE TO [3]
	config_tcc.pins.enable_wave_out_pin[7] = true;
	config_tcc.pins.wave_out_pin[7]        = PIN_PA17F_TCC0_WO7;
	config_tcc.pins.wave_out_pin_mux[7]    = MUX_PA17F_TCC0_WO7;
	
	//COMPARE TO [2]
	config_tcc.pins.enable_wave_out_pin[2] = true;
	config_tcc.pins.wave_out_pin[2]		   = PIN_PA18F_TCC0_WO2;
	config_tcc.pins.wave_out_pin_mux[2]	   = MUX_PA18F_TCC0_WO2;
	
	//COMPARE TO [1]
	config_tcc.pins.enable_wave_out_pin[5] = true;
	config_tcc.pins.wave_out_pin[5]		   = PIN_PA23F_TCC0_WO5;
	config_tcc.pins.wave_out_pin_mux[5]	   = MUX_PA23F_TCC0_WO5;
	
	tcc_init(&tcc_instance, TCC0, &config_tcc);

	tcc_get_config_defaults(&config_tcc, TCC1);
	config_tcc.counter.period = 100;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.compare.match[0] = 2*100/5;
	config_tcc.compare.match[1] = 3*100/5;
	//config_tcc.compare.match[2] = 4*100/5;
	//config_tcc.compare.match[3] = 5*100/5;
	
	//COMPARE TO [1]
	config_tcc.pins.enable_wave_out_pin[3] = true;
	config_tcc.pins.wave_out_pin[3]        = PIN_PA25F_TCC1_WO3;
	config_tcc.pins.wave_out_pin_mux[3]    = MUX_PA25F_TCC1_WO3;
	
	//COMPARE TO [0]
	config_tcc.pins.enable_wave_out_pin[2] = true;
	config_tcc.pins.wave_out_pin[2]        = PIN_PA24F_TCC1_WO2;
	config_tcc.pins.wave_out_pin_mux[2]    = MUX_PA24F_TCC1_WO2;
	
	
	tcc_init(&tcc_instance1, TCC1, &config_tcc);
	
	tcc_enable(&tcc_instance);
	tcc_enable(&tcc_instance1);
}

void *const tccAddress[] = {&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1, &tcc_instance, //0, 1 , 2 , 3 , 4
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance, //5
	&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance,  //10
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance, //15
	&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance,  //20
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance, //25
	&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance,  //30
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance, //35
	&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance,  //40
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance, //45
	&tcc_instance,&tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance,  //50
	&tcc_instance, &tcc_instance,&tcc_instance1,&tcc_instance1,&tcc_instance //55
};

uint8_t waveformAddress[] = {3,2,1,0,1,  //0, 1 , 2 , 3 , 4
	3,1,0,1,2,
	3,2,1,0,1,
	3,1,0,1,2,
	3,2,1,0,1,
	3,1,0,1,2,
	3,2,1,0,1,
	3,1,0,1,2,
	3,2,1,0,1,
	3,1,0,1,2,
	3,2,1,0,1,
	3,1,0,1,2
};

void setPWM(uint8_t ledNumber, uint8_t pwmPercentage)
{
	tcc_set_compare_value((struct tcc_module *const) tccAddress[ledNumber],waveformAddress[ledNumber],(101-pwmPercentage));
}