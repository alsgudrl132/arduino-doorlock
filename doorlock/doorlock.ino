#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int rowPins[4] = {A0, A1, A2, A3};     // 키패드 행
const int colPins[4] = {6, 7, 2, 3};         // 키패드 열
const int buzzerPin = 4;

int accessKey[4] = {1, 5, 4, 1}; // 정답 비밀번호 (예)
int typeKey[4];                  // 입력 저장용
int index = 0;                   // 입력 인덱스
bool isAccess = false;
byte keymap[4][4] = {
  { 1,  2,  3,  0  },
  { 4,  5,  6,  0  },
  { 7,  8,  9,  0  },
  { 0,  0,  0, 10 }
};

void addKey(byte key) {
  if (index < 4 && key <= 9) { // 숫자만 저장
    typeKey[index++] = key;
  }
}

void setup() {
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("press password");
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void resetInput() {
  index = 0;
  lcd.setCursor(0, 1);
  lcd.print("    _ _ _ _    ");
  noTone(buzzerPin);
}

void printAsterisks() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    if(i == 0) {
      if (i < index) lcd.print("    * ");
      else lcd.print("    _ ");
    }
    else {
      if (i < index) lcd.print("* ");
      else lcd.print("_ ");
    }
    
  }
}

bool checkPassword() {
  for (int i = 0; i < 4; i++) {
    if (typeKey[i] != accessKey[i]) return false;
  }
  return true;
}

void loop() {
  bool keyPressed = false;

  for (int c = 0; c < 4; c++) {
    digitalWrite(colPins[c], LOW);
    for (int r = 0; r < 4; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        byte key = keymap[r][c];
        if (key <= 9) {
          if (index < 4) {
            tone(buzzerPin, 262);
            delay(200);
            noTone(buzzerPin);

            addKey(key);
            printAsterisks();

            keyPressed = true;
            Serial.println(key);

            delay(150);  // 디바운스
          }
        } 
        else if (key == 10) {  // 확인 버튼
          tone(buzzerPin, 500);
          delay(200);
          noTone(buzzerPin);

          if (index == 4) {
            Serial.println(typeKey[0]);
            Serial.println(typeKey[1]);
            Serial.println(typeKey[2]);
            Serial.println(typeKey[3]);
            if (checkPassword()) {
              lcd.setCursor(0, 1);
              lcd.print("Correct!      ");
              Serial.println("Password correct");
              isAccess = true;
            } else {
              lcd.setCursor(0, 1);
              lcd.print("Wrong!        ");
              Serial.println("Password wrong");
              isAccess = false;
            }
          } else {
            lcd.setCursor(0, 1);
            lcd.print("Incomplete!   ");
            Serial.println("Password incomplete");
          }

          delay(1000);
          resetInput();
          keyPressed = true;
        }
      }
    }
    digitalWrite(colPins[c], HIGH);
  }

  if (!keyPressed) {
    noTone(buzzerPin);
  }
}
