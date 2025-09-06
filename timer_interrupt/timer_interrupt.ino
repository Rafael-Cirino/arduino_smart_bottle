// Interrupt configured for 10ms, using a unsigned int count I can count around until 54 minutes.

unsigned int count =  0;
int sec = 0;
bool check = false;

ISR(TIMER0_COMPB_vect)
{
  OCR0B += 156; // Advance The COMPA Register
  // Handle The Timer Interrupt
  //...
  count += 1;

  if (count == 500){
    count = 0;
    check = true;
    sec += 1;
  }
}

void setup()
{
  TCCR0A = 0;           // Init Timer0A
  TCCR0B = 0;           // Init Timer0B
  TCCR0B |= B00000101;  // Prescaler = 1024
  OCR0B = 156;        // Timer Compare0B Register
  TIMSK0 |= B00000100;  // Enable Timer COMPB Interrupt

  Serial.begin(9600);
}

void loop()
{
  if (check) {
    Serial.println(sec);
    check = false;
  }
  
}
