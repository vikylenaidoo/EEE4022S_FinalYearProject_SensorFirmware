/**
 * default pwm frequency:   f=50Hz ==> T=20ms
 * min width:   992 us
 * max width:   3280 us
 * 
 *  
*/

#include <stdint.h>
#include "serial.h"


//---------------DEFINES--------------//

typedef enum{ //see pg 39 of pololu user guide for details
    SERVO_OK=0,
    SERVO_SIG_ERROR,
    SERVO_OVERRUN_ERROR,
    SERVO_BUF_FULL_ERROR,
    SERVO_CRC_ERROR,
    SERVO_PROTOCOL_ERROR,
    SERVO_TIMEOUT_ERROR,
    SERVO_TARGET_BOUNDS_ERROR

}Servo_Error_Typedef;


//---------------FUNCTIONS--------------//

/** 
 * set the target pulse width on selected channel
 * channel: 0 - 5
 * target:  992 - 3280 (us)
 * 
*/
Servo_Error_Typedef servo_set_target(int serial_port, uint8_t channel, unsigned int target);

Servo_Error_Typedef servo_disable(int serial_port, uint8_t channel);

Servo_Error_Typedef servo_set_speed();

Servo_Error_Typedef servo_set_acceleration();

Servo_Error_Typedef servo_get_position(int serial_port, uint8_t channel, int* position);

Servo_Error_Typedef servo_get_errors(int serial_port);