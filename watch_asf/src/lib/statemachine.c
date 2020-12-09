
#define INTERRUPT_PIN 10

enum stateStatus {
    ENTER,
    ACTIVE,
    EXIT
};

void runStateMachine(void);

void sleepState(void);
void displayTimeState(void);
void demoState(void);
void wakeAnimation(void);

void switchStates(void (*stateToSwitch)());

volatile static void (*currentState)() = NULL;
volatile static enum stateStatus stateMachineStatus = ENTER;

void runStateMachine(void) {
    switch (stateMachineStatus) {
    case ENTER:
        //switchStates(displayTimeState);
        switchStates(demoState);
        stateMachineStatus = ACTIVE;
        break;
    case ACTIVE:
        currentState();
        break;
    }
}

void switchStates(void (*stateToSwitch)()) {
    currentState = stateToSwitch;
}

void displayTimeState(void) {
    static enum stateStatus localState = ENTER;
    static uint16_t countUp = 0;
    switch (localState) {
    case ENTER:
        countUp = 0;
        localState = ACTIVE;
        break;
    case ACTIVE:
        //displayTimeSimple();
		displayTimeHourMin();
        countUp++;
        if (countUp > 500) {
            turnOffAllLEDS();
            switchStates(sleepState);
            localState = ENTER;
        }
        break;
    case EXIT:
        break;
    }

}

void sleepState(void) {
    static enum stateStatus localState = ENTER;
    static uint8_t prepForSleep = 10;
	switch (localState) {
    case ENTER:
		prepForSleep = 10;
		accelerometer_sleep();
        localState = ACTIVE;
        break;
    case ACTIVE:
	    prepForSleep--;
            if(prepForSleep == 0){
            enterSleepMode();
            switchStates(wakeAnimation);
            localState = ENTER;
			accelerometer_wake();
			}
			
        break;
    }
}

void wakeAnimation(void) {
	static enum stateStatus localState = ENTER;
	switch (localState) {
		case ENTER:

		localState = ACTIVE;
		break;
		case ACTIVE:
		displayTimeHourMin();
		if(wipeTime(1)) {        //Condition for exiting this state
			localState = EXIT;   //Exit this state if condition is met
		}
		break;
		case EXIT:
		switchStates(displayTimeState);
		localState = ENTER;
		break;
	}
}

void demoState(void) {
    static enum stateStatus localState = ENTER;
    static uint16_t timer = 500;
    static uint8_t demoNumber = 0;
    switch (localState) {
    case ENTER:
        localState = ACTIVE;
        break;
    case ACTIVE:
        switch(demoNumber) {
        case 0:
            spinLEDs();
            break;
        case 1:
            ledFillEmpty();
            break;
        case 2:
            singleLEDphysics();
            break;
        case 3:
            displayTimeHourMin();
            break;
		case 4:
		    ledWave();
			break;
		case 5:
			sparkle();
			break;
        default:
            demoNumber = 0;
            break;
        }

        timer--;
        if(timer == 0) {
            timer = 500;
            turnOffAllLEDS();
            demoNumber++;
        }

        break;
    case EXIT:
        localState = ENTER;      //Make sure we start at the enter state when we return
        break;
    }
}

void testTapState(void) {
	static enum stateStatus localState = ENTER;
	switch (localState) {
		case ENTER:

		localState = ACTIVE;
		break;
		case ACTIVE:

		/*
		if(<condition>) {        //Condition for exiting this state
			localState = EXIT;   //Exit this state if condition is met
		}
		break;
		*/
		case EXIT:

		//switchStates(<state>);   //Which state should be switched to after exit
		localState = ENTER;      //Make sure we start at the enter state when we return
		break;
	}
}


/* State Template

void stateName(void) {
    static enum stateStatus localState = ENTER;
    switch (localState) {
    case ENTER:

        localState = ACTIVE;
        break;
    case ACTIVE:

        if(<condition>) {        //Condition for exiting this state
            localState = EXIT;   //Exit this state if condition is met
        }
        break;
    case EXIT:

        switchStates(<state>);   //Which state should be switched to after exit
        localState = ENTER;      //Make sure we start at the enter state when we return
        break;
    }
}

*/
