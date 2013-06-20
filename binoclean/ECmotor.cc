#include <stdio.h>
#include "ECmotor.h"

// default constructor
ECmotor::ECmotor()
{
}

// class constructor
ECmotor::ECmotor(int port)
{
	m_motorPort = port;
}

ECmotor::~ECmotor()
{
	char str[32];
}

void ECmotor::port(int p)
{
	m_motorPort = p;
}

void ECmotor::connect()
{
    
	struct termios termios_p;
	long speed;
	char str[32];
	char istr[32];
	tcgetattr(m_motorPort, &termios_p);

	/* setup serial line */
	speed = BAUD_RATE;					/* 9600 BAUD rate */
	cfsetospeed(&termios_p, speed);
	cfsetispeed(&termios_p, speed);
	termios_p.c_cc[VMIN] = 1;
	termios_p.c_cc[VTIME] = 0;
	termios_p.c_lflag &= ~(ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL);
	tcsetattr(m_motorPort, TCSANOW, &termios_p);

	/* allow asynchronous responsed */
	sprintf(str, "ANSW1%c", CR);
	write(m_motorPort, str, strlen(str)), usleep(DELAY);

	/* set mode to serial interface */
	sprintf(str, "SOR0%c", CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	/* send command to allow multiple motors */
	sprintf(str, "NET1%c", CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	/* send commands to controller to select BAUD rate */
	sprintf(str, "BAUD%ld%c", speed, CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	/* set maximum motor speed */
	sprintf(str, "SP%d%c", MAX_SPEED, CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	/* set the position range limits */
	sprintf(str, "LL0%c", CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	sprintf(str, "LL%d%c", MOTOR_MAX, CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);

	sprintf(str, "APL1%c", CR);
	write(m_motorPort, str, strlen(str)); usleep(DELAY);
}

int ECmotor::getCurrentPosition(int num)
{
	char ostr[32];
	char istr[32];
	int n;

	sprintf(ostr, "%dPOS%c", num, CR);
	write(m_motorPort, ostr, strlen(ostr));	usleep(DELAY);

	for(int j = 0; j < 32; ++j) istr[j] = 0;
	int i = 0;
	char lastChar;
	do {
		n = read(m_motorPort, &istr[i], 32);
		i += n;
		lastChar = istr[i - 1];
	} while((lastChar != CR) && (lastChar != LF));

	int pos = 0;
	sscanf(istr, "%d", &pos);

	return(pos);
}

void ECmotor::setHomePosition(int num, int pos)
{
	char str[32];
	char istr[32];

	sprintf(str, "%dHO%d%c", num, pos,CR);
	write(m_motorPort, str, strlen(str));	usleep(DELAY);
}

void ECmotor::setNewPosition(int num, int pos)
{
	char str[32];
	char istr[32];

	// set the position
	sprintf(str, "%dLA%d%c", num, pos, CR);
	write(m_motorPort, str, strlen(str));	usleep(DELAY);

	// initiate the movement
	sprintf(str, "%dM%c", num, CR);
	write(m_motorPort, str, strlen(str));	usleep(DELAY);
}

void ECmotor::changePosition(int num, int diff)
{
	char str[32];
	char istr[32];

	// set the position
	sprintf(str, "%dLR%d%c", num, diff, CR);
	write(m_motorPort, str, strlen(str));	usleep(DELAY);

	// initiate the movement
	sprintf(str, "%dM%c", num, CR);
	write(m_motorPort, str, strlen(str));	usleep(DELAY);

	return;
}

void ECmotor::disableMotor(int num, bool flag)
{
	char str[32];
	char istr[32];

	if(flag == true) {
		sprintf(str, "%dDI%c", num, CR);
		write(m_motorPort, str, strlen(str));	usleep(DELAY);
	}
	else {
		sprintf(str, "%dEN%c", num, CR);
		write(m_motorPort, str, strlen(str));	usleep(DELAY);
	}
}
