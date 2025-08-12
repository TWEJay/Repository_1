
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
#define LED_GREEN   5
#define LED_BLUE    6
#define LED_YELLOW  7
#define BUTTONK1 8
#define BUTTONK2 9
TM1637 disp(CLK,DIO);
PCA9685 pwmController(Wire);
PCA9685_ServoEval pwmServo1;


DHT dht;
unsigned long startTime;

// Prototype
void handleButtonPress(void);
void displayChoices(void);

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
// LEDs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  //Buttons
  pinMode(BUTTONK1, INPUT_PULLUP);
  pinMode(BUTTONK2, INPUT_PULLUP);

  // Initialize
  Serial.begin(9600);
  disp.init();  
	dht.begin();
  displayChoices();

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
  handleButtonPress();
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

// === Displaying Choices ===
void displayChoices(void)
{
Serial.println("Select Option");
Serial.println("1. Zone 1 (YELLOW LED)");
Serial.println("Default - No Buttons are pressed");
Serial.println("2. Zone 2 (BLUE LED)");
Serial.println("BUTTONK2 is pressed and hold to move forward from pin 7 to 6");
Serial.println("3. Zone3 (GREEN LED");
Serial.println("BUTTONK2 is pressed and hold again to move forward from pin 6 to 5");
delay(1000);
return;
}

// === Button handling ===
void handleButtonPress(void) 
{

  if (digitalRead(BUTTONK2) == 0)   //Reads the buttonk2
  {
    for (int i=5; i<= 7; i++) 
      digitalWrite(i, LOW);

   digitalWrite(mode, HIGH);  //Display the led according to the pin number

   mode--;      //Displaying the buttonk2 starting from yellow led
   if (mode < 5)
    mode = 7;

   while (digitalRead(BUTTONK2) == 0);
  }

  return;
}




//#include "RichShieldDHT.h"
#include "RichShieldTM1637.h" // For 4-digit 7-segment display

// === Pins ===
#define DHTPIN 12
#define DHTTYPE DHT22
#define LED_GREEN   5
#define LED_BLUE    6
#define LED_YELLOW  7
#define BUTTONK1 8
#define BUTTONK2 9

// 7-segment display using TM1637
#define CLK 10
#define DIO 11
TM1637 disp(CLK,DIO);

DHT dht;

// Prototype
void handleButtonPress(void);
void displayChoices(void);

void setup() {
  // LEDs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  //Buttons
  pinMode(BUTTONK1, INPUT_PULLUP);
  pinMode(BUTTONK2, INPUT_PULLUP);

  // Initialize
  Serial.begin(9600);
  disp.init();  
	dht.begin();
  displayChoices();
}

// Variables 
int mode = 7;

void loop(){
  
  handleButtonPress();
  return 0;
}

// === Displaying Choices ===
void displayChoices(void)
{
Serial.println("Select Option");
Serial.println("1. Zone 1 (YELLOW LED)");
Serial.println("Default - No Buttons are pressed");
Serial.println("2. Zone 2 (BLUE LED)");
Serial.println("BUTTONK2 is pressed and hold to move forward from pin 7 to 6");
Serial.println("3. Zone3 (GREEN LED");
Serial.println("BUTTONK2 is pressed and hold again to move forward from pin 6 to 5");
delay(1000);
return;
}

// === Button handling ===
void handleButtonPress(void) 
{

  if (digitalRead(BUTTONK2) == 0)   //Reads the buttonk2
  {
    for (int i=5; i<= 7; i++) 
      digitalWrite(i, LOW);

   digitalWrite(mode, HIGH);  //Display the led according to the pin number

   mode--;      //Displaying the buttonk2 starting from yellow led
   if (mode < 5)
    mode = 7;

   //insert entry point for temp humidity sensor here 
    temp_main()

   while (digitalRead(BUTTONK2) == 0);
   delay(1500);
  }

	

  return;
}



void main(void) {
	displayChoices()//console displays instructions 
    //handleButtonPress() //busy waiting for input 

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
void temp_main() {
  //handleButtonPress();
  //do{
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
}





main()

