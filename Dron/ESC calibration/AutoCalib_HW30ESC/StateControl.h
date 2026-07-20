// ********* DEFINES ***************** //
#define WAITING_TIME    500    // how many mili seconds system waits after first calibration trigger is set
#define HIGH_PWM_TIME   3900   // how many mili second PWM will be set to HIGH
#define CALIB_END_TIME  10500  // how many mili seconds last ESC calibration/is rele active

//  ******** VARIABLE DEFINITION ****** //
typedef enum
{
  STATE_STANDBY = 0,
  STATE_TRANSITION_WAITING,
  STATE_CALIB_HIGH_PWM,
  STATE_CALIB_LOW_PWM,
  STATE_FINISH,
  STATE_ERROR,
} StateType;

typedef enum
{
  DEACTIVE = 0,
  ACTIVE
} PinStateType;

// ********* variable declaration *****// 
extern StateType eMAchineState;
extern int iRelePin;
double dTimeStart, dSeconds;



void CalibrationDriver(PinStateType eCalibFlag)
{
  static double dStartTime = 0;

  // first check whether system is in standby mode and calibration flag is set to ACTIVE
  if((ACTIVE == eCalibFlag) && (STATE_STANDBY == eMAchineState))
  {
    // set state machine to transition waiting
    eMAchineState = STATE_TRANSITION_WAITING;

    // save current time for time comparison
    dStartTime = millis();

    Serial.println("System switch to TRANSITION WAITING STATE");
  }

  //  check whether predefine waiting time has passed since calibration trigger flag was set
  if(((millis() - dStartTime) > WAITING_TIME) && (STATE_TRANSITION_WAITING == eMAchineState))
  {
    // check whether calibration trigger pin is still active
    if(ACTIVE == eCalibFlag)
    {
      // set state machine to calibration state
      eMAchineState = STATE_CALIB_HIGH_PWM;

      // set MAX servo value = 2000
      SetESCVal(2000); //SetESCVal(map(2000, 0,1000,1000,2000));

      Serial.println("System switch to HIGH PWM STATE: PWM value set to 2000");

      // reset starting time
      dStartTime = millis();

      // turn on RELE
      digitalWrite(iRelePin, HIGH);  // turn on RELE
    }
    else
    {
      // calibration trigger pin was deactivated inside waiting time, switch back to standby state
      eMAchineState = STATE_STANDBY;

      Serial.println("System switch back to STANDBY STATE.");
    }
  }

  // during HIGH PWM STATE continuely send max PWM signal to ESC
  if (STATE_CALIB_HIGH_PWM == eMAchineState)
  {
    Serial.print("ESC value: ");
    Serial.print("2000");
    Serial.print("   at time:");
    Serial.println(Seconds(millis()-dStartTime));

    // set MAX servo value = 2000
    SetESCVal(2000); //SetESCVal(map(2000, 0,1000,1000,2000));
  }

  // check whether HIGH PWM TIME interval has passed when system is in HIGH PWM CALIB STATE
  if (((millis() - dStartTime) > HIGH_PWM_TIME) && (STATE_CALIB_HIGH_PWM == eMAchineState))
  {
    // HIGH PWM TIME interval has passed: set LOW PWM and switch state to LOW PWM
      // set state machine to LOW PWM state
      eMAchineState = STATE_CALIB_LOW_PWM;

      // set MAX servo value = 1000
      SetESCVal(1000); //SetESCVal(map(1000, 0,1000,1000,2000));

      Serial.println("System switch to LOW PWM STATE: PWM value set to 1000");
  }

  // during LOW PWM STATE continuely send min PWM signal to ESC
  if (STATE_CALIB_LOW_PWM == eMAchineState)
  {
    Serial.print("ESC value: ");
    Serial.print("1000");
    Serial.print("   at time:");
    Serial.println(Seconds(millis()-dStartTime));

    // set MAX servo value = 1000
    SetESCVal(1000); //SetESCVal(map(1000, 0,1000,1000,2000));
  }

  // check whether LOW PWM TIME interval has passed when system is in LOW PWM CALIB STATE
  if (((millis() - dStartTime) > CALIB_END_TIME) && (STATE_CALIB_LOW_PWM == eMAchineState))
  {
    // CALIB PWM TIME interval has passed: disable rele and switch to FINISH state
    // set state machine to FINISH state
    eMAchineState = STATE_FINISH;

    // turn off RELE
    digitalWrite(iRelePin,LOW);  // turn off RELE

    Serial.println("System switch to FINISHED STATE, rele is disabled.");
  }

  // check whether calibration trigger is set to DEACTIVE while calibration is finished
  if((STATE_FINISH == eMAchineState) && (DEACTIVE == eCalibFlag))
  {
    // switch back state machine to standby state
    eMAchineState = STATE_STANDBY;

    Serial.println("System switch to STANDBY STATE.");
  }

  // in case calibration trigger is set to DEACTIVE while in HIGH,LOW PWM STATE, switch to ERROR STATE and disable rele
  if  (((STATE_CALIB_HIGH_PWM == eMAchineState) || (STATE_CALIB_LOW_PWM == eMAchineState)) && (DEACTIVE == eCalibFlag))
  {
    // calibration trigger was disabled too soon while calibration, switch to ERROR STATE and disable rele
    // set state machine to ERROR state
    eMAchineState = STATE_ERROR;

    // turn off RELE
    digitalWrite(iRelePin,LOW);  // turn off RELE

    Serial.println("System switch to ERROR STATE. Rele is disabled.");
  }
}
