#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
  const char device[] = "/dev/tty.usbmodem14101";
  int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1) {
    printf( "failed to open port\n" );
  };
  if(!isatty(fd)) {
    printf( "not tty port\n" );
  }

  struct termios  config;

  //
  // Get the current configuration of the serial interface
  //
  if(tcgetattr(fd, &config) < 0) {
    printf( "could not get attributes\n" );
  }

  //
  // Input flags - Turn off input processing
  //
  // convert break to null byte, no CR to NL translation,
  // no NL to CR translation, don't mark parity errors or breaks
  // no input parity check, don't strip high bit off,
  // no XON/XOFF software flow control
  //
  //config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

  //
  // Output flags - Turn off output processing
  //
  // no CR to NL translation, no NL to CR-NL translation,
  // no NL to CR translation, no column 0 CR suppression,
  // no Ctrl-D suppression, no fill characters, no case mapping,
  // no local output processing
  //
  // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
  //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
  //config.c_oflag = 0;

  //
  // No line processing
  //
  // echo off, echo newline off, canonical mode off,
  // extended input processing off, signal chars off
  //
  //config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

  //
  // Turn off character processing
  //
  // clear current char size mask, no parity checking,
  // no output processing, force 8 bit input
  //
  //config.c_cflag &= ~(CSIZE | PARENB);
  //config.c_cflag |= CS8;

  //
  // One input byte is enough to return from read()
  // Inter-character timer off
  //
  //config.c_cc[VMIN]  = 1;
  //config.c_cc[VTIME] = 0;

  //
  // Communication speed (simple version, using the predefined
  // constants)
  //
  if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
    printf( "could not set speed to 9600\n" );
  }

  //
  // Finally, apply the configuration
  //
  if(tcsetattr(fd, TCSAFLUSH, &config) < 0) {
    printf( "could not set attributes\n" );
  }


  uint8_t c=0x00;
  while (c!=0x3E)
  {
    if (read(fd,&c,1)>0)
      write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
  }

  printf("sleep\n");
  sleep(1);
  c=0x75;
  write(fd,&c,1);
  c=0x0D;
  write(fd,&c,1);

  while (c!=0x3E)
  {
    if (read(fd,&c,1)>0)
      write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
    c=0x0D;
    write(fd, &c, 1);
  }

  close(fd);
}
