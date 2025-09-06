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
bool av_read_acc = false;
int count_old = 0;

// ACC
const int MPU = 0x68;
float AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Data array
const int num_cols = 7;
int last_idx = 0;

const int acc_data_size = 10;
String acc_data[acc_data_size] = {};  // To save 280ms of data

float aux_data[num_cols] = {0, 0, 0, 0, 0, 0, 0};

ISR(TIMER0_COMPB_vect) {
  OCR0B += 156;  // Advance The COMPA Register
  // Handle The Timer Interrupt
  //...
  count += 1;

  if (count % 5 == 0){ // Each 100ms
    av_read_acc = true;
  }

  if (count % 100 == 0) { // Each 1 second
    check = true;
    sec += 1;
  }
}

void setup() {
  delay(3000);
  // TIMER
  TCCR0A = 0;           // Init Timer0A
  TCCR0B = 0;           // Init Timer0B
  TCCR0B |= B00000101;  // Prescaler = 1024
  OCR0B = 156;          // Timer Compare0B Register
  TIMSK0 |= B00000100;  // Enable Timer COMPB Interrupt


  Serial.begin(9600);
  // MICRO SD
  pinMode(chipSelect, OUTPUT);
  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  // Read acc
  if (av_read_acc) {
    av_read_acc = false;
    read_acc();
    last_idx += 1;
  }

  if (check) {
    //Serial.println(sec);
    check = false;
  }

  if (last_idx == acc_data_size) {
    //int count_init = count;
    write_sd();
    last_idx = 0;
    //Serial.println("time: " + String(count - count_init));
  }
}

void read_acc() {
  aux_data[0] = count;
  aux_data[1] = random(1, 10);
  aux_data[2] = random(1, 10);
  aux_data[3] = random(1, 10);
  aux_data[4] = random(1, 10);
  aux_data[5] = random(1, 10);

  String aux_ = "";
  for (int i = 0; i < num_cols; i++){
    aux_ += String(aux_data[i], 0) + ",";
  }
  acc_data[last_idx] = aux_;
  aux_ = "";
  /*
  acc_data[last_idx + 0] = count;
  acc_data[last_idx + 1] = random(1, 10);
  acc_data[last_idx + 2] = random(1, 10);
  acc_data[last_idx + 3] = random(1, 10);
  acc_data[last_idx + 4] = random(1, 10);
  acc_data[last_idx + 5] = random(1, 10);
  acc_data[last_idx + 6] = random(1, 10);
  acc_data[last_idx + 0]
  */
}

void write_sd() {
  // This function takes on average 20ms to write data

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println(acc_data[0]);
    for (int i = 0; i < acc_data_size; i++) {
      myFile.print(acc_data[i]);
      myFile.println("");
    }

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
