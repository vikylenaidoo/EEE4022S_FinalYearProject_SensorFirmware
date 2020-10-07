#include "pololu_servo.h"



//---------------FUNCTIONS--------------//

Servo_Error_Typedef servo_set_target(int serial_port, uint8_t channel, unsigned int target){
    Servo_Error_Typedef e = SERVO_OK;

    if((target<=3280 && target>=992) || target==0){
        target *= 4;
        uint8_t message[4];

        //write message
        message[0] = 0x84; // Command byte: Set Target.
        message[1] = channel; // First data byte holds channel number.
        message[2] = target & 0x7F; // Second byte holds the lower 7 bits of target.
        message[3] = (target >> 7) & 0x7F;
        
        serial_write(serial_port, message, sizeof(message));
        
        //printf("%x %x %x %x\n", message[0], message[1], message[2], message[3]);
        e = servo_get_errors(serial_port);
    }
    else{
        e = SERVO_TARGET_BOUNDS_ERROR;
    }
    
    return e;

}

/**
 * 
*/
Servo_Error_Typedef servo_disable(int serial_port, uint8_t channel){
    return servo_set_target(serial_port, channel, 0);
}

/**
 * 
*/
Servo_Error_Typedef servo_get_errors(int serial_port){
    //send request for errors
    uint8_t message[] = {0xA1};
    serial_write(serial_port, message, sizeof(message));

    //read errors
    uint8_t error[2];
    int n = serial_read(serial_port, error, sizeof(error));

    //printf("%x %x\n", error[0], error[1]);

    //decipher error
    if(error[0] & 0b00000001)   
        return SERVO_SIG_ERROR;


    //no errors
    return SERVO_OK;
}


/**
 * 
*/
Servo_Error_Typedef servo_get_position(int serial_port, uint8_t channel, int* position){
    uint8_t message[2];

    //write message
    message[0] = 0x90; // Command byte: get position.
    message[1] = channel; // First data byte holds channel number.
    
    serial_write(serial_port, message, sizeof(message));

    //read position
    uint8_t pos[2];
    int n = serial_read(serial_port, pos, sizeof(pos));

    union u{
        uint8_t bytes[2];
        uint16_t value;
    } uu;
    uu.bytes[0] = pos[0];
    uu.bytes[1] = pos[1];
    *position = uu.value/4;

    //printf("%x %x\n", pos[0], pos[1]);
    printf("bytes: %d \n", n);

    return servo_get_errors(serial_port);
}