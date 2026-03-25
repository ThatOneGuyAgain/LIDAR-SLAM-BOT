// -------- Encoder Pins --------
#define ENC1_A 2
#define ENC1_B 4
#define ENC2_A 3
#define ENC2_B 12

volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

// -------- ISR (with direction) --------
void encoder1ISR() {
  if (digitalRead(ENC1_B) == HIGH)
    encoder1Count++;
  else
    encoder1Count--;
}

void encoder2ISR() {
  if (digitalRead(ENC2_B) == HIGH)
    encoder2Count--;
  else
    encoder2Count++;
}

void setup() {
  Serial.begin(115200);

  // Enable internal pull-ups (IMPORTANT)
  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);
  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, RISING);
}

void loop() {
  long c1, c2;

  // Prevent weird partial reads
  noInterrupts();
  c1 = encoder1Count;
  c2 = encoder2Count;
  interrupts();

  // Send clean CSV format
  Serial.print(c1);
  Serial.print(",");
  Serial.println(c2);

  delay(50);  // ~20 Hz
}
