#include <Servo.h>

const int lightSensorPin = A0;
const int speakerPin = 3;      // tone()用スピーカー
const int speakerPin2 = 5;     // PWM的なスピーカー
const int buttonPin = 2;

Servo motor;
bool stopped = false;

unsigned long lastBeepTime = 0;
unsigned long lastToggleTime = 0;  // 自作矩形波のタイミング用
bool isBeeping = false;
bool speaker2State = false;
const long beepInterval = 300;

const int freq2 = 1500; // ピン5の音程
const int halfPeriod2 = 1000000 / freq2 / 2; // μs単位

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(speakerPin2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  motor.attach(8);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    stopAll();
    stopped = true;
  }

  if (!stopped) {
    int lightValue = analogRead(lightSensorPin);
    Serial.println(lightValue);

    if (lightValue < 500) {
      moveMotor();

      unsigned long currentTime = millis();
      if (currentTime - lastBeepTime >= beepInterval) {
        lastBeepTime = currentTime;
        isBeeping = !isBeeping;

        if (isBeeping) {
          makeNoise();
        } else {
          stopNoise();
        }
      }

      // ピン5で矩形波の手動出力（擬似tone）
      if (isBeeping && micros() - lastToggleTime >= halfPeriod2) {
        lastToggleTime = micros();
        speaker2State = !speaker2State;
        digitalWrite(speakerPin2, speaker2State ? HIGH : LOW);
      }

    } else {
      stopAll();
    }
  }
}

void makeNoise() {
  tone(speakerPin, 1000); // ピン3で主音
  lastToggleTime = micros(); // ピン5のトグル開始
}

void stopNoise() {
  noTone(speakerPin);
  digitalWrite(speakerPin2, LOW);
}

void moveMotor() {
  motor.write(0);
}

void stopMotor() {
  motor.write(90);
}

void stopAll() {
  stopNoise();
  stopMotor();
  isBeeping = false;
}
