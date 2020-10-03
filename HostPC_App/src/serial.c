#include "serial.h"

//-----------------VARS-----------------//



//----------------------FUNCTIONS---------------------//

int serial_setup(){
    printf("---------------starting serial setup--------------\n");

    //get serial device from user
    printf("Please enter the serial device: eg. ttyUSB0\n");
    char input_serial_name[16];
    scanf("%s", input_serial_name);
    

    //open serial port
    char serial_name[32];
    strcpy(serial_name, "/dev/");
    strcat(serial_name, input_serial_name);
    
    //int *serial_port = &serial_port_1;

    /*if(serial_port_1>0){ //already in use
        serial_port = &serial_port_2;
    }*/

    int serial_port = open(serial_name, O_RDWR);

    //printf("-------serial port: %d\n", serial_port_1);

    //check for error
    if (serial_port < 0) {
        printf("The serial device %s is invalid, exiting program\n", input_serial_name);
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }
    

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    
    //configure tty control modes
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    //configure tty local modes
    tty.c_lflag &= ~ICANON; //disable canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    //configure tty input modes
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    //configure tty output modes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    //configure tty timeouts: no blocking, return immediately what is available
    tty.c_cc[VTIME] = 5;
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    
    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    printf("The serial device %s is successfully open and setup on port %d\n", serial_name, serial_port);
    return serial_port;
}

void serial_close(int sp){
    close(sp);
    printf("closed serial port %d\n", sp);

}

int serial_write(int serial_port, char data[], size_t length){
    return write(serial_port, data, length);

}

int serial_read(int serial_port, void *read_buff, size_t length){
    return read(serial_port, read_buff, length);
}