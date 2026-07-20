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

  CalibrationDriver(eCalibFlag);

}
