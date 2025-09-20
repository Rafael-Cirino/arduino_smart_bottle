// Interrupt configured for 10ms.

// Devices
// Arduino Uno
// Acc - MPU-6050
// Micro SD

// Micro SD module
#include <SD.h>
#include <SPI.h>

// Acc module
#include <Adafruit_MPU6050.h>
#include <Wire.h>

// Timer
const uint8_t interrupt_ms = 10;               // Each 10ms
const uint8_t sample_ms = 100 / interrupt_ms;  // Define sample time
volatile uint8_t count = 0;

// MICRO SD
File myFile;  // Define myfile variable
String fname = "trc_1.txt";
const int chipSelect = 10;

// ACC
sensors_event_t a, g, temp;
Adafruit_MPU6050 mpu;

// Data array
const uint8_t num_cols = 8;
uint8_t last_idx = 0;
String write_aux = "";

const uint8_t acc_data_size = num_cols * 4;
int acc_data[acc_data_size] = {};  // To save 280ms of data

const uint8_t max_j = num_cols - 1;
const uint8_t max_i = acc_data_size - num_cols;

ISR(TIMER0_COMPA_vect) {
  OCR0A += 156;  // Advance The COMPA Register
  count += 1;
}

void setup() {
  // Wait 5sec before load start system
  delay(10000);

  Serial.begin(115200);
  // Init micro SD module
  pinMode(chipSelect, OUTPUT);
  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Init acc module
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Configure TIMER interrupt
  TCCR0A = 0;           // Init Timer0A
  TCCR0B = 0;           // Init Timer0B
  TCCR0B |= B00000101;  // Prescaler = 1024
  OCR0A = 156;          // Timer Compare0A Register
  TIMSK0 |= B00000010;  // Enable Timer COMPA Interrupt
}

void loop() {
  // Read acc
  if (count >= sample_ms) {
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
  // Read accelerometer data from MPU-6050 module

  // ACC - m/s^2.
  // Gyro - rad/s.
  // Temperature - Celsius.
  mpu.getEvent(&a, &g, &temp);
  acc_data[last_idx + 0] = count * interrupt_ms;
  count = 0;

  acc_data[last_idx + 1] = a.acceleration.x * 100;
  acc_data[last_idx + 2] = a.acceleration.y * 100;
  acc_data[last_idx + 3] = a.acceleration.z * 100;
  acc_data[last_idx + 4] = g.gyro.x * 100;
  acc_data[last_idx + 5] = g.gyro.y * 100;
  acc_data[last_idx + 6] = g.gyro.z * 100;
  acc_data[last_idx + 7] = 0;

  // Get temperature temp.temperature
}

void write_sd() {
  // This function takes on average 30ms to write data

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fname, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    for (uint8_t i = 0; i <= max_i; i += num_cols) {
      for (uint8_t j = 0; j < num_cols; j++) {
        write_aux += String(acc_data[i + j]) + ",";
      }
      myFile.println(write_aux);
      write_aux = "";
    }

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}
