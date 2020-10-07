
//--------------------INCLUDES-------------------------//
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

//------------------------DEFINES & VARS--------------------------//



//-----------------------PUBLIC FUNCTIONS----------------//
int serial_setup();

void serial_close();

int serial_write(int serial_port, void *data, size_t length);

int serial_read(int serial_port, void *read_buff, size_t length);
