#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// -------- Motor Pins --------
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10

#define ENA 5
#define ENB 6

// -------- Encoder Pins --------
#define ENC1_A 2
#define ENC1_B 4
#define ENC2_A 3
#define ENC2_B 12

volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

// -------- MPU6050 --------
Adafruit_MPU6050 mpu;

// -------- Settings --------
int pwmValue = 150;   // 0–255

void encoder1ISR() {
  encoder1Count++;
}

void encoder2ISR() {
  encoder2Count++;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Encoder pins
  pinMode(ENC1_A, INPUT);
  pinMode(ENC1_B, INPUT);
  pinMode(ENC2_A, INPUT);
  pinMode(ENC2_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, RISING);

  // Set motors forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, pwmValue);
  analogWrite(ENB, pwmValue);

  // Initialize MPU6050
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("System Ready");
}

void loop() {
  static long lastCount1 = 0;
  static long lastCount2 = 0;

  delay(1000);  // 1 second sample interval

  // ----- Encoder Data -----
  long count1 = encoder1Count;
  long count2 = encoder2Count;

  long pulses1 = count1 - lastCount1;
  long pulses2 = count2 - lastCount2;

  lastCount1 = count1;
  lastCount2 = count2;

  // ----- MPU6050 Data -----
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // ----- Print Everything -----
  Serial.println("---- DATA ----");

  Serial.print("Motor1 pulses/sec: ");
  Serial.print(pulses1);
  Serial.print(" | Motor2 pulses/sec: ");
  Serial.println(pulses2);

  Serial.print("Accel: ");
  Serial.print(a.acceleration.x); Serial.print(" ");
  Serial.print(a.acceleration.y); Serial.print(" ");
  Serial.println(a.acceleration.z);

  Serial.print("Gyro: ");
  Serial.print(g.gyro.x); Serial.print(" ");
  Serial.print(g.gyro.y); Serial.print(" ");
  Serial.println(g.gyro.z);

  Serial.print("Temp: ");
  Serial.println(temp.temperature);

  Serial.println("-------------------");
}
