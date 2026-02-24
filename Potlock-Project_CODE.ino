#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lockServo;

 // setting constant
const int POT_PIN   = A0;
const int BTN_PIN   = 2;

const int LED_G_PIN = 7;   
const int LED_R_PIN = 6;   

const int BUZ_PIN   = 10;
const int SERVO_PIN = 9;


const int CODE_LEN = 4;
int secretCode[CODE_LEN] = {0, 9, 0, 4}; 

const int SERVO_LOCKED_ANGLE   = 10;
const int SERVO_UNLOCKED_ANGLE = 90;

// Button debounce (created using ChatGPT)
const unsigned long DEBOUNCE_MS = 180;

enum State { ENTERING, UNLOCKED };
State state = ENTERING;

int entered[CODE_LEN];
int enteredCount = 0;

int lastLiveDigit = -1;

// Button edge detect + debounce
bool lastBtnReading = HIGH;
bool stableBtnState = HIGH;
unsigned long lastDebounceTime = 0;

// Sounds fuctions
void beepShort() {
  tone(BUZ_PIN, 2000, 60);
}

void beepError() {
  tone(BUZ_PIN, 350, 220);
}

void beepSuccess() {
  tone(BUZ_PIN, 1200, 120); delay(140);
  tone(BUZ_PIN, 1700, 140); delay(160);
  tone(BUZ_PIN, 2300, 160);
}

//Helper functions LED
void showLocked() {
  digitalWrite(LED_R_PIN, HIGH);
  digitalWrite(LED_G_PIN, LOW);
}

void showUnlocked() {
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, HIGH);
}

//potentiometer mapping
int potToDigit(int raw) {
  int d = raw / 100;    
  if (d > 9) d = 9;
  if (d < 0) d = 0;
  return d;
}

//LCD
void clearLine(int row) {
  lcd.setCursor(0, row);
  lcd.print("                ");
}
// Display Input (created using ChatGPT)
void showEnterScreen(int liveDigit) {
  clearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Enter Code!");

 
  lcd.setCursor(0, 1);
  lcd.print("PIN: ");

  for (int i = 0; i < CODE_LEN; i++) {
    if (i < enteredCount) lcd.print(entered[i]);
    else if (i == enteredCount) lcd.print(liveDigit);
    else lcd.print('_');
    lcd.print(' ');
  }

  // ensure no leftovers
  lcd.print("  ");
}

void showUnlockedScreen() {
  clearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Unlocked!");

  clearLine(1);
  lcd.setCursor(0, 1);
  lcd.print("Press to relock");
}

// Code check
bool isCodeCorrect() {
  for (int i = 0; i < CODE_LEN; i++) {
    if (entered[i] != secretCode[i]) return false;
  }
  return true;
}

// Lock Control
void lockChest() {
  lockServo.write(SERVO_LOCKED_ANGLE);
  state = ENTERING;
  enteredCount = 0;
  showLocked();

  int liveDigit = potToDigit(analogRead(POT_PIN));
  lastLiveDigit = liveDigit;
  showEnterScreen(liveDigit);
}

void unlockChest() {
  lockServo.write(SERVO_UNLOCKED_ANGLE);
  state = UNLOCKED;
  showUnlocked();
  showUnlockedScreen();
}

//button handling (Chat GPT)
bool buttonPressedEvent() {
  // Debounced falling-edge event (HIGH->LOW because INPUT_PULLUP)
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceTime = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableBtnState) {
      stableBtnState = reading;
      if (stableBtnState == LOW) {
        return true; // pressed
      }
    }
  }
  return false;
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(BUZ_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  lockServo.attach(SERVO_PIN);


  lockChest();
}

void loop() {
  // Live input updating (C)
  if (state == ENTERING) {
    int liveDigit = potToDigit(analogRead(POT_PIN));
    if (liveDigit != lastLiveDigit) {
      lastLiveDigit = liveDigit;
      showEnterScreen(liveDigit);
    }
    //sound enter
    if (buttonPressedEvent()) {
      beepShort();

      if (enteredCount < CODE_LEN) {
        entered[enteredCount] = liveDigit;
        enteredCount++;

        if (enteredCount >= CODE_LEN) {
          if (isCodeCorrect()) {
            beepSuccess();
            unlockChest();
          } else {
            beepError();
            showLocked();

            clearLine(0);
            lcd.setCursor(0, 0);
            lcd.print("Wrong Code!");
            clearLine(1);
            lcd.setCursor(0, 1);
            lcd.print("Try again...");
            delay(900);

            // Reset entry
            enteredCount = 0;
            showEnterScreen(liveDigit);
          }
        } else {
          showEnterScreen(liveDigit);
        }
      }
    }
  }

  else if (state == UNLOCKED) {
    if (buttonPressedEvent()) {
      beepShort();
      lockChest();
    }
  }
}
