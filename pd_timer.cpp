#include <Servo.h>

// --- ピンの定義 ---
const int lightSensorPin = A0; // 光センサーのピン
const int speakerPin = 3;      // スピーカーのピン
const int buttonPin = 2;       // 緊急停止ボタンのピン

// --- サーボモーターのオブジェクトを1つだけ宣言 ---
Servo motor; 

// --- 停止状態を管理するフラグ ---
bool stopped = false;

// --- 音の点滅（ビープ音）を管理するための変数 ---
unsigned long lastBeepTime = 0;   // 最後に音の状態を変えた時刻を記録
bool isBeeping = false;           // 現在、音が鳴っているかどうかの状態
const long beepInterval = 300;    // 音のリズム（0.3秒間隔）

void setup()
{
  // --- 各ピンのモードを設定 ---
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // --- モーターをピン8に接続 ---
  motor.attach(8);

  // シリアル通信を開始（デバッグ用）
  Serial.begin(9600);
}

void loop()
{
  // ボタンが押されたら、全ての動作を止める
  if (digitalRead(buttonPin) == LOW)
  {
    stopAll();
    stopped = true;
  }

  // 停止フラグが立っていない場合のみ、以下の処理を実行
  if (!stopped)
  {
    // 光センサーの値を読み取る
    int lightValue = analogRead(lightSensorPin);
    Serial.println(lightValue);

    // もし光センサーの値が500未満（暗い）なら
    if (lightValue < 500)
    {
      // モーターを動かし続ける
      moveMotor();

      // 音を点滅させる処理
      unsigned long currentTime = millis();
      if (currentTime - lastBeepTime >= beepInterval) {
        lastBeepTime = currentTime; 

        isBeeping = !isBeeping;

        if (isBeeping) {
          makeNoise(1000); // 音を鳴らす
        } else {
          stopNoise();   // 音を消す
        }
      }
    }
    // そうでなければ（明るいなら）
    else
    {
      // 全ての動作を停止させ続ける
      stopAll();
    }
  }
}

// --- 関数定義 ---

void makeNoise(int freq)
{
  tone(speakerPin, freq);
}

void stopNoise()
{
  noTone(speakerPin);
}

// モーターを前進させる関数
void moveMotor()
{
  motor.write(0);   // モーターを回転させる（0または180で回転方向を選ぶ）
}

// モーターを停止させる関数
void stopMotor()
{
  motor.write(90); // モーターを停止
}

// 全ての動作（音とモーター）を停止させる関数
void stopAll()
{
  stopNoise();
  stopMotor();
  isBeeping = false;
}