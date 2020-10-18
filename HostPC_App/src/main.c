#include "main.h"


int main(){
    printf("------------------1st serial device--------------\n");
    int serial_port_1 = serial_setup();
    if(serial_port_1 < 0){
        return 1;
    }

    /*
    printf("------------------2nd serial device--------------\n");
    int serial_port_2 = serial_setup();
    if(serial_port_2 < 0){
        serial_close(serial_port_1);
        return 1;
    }
    */

    printf("p1: %d \n", serial_port_1);
    //printf("p2: %d \n", serial_port_2);


    isActive=1;
    while(isActive){
 /*       printf("Enter message to send, type quit to exit\n");
        char message[64];
        scanf("%s", message);

        if(strcmp("quit", message) == 0){
            isActive=0;
            break;
        }
 */   
        //printf("length of %s is %d \n", message, (int)strlen(message));
            
/*
        //write message
        char message[] = "test"; //{'a', 'a'};
        //int w = serial_write(serial_port_1, message, 2);
        
        int w = serial_write(serial_port_1, message, strlen(message));
*/
        
        //printf("%s\n", message);

        
        //read message
        char read_buffer[64];
        memset(&read_buffer, '\0', sizeof(read_buffer));

        //int n = read(serial_port_2, &read_buffer, sizeof(read_buffer));
        int n = serial_read(serial_port_1, read_buffer, sizeof(read_buffer));
        if(n){
            printf("data read %d bytes from serial:\n",n);
            for(int i=0; i<n; i++){
                printf("%x / ", read_buffer[i]);
            }
            printf("\n");
        }
      
        usleep(100000);        
    }

    serial_close(serial_port_1);
    //serial_close(serial_port_2);
    return 0;
}


