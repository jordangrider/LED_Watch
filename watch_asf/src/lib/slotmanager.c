void slotManager(void){
	if(slot_triggered){
    timeSlot_1ms();
    if(timeslotCounter % 10 == 0) {
        timeSlot_10ms();
    }
    if(timeslotCounter % 10 == 5) {
        timeSlot_10ms_2();
    }
    if(timeslotCounter % 100 == 0) {
        timeSlot_100ms();
    }
    if(timeslotCounter % 1000 == 0) {
        timeSlot_1000ms();
    }
	slot_triggered = false;
	}
}