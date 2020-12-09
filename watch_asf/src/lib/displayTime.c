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
	
	uint8_t hour_high = 60;
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