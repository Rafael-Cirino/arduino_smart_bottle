// Interrupt configured for 10ms, using a unsigned int count I can count around until 54 minutes.
#include <SD.h>
#include <SPI.h>
#include <Wire.h>


// MICRO SD
File myFile;  // Define myfile variable
String fname = "testvai.txt";
const int chipSelect = 10;

// Timer
const uint8_t int_period = 5; // Each 10ms
const uint8_t sample_time_ms = 100/int_period; // Define sample time
volatile uint8_t count = 0;

// ACC
const int MPU = 0x68;
float AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Data array
const uint8_t num_cols = 7;
uint8_t last_idx = 0;
String aux_ = "";

const uint8_t acc_data_size = num_cols * 4;
float acc_data[acc_data_size] = {};  // To save 280ms of data

const uint8_t max_j = num_cols - 1;
const uint8_t max_i = acc_data_size - num_cols;

ISR(TIMER0_COMPA_vect) {
  OCR0A += 156;  // Advance The COMPA Register
  count += 1;
}

void setup() {
  delay(5000);

  Serial.begin(9600);
  // MICRO SD
  pinMode(chipSelect, OUTPUT);
  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // TIMER
  TCCR0A = 0;           // Init Timer0A
  TCCR0B = 0;           // Init Timer0B
  TCCR0B |= B00000101;  // Prescaler = 1024
  OCR0A = 156;        // Timer Compare0A Register
  TIMSK0 |= B00000010;  // Enable Timer COMPA Interrupt
}

void loop() {
  // Read acc
  if (count >= sample_time_ms) {
    //av_read_acc = false;
    read_acc();
    last_idx += num_cols;
  }

  if (last_idx == acc_data_size) {
    write_sd();
    last_idx = 0;
  }
}

void read_acc() {
  acc_data[last_idx + 0] = count * int_period;
  count = 0;

  acc_data[last_idx + 1] = random(1, 100);
  acc_data[last_idx + 2] = random(1, 100);
  acc_data[last_idx + 3] = random(1, 100);
  acc_data[last_idx + 4] = random(1, 100);
  acc_data[last_idx + 5] = random(1, 100);
  acc_data[last_idx + 6] = random(1, 100);
}

void write_sd() {
  // This function takes on average 20ms to write data

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fname, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    for (uint8_t i = 0; i <= max_i; i += num_cols) {
      for (uint8_t j = 0; j < num_cols; j++) {
        aux_ += String(acc_data[i + j], 2) + ",";
      }
      myFile.println(aux_);
      aux_ = "";
    }

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}
