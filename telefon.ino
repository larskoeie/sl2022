#include <LiquidCrystal_I2C.h>


const int input = 15;

int voltage = 0;
int lastVoltage = 0;
int upAt = 0;
int downAt = 0;
int downFor = 0;
int upFor = 0;
int pulses = 0;
int hasBeenUp = 1;
int digit = 0;

String displayLines[] = {"", "", "", ""};
int lastDigitRegisteredAt = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup () {

  pinMode(input, INPUT);
  Serial.begin(9600);


  lcd.init();

  // Turn on the backlight.
  lcd.backlight();
}


void loop () {
  voltage =   digitalRead(input);



  if (voltage != lastVoltage) {
    if (voltage == 1) {

      Serial.println("volt up");
      upAt = millis();
      downFor = upAt - downAt;
      Serial.println("down time : " + String(downFor));

      // Low-pulses must be 5-150 ms to count as a pulse. They are typically 40-42ms.
      if (downFor > 5 && downFor < 150 && hasBeenUp == 1) {

        pulses ++;
        hasBeenUp = 0;
        Serial.println("pulses : " + String(pulses));

      }

    } else {
      Serial.println("volt down");

      downAt = millis();
      upFor = downAt - upAt;
      Serial.println("up time : " + String(upFor));

      // High-pulses must be 12-30 ms for the next down-pulse to count. They are typically 20-22 ms.
      if (upFor > 12 && upFor < 30) {
        hasBeenUp = 1;
      }

    }

  }

  lastVoltage = voltage;

  if (voltage == 1) {
    if (millis() - upAt > 100 && pulses > 0) {
      digit = pulses;
      if (digit == 10) {
        digit = 0;
      }
      Serial.println("Digit : " + String(digit));


      // If more than 5 seconds has passed, we'll interpret it as a new phone number is being dialed
      if (millis() - lastDigitRegisteredAt > 5000 && displayLines[0].length() > 0) {
        lcd.clear();
        displayLines[2] = displayLines[1];
        displayLines[1] = displayLines[0];
        displayLines[0] = "";

        lcd.setCursor(0, 1);  lcd.print(displayLines[2]);
        lcd.setCursor(0, 2);  lcd.print(displayLines[1]);
        lcd.setCursor(0, 3);  lcd.print(displayLines[0]);

      }

      displayLines[0] += String(digit);

      lcd.setCursor(0, 3);  lcd.print(displayLines[0]);

      lastDigitRegisteredAt = millis();


      pulses = 0;
      hasBeenUp = 1;

    }
  }
}
