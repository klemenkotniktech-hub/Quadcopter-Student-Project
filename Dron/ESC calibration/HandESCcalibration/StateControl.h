// ********* DEFINES ***************** //
#define WAITING_TIME 500   // how many mili seconds system waits after first calibration trigger is set

//  ******** VARIABLE DEFINITION ****** //
typedef enum
{
  STATE_STANDBY = 0,
  STATE_TRANSITION_WAITING,
  STATE_CALIBRATION,
  STATE_FINISH,
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



void CalibrationDriver(PinStateType eCalibFlag,int iValueESC)
{
  static double dStartTime = 0;

  // first check whether system is in standby mode and calibration flag is set to ACTIVE
  if((ACTIVE == eCalibFlag) && (STATE_STANDBY == eMAchineState))
  {
    // set state machine to transition waiting
    eMAchineState = STATE_TRANSITION_WAITING;

    // save current time for time comparison
    dStartTime = millis();
  }

  //  check whether predefine waiting time has passed since calibration trigger flag was set
  if(((millis() - dStartTime) > WAITING_TIME) && (STATE_TRANSITION_WAITING == eMAchineState))
  {
    // check whether calibration trigger pin is still active
    if(ACTIVE == eCalibFlag)
    {
      // set state machine to calibration state
      eMAchineState = STATE_CALIBRATION;

      // set servo output signal: user needs to set it max value
      SetESCVal(iValueESC);

      // save calibration starting time for parallel time& value display
      dStartTime = millis();

      // turn on RELE
      digitalWrite(iRelePin, HIGH);  // turn on RELE
    }
    else
    {
      // calibration trigger pin was deactivated inside waiting time, switch back to standby state
      eMAchineState = STATE_STANDBY;
    }
  }

  // during calibration write to display time and ESC value
  if (STATE_CALIBRATION == eMAchineState)
  {
    // display relative time to calibration start and actual ESC value
    Serial.print("ESC value:");
    Serial.print(iValueESC);
    Serial.print("   at time:");
    Serial.println(Seconds(millis()-dStartTime));

    // set servo output signal to high
    SetESCVal(iValueESC);
  }
  
  // add logic to switch from: STATE_CALIBRATION --> STATE_FINISH

  // check whether calibration trigger is set to DEACTIVE to switch back to standby mode (in this code this is done in CALIBRATION state. otherwise it should be from FINISH state)
  if((STATE_CALIBRATION == eMAchineState) && (DEACTIVE == eCalibFlag))
  {
    // switch back state machine to standby state
    eMAchineState = STATE_STANDBY;

    // turn off RELE
    digitalWrite(iRelePin,LOW);  // turn off RELE
  }
}
