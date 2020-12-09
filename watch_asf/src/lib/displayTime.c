#define TWELVEHOURS 43200
#define ONEHOUR 3600
#define ONEMINUTE 60

void displayTimeSimple(void){
	uint32_t timeVal = getTime();
	timeVal = timeVal % TWELVEHOURS;
	uint8_t hours = (5*timeVal)/ONEHOUR;
	uint8_t mins = (timeVal % ONEHOUR)/60;
	uint8_t seconds = timeVal % ONEMINUTE;
	turnOffAllLEDS();	
	switchLED(hours, 10);
	switchLED(mins, 10);
	switchLED(seconds, 10);
	
}

void displayTimeHourMin(void){
	
	
	static uint8_t oneSecond = 0;
	uint32_t timeVal = getTime();
	timeVal = timeVal % TWELVEHOURS;
	uint8_t hours = (5*timeVal)/ONEHOUR;
	uint8_t mins = (timeVal % ONEHOUR)/60;
	uint8_t seconds = timeVal % ONEMINUTE;
	turnOffAllLEDS();
	
	uint8_t hour_high = 30;
	uint8_t hour_low = 10;
	uint8_t minute_high = 20;
	uint8_t second_high = 9;
	
	if(hours == 0){
		switchLED(59, hour_high);
		} else {
		switchLED(hours-1, hour_high);
	}
	switchLED(hours, hour_low);
	if(hours == 59){
		switchLED(0, hour_high);
		} else {
		switchLED(hours+1, hour_high);
	}
	
	if(isLEDOn(mins)){
		switchLED(mins, 0);
		} else {
		switchLED(mins, minute_high);
	}
	

	if(isLEDOn(seconds)){
		switchLED(seconds, 0);
		} else {
		switchLED(seconds, second_high);
	}
	
	
	

	oneSecond++;
	if(oneSecond >= 100){
		oneSecond = 0;
	}
}

void displayTimeAlternate_one(void){
	
	static timeVal = 12345;
	static uint8_t oneSecond = 0;
	
	bool use_real_time = true;
	
	if(use_real_time){
	timeVal = getTime();	
	} else {
	timeVal = timeVal + 1;
	}
	
	timeVal = timeVal % TWELVEHOURS;
	uint8_t hours = (5*timeVal)/ONEHOUR;
	uint8_t mins = (timeVal % ONEHOUR)/60;
	uint8_t seconds = timeVal % ONEMINUTE;
	turnOffAllLEDS();
	
	uint8_t hour_high = 0;
	uint8_t hour_low = 3;
	uint8_t minute_high = 40;
	uint8_t minute_low = 7;
	uint8_t second_high = 5;
	
	uint8_t background_high = 7;
	uint8_t background_low = 0;
	
	bool enable_hour_outline = false;
	
	for(uint8_t i = 0; i < 60; i++){
		if(i % 5 == 0){
			switchLED(i, background_high);
		} else {
			switchLED(i, background_low);
		}
	}
	
	for(uint8_t i = 0; i < 5; i++){
		if(i == 0){
		switchLED(hours*5 + i, hour_high);	
		} else {
		switchLED((hours/5)*5 + i, hour_low);
		}
	}
	//switchLED(hours , hour_high);	
	
	/*
	if(mins == 59){
		addLED(0, minute_low);
		} else {
		addLED(mins+1, minute_low);
	}
	if(mins == 0){
		addLED(59, minute_low);
		} else {
		addLED(mins-1, minute_low);
	}
	*/
	
	addLED(mins, minute_high);
	
	
	

	if(isLEDOn(seconds)){
		switchLED(seconds, 0);
		} else {
		addLED(seconds, second_high);
	}
	
	
	

	oneSecond++;
	if(oneSecond >= 100){
		oneSecond = 0;
	}
}