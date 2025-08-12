
Title: URBAN FARMING
Names: Jayden Toh, Jerren Lim
Admission No's: 2517072, 2505875
Class: DCEP/FT/1A/09
Description: monitoring and control of temp and humidity (measuring the temp. and humidity of different zones for the crops)

#include <Wire.h>

#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#include "PCA9685.h" 
#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
TM1637 disp(CLK,DIO);
PCA9685 pwmController(Wire);
PCA9685_ServoEval pwmServo1;


DHT dht;
unsigned long startTime;




void setup() {
  disp.init();  
	dht.begin();
  Serial.begin(9600);
  startTime = millis();
  
  pwmController.resetDevices();  
  pwmController.init();
  pwmController.setPWMFreqServo();
  pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-10));
  pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(0));
  delay(1000);

}

int i;  //for loop control

void loop() {
  do{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(t) || isnan(h)) {
    displayError();
      } 
  else{
    displayTemperature((int8_t)t);//
    delay(3000);
    displayHumidity((int8_t)h);//
    Serial.print(t);
    Serial.print(",");
    Serial.println(h);
    delay(3000);
  }
      for (i = 0; i <= 150; i += 5) { 
        pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
        delay(100); 
    }

    for (i = 150; i >= 0; i -= 5) { 
        pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
        delay(100); 
    }

    for (i = 0; i >= -150; i -= 5) { 
        pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
        delay(100); 
    }

    for (i = -150; i <= 0; i += 5) { 
        pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
        delay(100);
    }
    
    for (i = -10; i <= 60; i += 5) {
        pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));
        delay(100); 
    }
    
    for (i = 60; i >= -10; i -= 5) { 
        pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));
        delay(100);  
    }
    
    }while(millis() - startTime < 120000);
  else {
    while (true);
  }

}


void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  if(temperature < 0)
	{
		temp[0] = INDEX_NEGATIVE_SIGN;
		temperature = abs(temperature);
	}
	else if(temperature < 100)temp[0] = INDEX_BLANK;
	else temp[0] = temperature/100;
	temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
  }
  
void displayHumidity(int8_t humi)
{
  int8_t temp[4];
  if(humi < 100)temp[0] = INDEX_BLANK;
  else temp[0] = humi/100;
  humi %= 100;
  temp[1] = humi / 10;
  temp[2] = humi % 10;
  temp[3] = 18;	          //index of 'H' for celsius degree symbol.
  disp.display(temp);
  
}

void displayError()
{
  disp.display(3,14);//display "E"
}


