struct rtc_calendar_config rtc_config_object;
struct rtc_module rtc_object;

//#  define RTC_CLOCK_SOURCE    RTC_CLOCK_SELECTION_XOSC32K

void getRTCTime(void);

volatile struct rtc_calendar_time currentTime;
volatile uint32_t twelveHourTime = 0;

void configure_rtc(void){
	rtc_calendar_get_config_defaults(&rtc_config_object);
	//rtc_config_object.mode = 2;
	rtc_calendar_init(&rtc_object, RTC, &rtc_config_object);
	rtc_calendar_enable(&rtc_object);
	
	currentTime.hour = 9;
	currentTime.pm = 0;
	currentTime.minute = 32;
	currentTime.second = 49;
	rtc_calendar_set_time(&rtc_object, &currentTime);
	//rtc_calendar_get_time(&rtc_object , &currentTime);
	getRTCTime();
}

void getRTCTime(void){
	rtc_calendar_get_time(&rtc_object , &currentTime);
	twelveHourTime = currentTime.hour * 3600 + currentTime.minute * 60 + currentTime.second;
}

void updateTime(void){
	twelveHourTime++;
}

uint32_t getTime(void){
	return twelveHourTime;
}