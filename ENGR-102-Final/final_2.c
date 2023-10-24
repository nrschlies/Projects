/*
 * PROJECT 2:
 * Code developed within the context of ENGR102: Electromechanical Design at the University of San Diego
 * With aid from www.osoyoo.com, specifically lessons 2 and 3
 * Questions or comments can be addressed to the following:
 * Noah Schliesman: nschliesman@sandiego.edu
 * Michaela Divito: mdivito@sandiego.edu
 * Jackson Hopper: jhopper@sandiego.edu
 * Mohammed Alghoribi: malghoribi@sandiego.edu
 * 
*/

#include "pca9685/pca9685.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

int fd;

//define L298N control pins in wPi system
#define ENA 0  //left motor speed pin ENA connect to PCA9685 port 0
#define ENB 1  //right motor speed pin ENB connect to PCA9685 port 1
#define IN1 4  //Left motor IN1 connect to wPi pin# 4 (Physical 16,BCM GPIO 23)
#define IN2 5  //Left motor IN2 connect to wPi pin# 5 (Physical 18,BCM GPIO 24)
#define IN3 2  //right motor IN3 connect to wPi pin# 2 (Physical 13,BCM GPIO 27)
#define IN4 3  //right motor IN4 connect to wPi pin# 3 (Physical 15,BCM GPIO 22)
#define SERVO_PIN 15  //right motor speed pin ENB connect to PCA9685 port 1
#define LEFT 400 //ultrasonic sensor facing right
#define CENTER 280//ultrasonic sensor facing front
#define RIGHT 160 //ultrasonic sensor facing left
#define TRIG 28 //wPi#28=BCM GPIO#20=Physical pin#38
#define ECHO 29 //wPi#29=BCM GPIO#21=Physical pin#40
#define OBSTACLE 20
#define short_delay 200
#define long_delay  300
#define extra_long_delay 400

//Speeds
#define high_speed 3000  // Max pulse length out of 4096
#define mid_speed  2000  // Max pulse length out of 4096
#define low_speed  1000  // Max pulse length out of 4096
#define SPEED 2000
#define HIGH_SPEED 3000
#define LOW_SPEED 1500

//define IR tracking sensor wPi pin#
#define sensor1 21 // No.1 sensor from far left to wPi#21 Physical pin#29
#define sensor2 22 // No.2 sensor from left to wPi#22 Physical pin#31
#define sensor3 23 // middle sensor to wPi#23 Physical pin#33
#define sensor4 24 // No.2 sensor from right to wPi#24 Physical pin#35
#define sensor5 25 // No.1 sensor from far  right to wPi#25 Physical pin#37

char val[5]; //sensor value array
int SL=(LEFT+CENTER)/2;
int SR=(RIGHT+CENTER)/2;
int sts1=0;
int sts2=0;
int sts3=0;
char val2[3];

void setup(){
	pinMode(IN1,OUTPUT);
	pinMode(IN2,OUTPUT);
	pinMode(IN3,OUTPUT);
	pinMode(IN4,OUTPUT);
	pinMode(TRIG,OUTPUT);
	pinMode(ECHO,INPUT);
	pinMode(sensor1,INPUT);
	pinMode(sensor2,INPUT);
	pinMode(sensor3,INPUT);
	pinMode(sensor4,INPUT);
	pinMode(sensor5,INPUT);
	digitalWrite(IN1,LOW);
	digitalWrite(IN2,LOW);
	digitalWrite(IN3,LOW);
	digitalWrite(IN4,LOW);
}
//IR Tracking Commands
void go_Back(int fd,int l_speed,int r_speed) {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW); 
    pca9685PWMWrite(fd, ENA, 0, l_speed);
    pca9685PWMWrite(fd, ENB, 0, r_speed);
}
void go_Advance(int fd,int l_speed,int r_speed) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH); 
    pca9685PWMWrite(fd, ENA, 0, l_speed);
    pca9685PWMWrite(fd, ENB, 0, r_speed);
}
void go_Left(int fd,int l_speed,int r_speed) {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH); 
    pca9685PWMWrite(fd, ENA, 0, l_speed);
    pca9685PWMWrite(fd, ENB, 0, r_speed);
}
void go_Right(int fd,int l_speed,int r_speed) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW); 
    pca9685PWMWrite(fd, ENA, 0, l_speed);
    pca9685PWMWrite(fd, ENB, 0, r_speed);
}
void stop_car(int fd) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW); 
    pca9685PWMWrite(fd, ENA, 0, 0);
    pca9685PWMWrite(fd, ENB, 0, 0);
}
//Obstacle Detection Commands
void go_back(int fd,int speed) {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW); 
    pca9685PWMWrite(fd, ENA, 0, speed);
    pca9685PWMWrite(fd, ENB, 0, speed);
}
void go_advance(int fd,int speed) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH); 
    pca9685PWMWrite(fd, ENA, 0, speed);
    pca9685PWMWrite(fd, ENB, 0, speed);
}
void go_left(int fd,int left_speed,int right_speed) {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH); 
    pca9685PWMWrite(fd, ENA, 0, left_speed);
    pca9685PWMWrite(fd, ENB, 0, right_speed);
}
void go_right(int fd,int left_speed,int right_speed) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW); 
    pca9685PWMWrite(fd, ENA, 0, left_speed);
    pca9685PWMWrite(fd, ENB, 0, right_speed);
}
// ctrl-C key event handler
void my_handler(int s){
           stop_car(fd);
           printf("Ctrl C detected %d\n",s);
           exit(1); 

}
int distance() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO) == LOW);
        
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 		if (distance==0) distance=1000;
        return distance;
}

int both_modes() {
	// Setup with pinbase 300 and i2c location 0x40
	int fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}
	// following 5 lines define ctrl-C events
   	struct sigaction sigIntHandler;
   	sigIntHandler.sa_handler = my_handler;
   	sigemptyset(&sigIntHandler.sa_mask);
   	sigIntHandler.sa_flags = 0;
   	sigaction(SIGINT, &sigIntHandler, NULL);
   	
	pca9685PWMWrite(fd, SERVO_PIN, 0, LEFT);
	delay(1000);
	pca9685PWMWrite(fd, SERVO_PIN, 0, CENTER);
	delay(1000);
	pca9685PWMWrite(fd, SERVO_PIN, 0, RIGHT);
	delay(1000);
	pca9685PWMWrite(fd, SERVO_PIN, 0, CENTER);
	delay(1000);
	
	val[0]='0'+!digitalRead(sensor1);
	val[1]='0'+!digitalRead(sensor2);
	val[2]='0'+!digitalRead(sensor3);
	val[3]='0'+!digitalRead(sensor4);
	val[4]='0'+!digitalRead(sensor5); 
	while (1) {
		pca9685PWMWrite(fd, SERVO_PIN, 0, LEFT);
		delay(300);
		if (distance()<OBSTACLE) sts1=1;
		else sts1=0;
		val2[0]='0'+sts1;
		
		pca9685PWMWrite(fd, SERVO_PIN, 0, CENTER);
		delay(300);
		if (distance()<OBSTACLE) sts2=1;
		else sts2=0;
		val2[1]='0'+sts2;
		
		pca9685PWMWrite(fd, SERVO_PIN, 0, RIGHT);
		delay(300);
		if (distance()<OBSTACLE) sts3=1;
		else sts3=0;
		val2[2]='0'+sts3;
		
		if (strcmp("100",val2)==0) {	
			printf("100 slight right\n");
			go_right(fd,SPEED,0);
            delay(long_delay);  
            stop_car(fd);
            delay(short_delay);
		}
		if (strcmp("001",val2)==0) {
			printf("100 slight left\n");
			go_left(fd,0,SPEED);
            delay(long_delay);  
            stop_car(fd);
            delay(short_delay);		
		}
		if (strcmp("110",val2)==0) {
			printf("110 sharp right\n");
			go_right(fd,HIGH_SPEED,LOW_SPEED);
            delay(long_delay);  
            stop_car(fd);
            delay(short_delay);	
		}
		if (strcmp("011",val2)==0) {
			printf("011 sharp left\n");
			go_left(fd,LOW_SPEED,HIGH_SPEED);
            delay(long_delay);  
            stop_car(fd);
            delay(short_delay);	
		}  
		if (strcmp("111",val2)==0 || strcmp("101",val2)==0 || strcmp("010",val2)==0) {
			printf("%s sharp turn back\n",val);
			go_left(fd,HIGH_SPEED,HIGH_SPEED);
            delay(extra_long_delay);  
            stop_car(fd);
            delay(short_delay);	
		}        
		if (strcmp("000",val2)==0) {
			printf("000 move forward\n");
			go_advance(fd,SPEED);
            delay(long_delay);  
            stop_car(fd);
            delay(short_delay);	
		} 
		//Leveraging else if statements to give object detection priority
		else if (strcmp("10000",val)==0 || strcmp("01000",val)==0 || strcmp("11000",val)==0) { 
            printf("%s sharp left\n",val);       
            //The black line is in the left of the car, need  left turn 
            go_Left(fd,low_speed,low_speed);  //Turn left
            delay(short_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("11100",val)==0 || strcmp("10100",val)==0) {
            printf("%s slight left\n",val);
            go_Advance(fd,low_speed,high_speed);  //Turn left slightly
            delay(long_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("11110",val)==0 || strcmp("01100",val)==0 ||
         strcmp("10010",val)==0 || strcmp("10110",val)==0 || strcmp("11010",val)==0) {
            printf("%s ahead left\n",val);
            go_Advance(fd,low_speed,mid_speed);  //go ahead
            delay(long_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("01110",val)==0 || strcmp("01010",val)==0 ||
         strcmp("00100",val)==0 || strcmp("10101",val)==0 || 
         strcmp("10011",val)==0 || strcmp("11101",val)==0 || 
         strcmp("10111",val)==0 || strcmp("11011",val)==0 || strcmp("11001",val)==0) {
            printf("%s ahead\n",val);
            go_Advance(fd,mid_speed,mid_speed);  //go ahead
            delay(long_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("00110",val)==0 || strcmp("01111",val)==0 ||
         strcmp("01001",val)==0 || strcmp("01011",val)==0 || strcmp("01101",val)==0)
        {
            printf("%s ahead right\n",val);
            go_Advance(fd,mid_speed,low_speed);  //Turn left
            delay(long_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("00111",val)==0 || strcmp("00101",val)==0) {
            printf("%s slight right\n",val);
            go_Advance(fd,high_speed,low_speed);  //Turn right slightly
            delay(long_delay);
            stop_car(fd);
            delay(short_delay);
        }
        else if (strcmp("00001",val)==0 || strcmp("00010",val)==0 ||
         strcmp("00011",val)==0) {
            printf("%s sharp right\n",val);
            go_Right(fd,low_speed,low_speed);  //Turn right sharply
            delay(short_delay);
            stop_car(fd);
            delay(short_delay);
        }  
         else if (strcmp("00000",val)==0) {
            printf("%s roll back\n",val);
            go_Back(fd,low_speed,low_speed);  //Turn right sharply
            delay(short_delay);
            stop_car(fd);
			delay(short_delay);
         }  
        else if (strcmp("11111",val)==0) { //Stop
			printf("%s stop\n",val);
			stop_car(fd);
        }
	}
}
int main(void) {
	//failsafe for Pi Setup
	if(wiringPiSetup()==-1){
        printf("setup wiringPi failed!\n");
        printf("please check your setup\n");
        return -1;
    }
    setup();
    while(1) {
		both_modes();
	}
}
