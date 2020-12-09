

void singleLEDphysics() {

    static float position = 0;
    static uint8_t currentPosition = 0;
    static uint8_t lastPosition = 0;

    struct ACCELEROMETER_VALUES_TYPE accelValues = getAccelerometerValues();
    //volatile float locationRadians = atan(accelValues.y/accelValues.x);
    //int output = (uint8_t)(60*((locationRadians+(M_PI/2.0))/(M_PI)));

    float force = 0.1;

    force = cos(2*M_PI*position/60.0)*accelValues.y + sin(2*M_PI*position/60.0)*accelValues.x;

    static float velocity = 0;
    velocity = 0.85*velocity + force;
    float output = 100*velocity + position;

    position = (0.95)*position + (0.05)*output;
    if(position > 60) {
        position = position - 60;
    }
    if(position < 0) {
        position = position + 60;
    }
    if(position > currentPosition + 0.5 || position < currentPosition - 0.5) {
        currentPosition = (uint8_t)position;
    }
    //currentPosition = (uint8_t)position;
    if(currentPosition != lastPosition) {
        turnOffAllLEDS();
        //switchLED(lastPosition, true);
        switchLED(currentPosition, 10);
        for(uint8_t i = 1; i <= (uint8_t)abs(velocity*5); i++) {
            if(velocity > 0) {
                switchLED(currentPosition+i, 0);
            } else {
                switchLED(currentPosition-i, 10);
            }
        }
        lastPosition = currentPosition;
    }
}

void spinLEDs(void) {
    static uint8_t counter = 0;
    static uint8_t ledCount = 0;
    counter += 1;
    if(counter == 10) {
        switchLED(ledCount, 0);
        ledCount++;
        if(ledCount == 60) {
            ledCount = 0;
        }
        switchLED(ledCount, 10);
        counter = 0;
    }
}

void ledFillEmpty(void) {
    static uint8_t splitTime = 0;

    splitTime++;
    if (splitTime == 2) {
        splitTime = 0;
        static uint8_t LEDIncrement = 0;

        if(LEDIncrement < 60) {
            switchLED(LEDIncrement, 10);
        } else {
            switchLED(120-LEDIncrement, 0);
        }

        LEDIncrement++;

        if(LEDIncrement == 121) {
            LEDIncrement = 0;
            turnOffAllLEDS();
        }
    }
}

void drawPoint(uint8_t location, uint8_t width, uint8_t brightness){
	uint8_t forward = location;
	uint8_t reverse = location;
	for(uint8_t i = 0; i < width; i++){
		
		addLED(forward, (uint8_t)(brightness - (brightness*i)/width));
		if(i != 0){
		addLED(reverse, (uint8_t)(brightness - (brightness*i)/width));
		}
        
		if(forward == 59){
	    forward = 0;
		} else {
		forward++;	
		}
	    if(reverse == 0){
		reverse = 59;
		} else {
	    reverse--;
		}
		
	}
}

void ledWave(void) {
    static uint8_t count = 0;
	static uint8_t count2 = 0;
	static uint8_t count3 = 0;

    turnOffAllLEDS();
    drawPoint(count, 6, 10);
	count ++;
	if (count == 60){
		count = 0;
	}
	
	if (count%3 == 1){
	count3 ++;
	if (count3 == 60){
		count3 = 0;
	}
		
	}
	drawPoint(count3, 15, 10);
	
	if (count%2 == 1){
	if (count2 == 0){
		count2 = 60;
	}
	count2 --;	
	}
	drawPoint(count2, 6, 5);
}

void ledHeartbeat(uint8_t ledToBlink) {
    static bool ledOn = false;
    if (ledOn) {
        switchLED(ledToBlink,true);
        ledOn = false;
    } else {
        switchLED(ledToBlink,false);
        ledOn = true;
    }
}

bool spinLEDsWide(uint8_t speed) {
	const uint8_t ledTrailLength = 3;
	static uint8_t ledCount = 0;
	turnOffAllLEDS();
	if(ledCount < 60){
		uint8_t ledToLight = ledCount;
		for(uint8_t i = 0; i <= ledTrailLength; i++){
			switchLED(ledToLight, true);
			if(ledToLight == 0){
				break;
			}
			ledToLight--;
		}
		ledCount = ledCount + speed;
		return false;
		} else {
		ledCount = 0;
		return true;
	}
}

bool wipeTime(uint8_t speed) {
	const uint8_t ledTrailLength = 1;
	static uint8_t ledCount = 0;
	static uint8_t lastLED = 0;
	static uint8_t firstLED = 61;
	
	if(ledCount == 0){
		for(uint8_t i = 0; i < 60; i++){
			if (isLEDOn(i)){
				lastLED = i;
				if(firstLED > 60){
					firstLED = i;
				}
			}
		}
		ledCount = firstLED;
	}
	
	for(uint8_t i = 0; i < 60; i++){
		if(i > ledCount){
			switchLED(i,false);
		}
	}
	
	if(ledCount < lastLED){
		switchLED(ledCount, true);
		ledCount = ledCount + speed;
		return false;
		} else {
		ledCount = 0;
		lastLED = 0;
		firstLED = 61;
		return true;
	}
}

void transitionToSettingTime(void) {

}