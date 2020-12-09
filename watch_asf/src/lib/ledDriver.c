#define NUMBER_OF_LEDS 60
#define NUMBER_OF_SEGMENTS 12
#define LEDS_PER_SEGMENT 5

void driveLEDs(void);
void setLEDPinsAsOutputs(void);
void spinLEDs();

void turnOffLED(uint8_t);
void turnOnLED(uint8_t);
void switchLED(uint8_t, uint8_t);
void addLED(uint8_t, uint8_t);
void turnOnAllLEDS(void);
void ledFillEmpty(void);
void ledHeartbeat(uint8_t);


const uint8_t sourcePins[] = {16,15,14,11,7,6,5,4,28,27,22,19};
const uint8_t sinkPins[] = {18,25,24,23,17};


// {source pin, sink pin}
// PWM will occur on the source pin
// Sink pin will be grounded when activated and held high when inactive
const uint8_t pinSet[][2] = {{16,17},{16,18},{16,25},{16,24},{16,23}, //0, 1 , 2 , 3 , 4
    {15,17},{15,23},{15,24},{15,25},{15,18}, //5
    {14,17},{14,18},{14,25},{14,24},{14,23}, //10
    {11,17},{11,23},{11,24},{11,25},{11,18}, //15
    {7,17},{7,18},{7,25},{7,24},{7,23}, //20
    {6,17},{6,23},{6,24},{6,25},{6,18}, //25
    {5,17},{5,18},{5,25},{5,24},{5,23}, //30
    {4,17},{4,23},{4,24},{4,25},{4,18}, //35
    {28,17},{28,18},{28,25},{28,24},{28,23}, //40
    {27,17},{27,23},{27,24},{27,25},{27,18}, //45
    {22,17},{22,18},{22,25},{22,24},{22,23}, //50
    {19,17},{19,23},{19,24},{19,25},{19,18} //55
};

bool LEDbinary[60];
uint8_t LEDdim[60];
uint8_t minSegmentsOn = 3;

void setMinSegmentsOn(uint8_t segmentCount) {
    minSegmentsOn = segmentCount;
}

void driveLEDs(void) {
    volatile bool ledInSegmentOn = false;
    static uint8_t sourceValue = 0;
    static uint8_t previousSourceValue = 0;
    volatile static uint8_t segmentOnCount = 0;

    while (!ledInSegmentOn) {

		ioport_set_pin_level(sourcePins[previousSourceValue], 0);
		
        for(uint8_t i = 0; i < LEDS_PER_SEGMENT; i++) {
            uint8_t ledOfInterest = sourceValue*LEDS_PER_SEGMENT+i;
            if(LEDdim[ledOfInterest] != 0 && ledOfInterest < NUMBER_OF_LEDS) {
                ledInSegmentOn = true;
            }
        }

        if(ledInSegmentOn) {
            previousSourceValue = sourceValue;
            for(uint8_t i = 0; i < LEDS_PER_SEGMENT; i++) {
                uint8_t ledOfInterest = sourceValue*LEDS_PER_SEGMENT+i;
                if(LEDdim[ledOfInterest] == 0) {
                    //ioport_set_pin_level(pinSet[ledOfInterest][1], 0);
					setPWM(ledOfInterest,0);
					setPWM(ledOfInterest,0);
                } else {
                    //ioport_set_pin_level(pinSet[ledOfInterest][1], 1);
					setPWM(ledOfInterest,LEDdim[ledOfInterest]);
					setPWM(ledOfInterest,LEDdim[ledOfInterest]);
                }
            }
			segmentOnCount++;
            ioport_set_pin_level(sourcePins[sourceValue], 1);
        }
		
		int8_t segmentsRemaining =  NUMBER_OF_SEGMENTS - ((int8_t)sourceValue+1);
        if(segmentsRemaining <= (int8_t)minSegmentsOn - (int8_t)segmentOnCount) {
            ledInSegmentOn = true;
            segmentOnCount++;
        }

        sourceValue++;
        if(sourceValue == NUMBER_OF_SEGMENTS) {
            sourceValue = 0;
            ledInSegmentOn = true;
            /*
			if(segmentOnCount == 0) {
                for(uint8_t i = 0; i < 11; i++) {
                    ioport_set_pin_level(sourcePins[i], 0);
                }
            }
			*/
            segmentOnCount = 0;
        }
    }
}

/*void driveLEDs(void) {
	bool ledInSegmentOn = false;
	static uint8_t sourceValue = 0;
	
	while (!ledInSegmentOn) {
		
		uint8_t previousSourceValue = 0;
		
		if(sourceValue == 0){
			previousSourceValue = 11;
			} else {
			previousSourceValue = sourceValue - 1;
		}
		
		for(uint8_t i = 0; i < 5; i++){
			ioport_set_pin_level(sinkPins[i], 1);
		}
		
		ioport_set_pin_level(sourcePins[previousSourceValue], 0);
		
		
		for(uint8_t i = 0; i < 5; i++){
			volatile uint8_t ledOfInterest = sourceValue*5+i;
			
			if(LEDdim[ledOfInterest]>0){
				//ioport_set_pin_level(pinSet[sourceValue*5+i][1], 0);
				setPWM(ledOfInterest,LEDdim[ledOfInterest]);
				ledInSegmentOn = true;
				} else {
				
				setPWM(ledOfInterest,0);
				//ioport_set_pin_level(pinSet[sourceValue*5+i][1], 1);
				ledInSegmentOn = true;

			}
		}
		ioport_set_pin_level(sourcePins[sourceValue], 1);
		
		sourceValue++;
		if(sourceValue == 12){
			sourceValue = 0;
			break;
		}
	}
}
*/

void switchLED(uint8_t led, uint8_t pwm){
	if(led < 60 && led >= 0){
		LEDdim[led] = pwm;
	}
}

void addLED(uint8_t led, uint8_t pwm){
	if(led < 60 && led >= 0){
		if(LEDdim[led] + pwm > 100){
			LEDdim[led] = 100;
		} else {
		LEDdim[led] = LEDdim[led] + pwm;
		}
	}
}

void turnOffAllLEDS(void){
	for(uint8_t i = 0; i <= NUMBER_OF_LEDS; i++){
		//turnOffLED(i);
		switchLED(i,0);
	}
}

void turnOnAllLEDS(void){
	for(uint8_t i = 0; i <= NUMBER_OF_LEDS; i++){
		//turnOffLED(i);
		switchLED(i,10);
	}
}

/*void driveLEDs(void) {
    volatile bool ledInSegmentOn = false;
    static uint8_t sourceValue = 0;
    static uint8_t previousSourceValue = 0;
    volatile static uint8_t segmentOnCount = 0;

    while (!ledInSegmentOn) {
        for(uint8_t i = 0; i < LEDS_PER_SEGMENT; i++) {
            volatile uint8_t ledOfInterest = sourceValue*LEDS_PER_SEGMENT+i;
            if(LEDbinary[ledOfInterest] && ledOfInterest < NUMBER_OF_LEDS) {
                ledInSegmentOn = true;
            }
        }
        if(ledInSegmentOn) {
            segmentOnCount++;
        }

        int8_t segmentsRemaining =  NUMBER_OF_SEGMENTS - ((int8_t)sourceValue+1);
        if(segmentsRemaining <= (int8_t)minSegmentsOn - (int8_t)segmentOnCount) {
            ledInSegmentOn = true;
            segmentOnCount++;
        }

        if(ledInSegmentOn) {
            ioport_set_pin_level(sourcePins[previousSourceValue], 0);
            previousSourceValue = sourceValue;
            for(uint8_t i = 0; i < LEDS_PER_SEGMENT; i++) {
                uint8_t ledOfInterest = sourceValue*LEDS_PER_SEGMENT+i;
                if(LEDbinary[ledOfInterest]) {
                    ioport_set_pin_level(pinSet[ledOfInterest][1], 0);
                } else {
                    ioport_set_pin_level(pinSet[ledOfInterest][1], 1);
                }
            }
            ioport_set_pin_level(sourcePins[sourceValue], 1);
        }

        sourceValue++;
        if(sourceValue == NUMBER_OF_SEGMENTS) {
            sourceValue = 0;
            ledInSegmentOn = true;
            if(segmentOnCount == 0) {
                for(uint8_t i = 0; i < 11; i++) {
                    ioport_set_pin_level(sourcePins[i], 0);
                }
            }
            segmentOnCount = 0;
        }
    }
}

void switchLED(uint8_t led, bool status) {
    if(led < 60 && led >= 0) {
        LEDbinary[led] = status;
    }
}

void turnOffAllLEDS(void) {
    for(uint8_t i = 0; i <= NUMBER_OF_LEDS; i++) {
        //turnOffLED(i);
        switchLED(i,false);
    }
}



void turnOnAllLEDS(void) {
    for(uint8_t i = 0; i <= NUMBER_OF_LEDS; i++) {
        //turnOffLED(i);
        switchLED(i,true);
    }
}*/

bool isLEDOn(uint8_t led){
	return LEDbinary[led];
}

void prepareLEDsForSleep(void) {
    for(uint8_t i = 0; i <= NUMBER_OF_LEDS; i++) {
        turnOffLED(i);
    }
}

void turnOffLED(uint8_t ledNumber) {
    ioport_set_pin_level(pinSet[ledNumber][0], 0);
    ioport_set_pin_level(pinSet[ledNumber][1], 1);
}

void turnOnLED(uint8_t ledNumber) {
    ioport_set_pin_level(pinSet[ledNumber][0], 1);
    ioport_set_pin_level(pinSet[ledNumber][1], 0);
}

void setLEDPinsAsOutputs(void) {
    for(uint8_t i = 0; i <= sizeof(sourcePins); i++) {
        ioport_set_pin_dir(sourcePins[i], IOPORT_DIR_OUTPUT);
        ioport_set_pin_level(sourcePins[i], 0);
    }
    for(uint8_t i = 0; i <= sizeof(sinkPins); i++) {
        ioport_set_pin_dir(sinkPins[i], IOPORT_DIR_OUTPUT);
        ioport_set_pin_level(sinkPins[i], 1);
    }
}

void setLEDPinsAsInputs(void) {
    for(uint8_t i = 0; i <= sizeof(sourcePins); i++) {
        ioport_set_pin_dir(sourcePins[i], IOPORT_DIR_INPUT);
    }
    for(uint8_t i = 0; i <= sizeof(sinkPins); i++) {
        ioport_set_pin_dir(sinkPins[i], IOPORT_DIR_INPUT);
    }
}

void setAllPinsAsInputs(void) {
    for(uint8_t i = 2; i <= 32; i++) {
        ioport_set_pin_dir(i, IOPORT_DIR_INPUT);
    }
}