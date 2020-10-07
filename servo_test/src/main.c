#include "main.h"


int main(){
    printf("------------------1st serial device--------------\n");
    int serial_port_1 = serial_setup();
    if(serial_port_1 < 0){
        return 1;
    }

    
    printf("p1: %d \n", serial_port_1);
    


    isActive=1;
    
    while(isActive){
        printf("Enter function: \n t=set_target, e=get_error, d=disable, x=exit \n");
        char cmd;
        scanf("%c", &cmd);


        if(cmd =='t'){
            printf("Enter target width 992-3280 (us) \n");
            int target;
            scanf("%d", &target);
            
            Servo_Error_Typedef e = servo_set_target(serial_port_1, 0, target);
            if(e){
                printf("error %d \n", e);
            }
        }
        else{
            if(cmd == 'e'){
                Servo_Error_Typedef e = servo_get_errors(serial_port_1);
                printf("error %d \n", e);
            }
            else{
                if(cmd == 'x'){
                    isActive=0;
                }
                else{
                    if(cmd=='d'){
                        servo_disable(serial_port_1, 0);
                    }
                    else{
                        if(cmd=='p'){
                            int p = 0;
                            servo_get_position(serial_port_1, 0, &p);
                            printf("position: %d \n", p);
                        }
                    }
                    //printf("invalid cmd, try again \n");
                }
            }
        }

    }

    serial_close(serial_port_1);
    //serial_close(serial_port_2);
    return 0;
}


