#include <LiquidCrystal_I2C.h>


// the GPIO pin of the pulse input
const int input = 14;

// the current voltage at the pulse input
int voltage = 0;

// the voltage at the pulse input at the previous loop
int lastVoltage = 0;

// the last voltage-up-transition occured at (milliseconds after boot)
int upAt = 0;

// the last voltage-down-transition occured at (milliseconds after boot)
int downAt = 0;

// the voltage has been low for (milliseconds)
int downFor = 0;

// the voltage has been high for (milliseconds)
int upFor = 0;

// the number of pulses within this digit
int pulses = 0;

// set to high when the voltage has been high for long enough for this to count as a pulse
int hasBeenUp = 1;

// the last received digit
int digit = 0;

// stop the blinking of the backlight at this time (milliseconds after boot)
int stopBlinkAt = 0;

// the display is 4x20 characters and these 4 lines hold the content of it
String displayLines[] = {"", "", "", ""};

// the last digit was registered at this time (milliseconds after boot)
int lastDigitRegisteredAt = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup () {

  // set input to input
  pinMode(input, INPUT);

  // set the built in LED to output
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  // init the lcd
  lcd.init();

  // Turn on the lcd backlight.
  lcd.backlight();

  // write OK to display
  lcd.setCursor(0, 0);  lcd.print("OK");

}


void loop () {
  // read the voltage on the line
  voltage =   digitalRead(input);

  // turn off built in LED and turn on backlight if they have blinked for long enough
  if (millis() > stopBlinkAt) {
    lcd.backlight();
    digitalWrite(LED_BUILTIN, 1);
  }


  // if voltage has changed since last loop
  if (voltage != lastVoltage) {
    // did voltage go up or down
    if (voltage == 1) {

      // it went up
      Serial.println("volt up");

      // register when voltage went up - that's for measuring high timw
      upAt = millis();

      // calulate low time
      downFor = upAt - downAt;
      Serial.println("down time : " + String(downFor));

      // Low-pulses must be 5-150 ms to count as a pulse. They are typically 40-42ms.
      if (downFor > 5 && downFor < 150 && hasBeenUp == 1) {

        // increment pulse counter
        pulses ++;

        // this is the flag that registers if the voltage has been high for long enough for the next low voltage to count as a pulse
        hasBeenUp = 0;
        Serial.println("pulses : " + String(pulses));

      }

    } else {
      // voltage went down
      Serial.println("volt down");

      // register when voltage went down - that's for measuring low time
      downAt = millis();

      // calculate high time
      upFor = downAt - upAt;
      Serial.println("up time : " + String(upFor));

      // High-pulses must be 12-30 ms for the next down-pulse to count. They are typically 20-22 ms.
      if (upFor > 12 && upFor < 30) {
        hasBeenUp = 1;
      }

    }

  }

  // register voltage for cmaparing with it during the next loop
  lastVoltage = voltage;

  if (voltage == 1) {

    // voltage is high - if it has been for long enough and pulses have been counted, it means the end of an incoming digit
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

      digitalWrite(LED_BUILTIN, 0);
      lcd.noBacklight();

      stopBlinkAt = millis() + 200;

      lcd.setCursor(0, 3);  lcd.print(displayLines[0]);

      lastDigitRegisteredAt = millis();


      pulses = 0;
      hasBeenUp = 1;

    }
  }
}
