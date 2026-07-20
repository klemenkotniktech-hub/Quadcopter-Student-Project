// ********* INCLUDES ********************//
#include <Adafruit_PWMServoDriver.h>
#include<Servo.h>
#include "Inicializacija.h"

//  ******** DEFINES *********************//


//***** global variables  ********//

StateType    eMAchineState;  // system starts in standby mode
PinStateType eCalibFlag;     // 0-> disabled1, -> active,  

int iCalibTrigPin = 5;     // calibration trigger pin 5
int iRelePin      = 3;     // RELE pin
int iPotentVal    = 0;     // potentiometer raw value
int iValueESC     = 1000;  // actual ESC value to be send to ESC

// *** INICIALIZATION ***** //
void setup()
{
  eMAchineState = STATE_STANDBY;
  Serial.begin(9600);
  pinMode(iCalibTrigPin,INPUT_PULLUP);  // init calibration trigger pin
  pinMode(iRelePin, OUTPUT);            // init RELE pin
  digitalWrite(iRelePin,LOW);           // set RELE pin to LOW

  InitESC();
}


// ***** MAIN LOOP ******//
void loop() {
  // read calibration flag trigger pin
  eCalibFlag = (PinStateType) !digitalRead(iCalibTrigPin);  // turn on switch -> 1, turn of switch -> 0

  // read potentiometer raw value (not needy in automated version)
  iPotentVal = (int) ((double) analogRead(A0)*1000/1024);
  // limit iPotenVal between 0 and 1000
  if (0 > iPotentVal)
  {
      iPotentVal = 0;
  }
  if (1000 < iPotentVal)
  {
      iPotentVal = 1000;
  }
  
  // convert raw potentiometer value to ESC corrected value
  iValueESC = map(iPotentVal, 0,1000,1000,2000);

  CalibrationDriver(eCalibFlag,iValueESC);

}
