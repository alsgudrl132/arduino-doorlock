#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int rowPins[4] = {A0, A1, A2, A3};  // R1~R4
const int colPins[4] = {A4, A5, 2, 3};    // C1~C4
const int buzzerPin = 4;
int count = 0;

char keymap[4][4] = {
  { '1', '2', '3', 0   },
  { '4', '5', '6', 0   },
  { '7', '8', '9', 0   },
  {  0 , '0',  0 , 0   }
};

void setup() {
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("press, password");
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void loop() {
  bool keyPressed = false;

  for (int c = 0; c < 4; c++) {
    digitalWrite(colPins[c], LOW);
    for (int r = 0; r < 4; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        char key = keymap[r][c];
        if (key >= '0' && key <= '9') {
          if(count <= 4) {
            Serial.print("count =");
            Serial.println(count);
            Serial.println(key);
            tone(buzzerPin, 262);
            delay(200);
            noTone(buzzerPin);
            switch (count) {
              case 0 : lcd.setCursor(0, 1); lcd.print("    _ _ _ _"); break;
              case 1 : lcd.setCursor(0, 1); lcd.print("    * _ _ _"); break;
              case 2 : lcd.setCursor(0, 1); lcd.print("    * * _ _"); break;
              case 3 : lcd.setCursor(0, 1); lcd.print("    * * * _"); break;
              case 4 : lcd.setCursor(0, 1); lcd.print("    * * * *"); break;
            }
            keyPressed = true;
            delay(100);  // 디바운스
            count++;
          }
          else {
            count = 0;
          }
          
        }
      }
    }
    digitalWrite(colPins[c], HIGH);
  }

  if (!keyPressed) {
    noTone(buzzerPin);
  }
}
