#define ESC_PIN 9  // ESC servo pin

Servo esc1;

void InitESC(void)
{
    // define 
    esc1.attach(ESC_PIN);
    esc1.writeMicroseconds(1000);
}

void SetESCVal(int iValueESC)
{
  esc1.writeMicroseconds(iValueESC);
}
