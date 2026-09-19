// Interrupt configured for 10ms.

// Devices
// Arduino Uno
// Acc - MPU-6050
// Bluetooth HC-06

#include <SoftwareSerial.h>

// Acc module
#include <Adafruit_MPU6050.h>
#include <Wire.h>

// Timer
const uint8_t interrupt_ms = 10;               // Each 10ms
const uint8_t sample_ms = 100 / interrupt_ms;  // Define sample time
volatile uint8_t count = 0;

// ACC
sensors_event_t a, g, temp;
Adafruit_MPU6050 mpu;

// Bluetooth
SoftwareSerial BT(10, 11);
volatile int timer_bluetooth = 0;

// Sip status
volatile uint8_t sip_status = 0;
volatile int sip_duration = 0;
int buf_sip[20] = {};
uint8_t sip_detecteds = 0;

ISR(TIMER0_COMPA_vect) {
  OCR0A += 156;  // Advance The COMPA Register
  count += 1;
  timer_bluetooth += 1;
}

void setup() {
  delay(5000);
  BT.begin(9600);
  BT.println("Conectado com o Arduino");
  BT.println("");

  for(int i = 0; i < sizeof(buf_sip); i++){
    buf_sip[i] = 0;
  }

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
    read_acc();
  }

  if ((timer_bluetooth >= (1000)) && (!sip_status)){
    float sip_mean = 0;
    BT.print("Nos últimos 10seg: ");
    BT.print(sip_detecteds);
    BT.println(" goles");

    for(int i = 0; i <= sip_detecteds; i++){
      if (buf_sip[i] > 0){
        BT.print("Gole detectado com duração: ");
        BT.print(buf_sip[i]);
        BT.println(" ms");
        sip_mean += buf_sip[i];
      }

      buf_sip[i] = 0;
    }

    if (sip_detecteds > 0){
      BT.print("Duração média: ");
      BT.print(sip_mean/sip_detecteds);
      BT.println(" ms");
    }

    BT.println("");
    timer_bluetooth = 0;
    sip_detecteds = 0;
  }
}

int linear_model(float acc_x, float gyro_y){
  float y = 0.0635120170557861 + acc_x * -0.06297311 + gyro_y * 0.02817046;
  y = sigmoid(y);

  if (y > 0.57){
    return 1;
  } else {
    return 0;
  }
}

float sigmoid(float x){
  return 1/(1 + exp(-x));
}

void read_acc() {
  // Read accelerometer data from MPU-6050 module

  // ACC - m/s^2.
  // Gyro - rad/s.
  // Temperature - Celsius.
  mpu.getEvent(&a, &g, &temp);
  sip_status = linear_model(a.acceleration.x, g.gyro.y);

  if (sip_status) {
    sip_duration += count * interrupt_ms;
  } else {
    if (sip_duration > 0){
      buf_sip[sip_detecteds] = sip_duration;
      sip_detecteds += 1;
      sip_duration = 0;
    }
  }
  count = 0;
}