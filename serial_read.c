#include<fcntl.h>
#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<errno.h>

void main()
{
int fd;
fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY |O_NONBLOCK);

struct termios set;
tcgetattr(fd,&set);
cfsetispeed(&set,B115200);
cfsetospeed(&set,B115200);
set.c_cflag &= ~ PARENB; //Disables the Parity Enable bit which is PARENB , hence no p`arity
set.c_cflag &= ~ CSTOPB; //setting stop bit =1  , generally CSTOPB is all 0 s so complementing it will make it 1
set.c_cflag&=~CSIZE; // since we used tcgetattr before to clear the bit mask we complement and & the c_cflag with CSTOPB
set.c_cflag|=CS8; //setting the number of bits sent in one data frame as 8
set.c_cflag&=~CRTSCTS; //turning of hardware based flow control , it is used in events when software control is enabled or when the transmitting device and the recieving device are in the same baud rate
set.c_cflag|=CREAD | CLOCAL;// enabling read on serial port and telling it to gnore  modem based serial communicationlines 
set.c_iflag&=~(IXON | IXOFF | IXANY);//disbling software flow control
set.c_iflag&=~(ICANON | ECHO | ECHOE |ISIG);//ICANON puts the terminal in canonical mode where each charcter input is reflected on screen , ECHO AND ECHOE make sure that the characters are echoed on screen after typing or giving to input ,ISIG makes sure that all signals such as kill , quit etc or SIQUIT , SIGINT and all that stuff is still recieved by the terminal
set.c_cc[VMIN]=10; //read minimum of 10 characters
set.c_cc[VTIME]=1;//wait idefintely untill you recieve some characters

tcsetattr(fd,TCSANOW,&set);
char read_buf[40];
int read_b=0;
while(1)
{
//tcflush(fd, TCIFLUSH); //discards the old data recieved by the serial buffer
read_b = read(fd,read_buf,40);
if(read_b>0)
{
read_buf[read_b]='\0';
puts(read_buf);
}
}
//TCSANOW makes sure that the changes make in termios struct are reflected instantly
//
close(fd);
}
