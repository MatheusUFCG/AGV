#include <Sensor_HC_SR04.h>
#include <CAN.h>
#include <Encoder.h> 
#include <SPI.h>

#include <Motor.h>
#include "Arduino.h"

#define DEBUG


Encoder   encoderMotor1(1),encoderMotor2(2);
Motor     motor_m1(1), motor_m2(2);
Sensor_HC_SR04 ultrasonicLeftBack(1);
Sensor_HC_SR04 ultrasonicBack(2);
Sensor_HC_SR04 ultrasonicRightBack(3);
Sensor_HC_SR04 ultrasonicRightFront(4);
Sensor_HC_SR04 ultrasonicFront(5);
Sensor_HC_SR04 ultrasonicLeftFront(6);


float cmDistance[7];

long encoder1count;
long encoder2count;

char getCan; 

void setup() {

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  Serial.begin(9600);

  encoderMotor1.clearEncoderCount();
  encoderMotor2.clearEncoderCount();
}

void loop() {
  
  cmDistance[1] = ultrasonicLeftBack.distance_cm();		
  cmDistance[2] = ultrasonicBack.distance_cm();	
  cmDistance[3] = ultrasonicRightBack.distance_cm();	
  cmDistance[4] = ultrasonicRightFront.distance_cm();		
  cmDistance[5] = ultrasonicFront.distance_cm();	
  cmDistance[6] = ultrasonicLeftFront.distance_cm();	
  
  // try to parse packet
  int packetSize = CAN.parsePacket();
  if (packetSize) 
  {
	getCan = (char)CAN.read();
	
	switch(getCan)
	{
		case 'a':	 //up
			CAN.beginPacket(0x12);
			CAN.write('a');
			CAN.endPacket();
			motor_m1.w_comando(200);
			motor_m2.w_comando(200);
		case 'b': 	//down
			CAN.beginPacket(0x12);
			CAN.write('b');
			CAN.endPacket();
			motor_m1.w_comando(-200);
			motor_m2.w_comando(-200);
		case 'c':	//left
			CAN.beginPacket(0x12);
			CAN.write('c');
			CAN.endPacket();
			motor_m1.w_comando(120);
			motor_m2.w_comando(0);
		case 'd':	//right
			CAN.beginPacket(0x12);
			CAN.write('d');
			CAN.endPacket();
			motor_m1.w_comando(0);
			motor_m2.w_comando(120);
		default: 	//stop
			CAN.beginPacket(0x12);
			CAN.write('e');
			CAN.endPacket();
			motor_m1.w_comando(0);
			motor_m2.w_comando(0);
	}
  }
  
  // Encoder Motor 1
  Serial.println("Leitura do encoder do motor 1");
  encoder1count = encoderMotor1.readEncoder();
  Serial.print("Encoder 01: "); Serial.println(encoder1count)
  // Encoder Motor 2
  Serial.println("Leitura do encoder do motor 2");
  encoder1count = encoderMotor2.readEncoder();
  Serial.print("Encoder 02: "); Serial.println(encoder2count)

  // Anti-Colission System
  
  if(cmDistance[1]<=5||cmDistance[2]<=5||cmDistance[3]<=5||cmDistance[4]<=5||cmDistance[5]<=5||cmDistance[6]<=5)
  {
    motor_m1.w_comando(0);
	motor_m2.w_comando(0);
  }
}
