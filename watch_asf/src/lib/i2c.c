#define DATA_LENGTH 2
static uint8_t wr_buffer[DATA_LENGTH] = {0x00, 0x00};
static uint8_t writeByte[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static uint8_t whoAmILocation[1] = {0xA8};
volatile static uint8_t rd_buffer[6] = {0x00,0x00,0x00,0x00,0x00,0x00};

void write_i2c_byte(uint8_t, uint8_t);
void read_i2c_packet(void);

#define SLAVE_WRITE_ADDRESS 0x19
#define SLAVE_READ_ADDRESS 0x19

void requestAccerometerReading(void);

/*
const uint8_t accelerometer_registers_to_configure[][2] = {{0x20,0b01011111},  //enabling reading
	                                                       {0X22,0b10000000},   //CTRL_REG3 AOI1 interrupt
														   //{0x25,0b10000000},
														   {0x24,0b00001000},   //Enable IN1 Latch
														   //{0x38,0b00010000},  //Enable Click interrupt on z
														   //{0x39,0b00010100},  //Enable Click interrupt
														   //{0x22,0b10000000},  //Enable Click interrupt on INT1

														   //{0x30,0b00000000},   //Config Y high interrupt
														   //{0x32,0b00010000},   //Sensitivity
														   //{0x33,0b00010001},   //Time INT1 duration
														   {0x38,0b00101010},   //Click config for 3 axis
														   //{0x39,0b00100000},   //Click config 2 src
														   {0x3A,0b01010000},   //Click threshold
														   {0x3B,0b00001010},   //Click time limit
														   {0x3C,0b00010100},   //Click time latency
														   {0x3D,0b11111111}   //Click time window

														   };
*/ //Working config for tap setup

//Attempt at wake
const uint8_t accelerometer_registers_to_configure[][2] = {{0x20,0b01011111},    //CTRL_REG1: enabling reading
    {0X22,0b01000000},    //CTRL_REG3: AOI1 interrupt
    {0x24,0b00000000},    //CTRL_REG5: Enable IN1 Latch
    {0x25,0b00001000},    //CTRL_REG6:
    {0x30,0b01100000},    //INT1_CFG
    //{0x31,0b11000000},  //INT1_SRC
    {0x32,0b00000110},    //INT1_THS: Sensitivity
    {0x33,0b00000001},    //INT1_DURATION Time INT1 duration
    //{0x33,0b00110000},    //INT1_DURATION Time INT1 duration
    //{0x38,0b00000100},  //Click config for Z axis
    //{0x39,0b00011000},  //Click config 2 src
    //{0x3A,0b01110000},  //Click threshold
    //{0x3B,0b00001000},  //Click time limit
    //{0x3C,0b00000100},  //Click time latency
    //{0x3D,0b00000100}   //Click time window

};

struct i2c_master_module i2c_master_instance;
struct i2c_master_packet wr_packet;
struct i2c_master_packet rd_packet;
struct i2c_master_packet write_byte_packet;

struct ACCELEROMETER_VALUES_TYPE {
    float x;
    float y;
    float z;
};

void write_i2c_byte(uint8_t location, uint8_t byte);
void write_i2c_packet(void);
float convertToGs(uint8_t input);
struct ACCELEROMETER_VALUES_TYPE getAccelerometerValues(void);

static bool accelerometerIsInitialized = false;

struct ACCELEROMETER_VALUES_TYPE getAccelerometerValues(void) {
    struct ACCELEROMETER_VALUES_TYPE output;
    output.x = convertToGs(rd_buffer[1]);
    output.y = convertToGs(rd_buffer[3]);
    output.z = convertToGs(rd_buffer[5]);
    return output;
}

float convertToGs(uint8_t input) {
    float output = 0;
    if(input > 127) {
        output = (float)(input-256)/64.0;
    } else {
        output = (float)(input)/64.0;
    }
    return output;
}

void accelerometerLoop(void) {
    static uint8_t setupRegisterToWrite = 0;
    static uint8_t initialCountdown = 100;
    if(initialCountdown > 0) {
        initialCountdown--;
    }
    if(!accelerometerIsInitialized && initialCountdown == 0) {
        if(setupRegisterToWrite >= (sizeof(accelerometer_registers_to_configure)/2)) {
            accelerometerIsInitialized = true;
        } else {
            write_i2c_byte(accelerometer_registers_to_configure[setupRegisterToWrite][0],accelerometer_registers_to_configure[setupRegisterToWrite][1]);
            setupRegisterToWrite++;
        }

    }
    requestAccerometerReading();
}

bool isAccelerometerInitialized(void) {
    return 	accelerometerIsInitialized;
}

void requestAccerometerReading(void) {
    if(isAccelerometerInitialized()) {
        write_i2c_packet();
    }
}

void configure_i2c(void) {
    /* Initialize config structure and software module */
    struct i2c_master_config config_i2c_master;
    i2c_master_get_config_defaults(&config_i2c_master);
    /* Change buffer timeout to something longer */
    //config_i2c_master.start_hold_time = I2C_MASTER_START_HOLD_TIME_50NS_100NS;

    config_i2c_master.buffer_timeout = 65535;
    config_i2c_master.pinmux_pad0    = SERCOM2_PAD0_DEFAULT;
    config_i2c_master.pinmux_pad1    = SERCOM2_PAD1_DEFAULT;
    config_i2c_master.generator_source = GCLK_GENERATOR_3;
    //config_i2c_master.baud_rate = 50;
    /* Initialize and enable device with config */
    while(i2c_master_init(&i2c_master_instance, SERCOM2, &config_i2c_master)     \
            != STATUS_OK);
    i2c_master_enable(&i2c_master_instance);
    write_i2c_byte(0x24,0b10000000);
    read_i2c_packet();
}

void accelerometer_sleep(void){
	write_i2c_byte(0x20,0b00101111);
}

void accelerometer_wake(void){
	write_i2c_byte(0x20,0b01011111);
}

void i2c_write_complete_callback(
    struct i2c_master_module *const module) {
    /* Initiate new packet read */

    i2c_master_read_packet_job(&i2c_master_instance,&rd_packet);
}

void i2c_read_complete_callback(struct i2c_master_module *const module) {

}

void configure_i2c_callbacks(void) {
    /* Register callback function. */
    i2c_master_register_callback(&i2c_master_instance, i2c_write_complete_callback,
                                 I2C_MASTER_CALLBACK_WRITE_COMPLETE);
    i2c_master_register_callback(&i2c_master_instance, i2c_read_complete_callback,
                                 I2C_MASTER_CALLBACK_READ_COMPLETE);
    i2c_master_enable_callback(&i2c_master_instance,
                               I2C_MASTER_CALLBACK_WRITE_COMPLETE);
    i2c_master_enable_callback(&i2c_master_instance,
                               I2C_MASTER_CALLBACK_READ_COMPLETE);
}

void write_i2c_packet(void) {
    wr_packet.address = SLAVE_WRITE_ADDRESS;
    wr_packet.data_length = 1;
    wr_packet.data = &whoAmILocation[0];
    volatile enum status_code write_result = i2c_master_write_packet_job_no_stop(&i2c_master_instance, &wr_packet);
    //addressToWrite++;
}



void write_i2c_byte(uint8_t location, uint8_t byte) {
    wr_packet.address = SLAVE_WRITE_ADDRESS;
    wr_packet.data_length = 2;
    wr_buffer[0] = location;
    wr_buffer[1] = byte;
    wr_packet.data = &wr_buffer[0];
    volatile enum status_code write_result = i2c_master_write_packet_job(&i2c_master_instance, &wr_packet);
}

void read_i2c_packet(void) {
    rd_packet.address = SLAVE_READ_ADDRESS;
    rd_packet.data_length = 6;
    rd_packet.data = &rd_buffer[0];
    //volatile enum status_code read_result = i2c_master_read_packet_job(&i2c_master_instance, &rd_packet);
}

void readAccelerometer(uint8_t location, uint8_t numberOfBytes) {
    wr_packet.address = SLAVE_WRITE_ADDRESS;
    wr_packet.data_length = 1;
    wr_packet.data = &whoAmILocation[0];
    volatile enum status_code write_result = i2c_master_write_packet_job_no_stop(&i2c_master_instance, &wr_packet);
}