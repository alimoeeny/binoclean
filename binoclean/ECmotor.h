#ifndef ECmotor_H
#define ECmotor_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

const long BAUD_RATE = B9600;
const int DELAY = 10000;				// number of microseconds in 10 milliseconds
const int STEP_SEC = 1008;
const unsigned char LF = 10;
const unsigned char CR = 13;
const unsigned int	MAX_SPEED = 50;
const unsigned char FIRST_RATE = 8;
const unsigned char MAX_RATE = 30;
const unsigned char ACCELERATION = 200;
const int MOTOR_MAX = 150000;

class ECmotor {
 public:
	ECmotor(void);							// default constructor
	ECmotor(int port);						// class constructor
	virtual ~ECmotor();						// class destructor
	void port(int p);
	void connect();									// establish connection with motor controller
	int getCurrentPosition(int num);				// get current encoder position
	void setHomePosition(int num, int pos);			// set motor home position
	void setNewPosition(int num, int pos);			// position to move motor
	void changePosition(int num, int diff);			// amount to change motor
	void disableMotor(int num, bool flag = true);	// enable or disable the motor
	inline int port(void) { return(m_motorPort); }
 protected:
 private:
	int m_motorPort;							// port to which commands are sent
};

#endif // ECmotor_h
