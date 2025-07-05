#include <Servo.h>

const int lightSensorPin = A0;
const int speakerPin = 3;
const int buttonPin = 2;

Servo motor1;  // 連続回転サーボ
Servo motor2;  // 連続回転サーボ

bool stopped = false;

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  motor1.attach(8);
  motor2.attach(9);
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
      makeNoise(1000);
      moveMotorsForward();  // 前進
      delay(300);
      stopNoise();
      stopMotors();
      delay(300);
    } else {
      stopNoise();
      stopMotors();
    }
  }
}

void makeNoise(int freq) {
  tone(speakerPin, freq);
}

void stopNoise() {
  noTone(speakerPin);
}

// 前進（両輪同じ方向に回転）
void moveMotorsForward() {
  motor1.write(0);   // 左モーター: 時計回り（前進）
  motor2.write(180); // 右モーター: 反時計回り（前進）
}



// モーター停止
void stopMotors() {
  motor1.write(90);  // 停止
  motor2.write(90);  // 停止
}

void stopAll() {
  stopNoise();
  stopMotors();
}