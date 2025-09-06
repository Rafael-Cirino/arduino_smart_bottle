// Interrupt configured for 10ms, using a unsigned int count I can count around until 54 minutes.
#include <SD.h>
#include <SPI.h>
#include <Wire.h>


// MICRO SD
File myFile;  // Define myfile variable
const int chipSelect = 10;

// Timer
unsigned int count = 0;
unsigned int sec = 0;
bool check = false;

// ACC
const int MPU = 0x68;
float AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Data array
String acc_data[3] = {"0", "0", "0"};

ISR(TIMER0_COMPB_vect) {
  OCR0B += 156;  // Advance The COMPA Register
  // Handle The Timer Interrupt
  //...
  count += 1;

  if (count == 100) {
    count = 0;
    check = true;
    sec += 1;
    //Serial.println("");
  }
}

void setup() {
  // TIMER
  TCCR0A = 0;           // Init Timer0A
  TCCR0B = 0;           // Init Timer0B
  TCCR0B |= B00000101;  // Prescaler = 1024
  OCR0B = 156;          // Timer Compare0B Register
  TIMSK0 |= B00000100;  // Enable Timer COMPB Interrupt


  Serial.begin(9600);
  // MICRO SD
  pinMode(chipSelect, OUTPUT);


  //Serial.print("Initializing SD card...");

  //if (!SD.begin()) {
  //  Serial.println("initialization failed!");
  //  return;
  //}
  //Serial.println("initialization done.");
}

void loop() {
  if (check) {
    //write_sd();
    Serial.println(sec);
    check = false;
    acc_data[0] = "Brasiiiiiil";
    for (int i = 0; i < sizeof(acc_data)/sizeof(String); i++) {
      Serial.print(acc_data[i]);
      Serial.print(", ");
    }
    Serial.println("");
  }
}

void write_sd() {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("sec: " + String(sec));
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
