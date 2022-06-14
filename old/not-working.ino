

#include <LiquidCrystal_I2C.h>



// The GPIO number of the phone line input
const int input = 15;

// The current logic level of the phone line input
int voltage = 0;

// The previous logic level of the phone line input
int lastVoltage = 0;

// The clock (in milliseconds) when the last high pulse started
int pulseWentUpAt = 0;

// The clock (in milliseconds) when the last low pulse started
int pulseWentDownAt = 0;

// The number of pulses in the current pulse sequence
int pulseCounter = 0;

//
int pulseLowTime = 0;
int pulseHighTime = 0;
int digit = 0;
String displayLines[] = {"", ""};
int lastDigitRegisteredAt = 0;

int loops = 0;
int h = 0;

// Construct an LCD object and pass it the
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void registerDigit(int d) {

  // The digit "0" sends 10 pulses
  if (d == 10) {
    d = 0;
  }
  Serial.println("Digit : " + String(d));


  // Register time for this digit
  lastDigitRegisteredAt = millis();

  // Add the digit to the end of the bottom LCD line
  displayLines[0] += String(d);

  Serial.println("Number : " + displayLines[0]);


  // If more than 5 seconds has passed, we'll interpret it as a new phone number is being dialed
  if (millis() - lastDigitRegisteredAt > 5000 && displayLines[0].length() > 0) {
    lcd.clear();
    displayLines[2] = displayLines[1];
    displayLines[1] = displayLines[0];
    displayLines[0] = "";
  }


  lcd.setCursor(0, 1);  lcd.print(displayLines[2]);
  lcd.setCursor(0, 2);  lcd.print(displayLines[1]);
  lcd.setCursor(0, 3);  lcd.print(displayLines[0]);

}


void setup() {
  pinMode(input, INPUT);
  Serial.begin(9600);
  Serial.println("Starting ...");

  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  h = millis();
}


void loop() {
  // this is for measuring the speed of the program execution
  loops ++;
  if (millis() > h + 1000) {
    //    Serial.println(String(loops) + " loops pr. sek. (" + String(1000000 / loops) + " uS/loop)");
    loops = 0;
    h = millis();
  }

  //  lcd.setCursor(5, 0);  lcd.print(String(millis() / 1000) + " sek.");

  voltage =   digitalRead(input);
  //  lcd.setCursor(0, 0);  lcd.print(String(voltage));

  if (voltage != lastVoltage) {
    if (voltage == 1) {
      // voltage goes up
      Serial.println("Voltage up              -");
      pulseWentUpAt = millis();
      pulseLowTime = millis() - pulseWentDownAt;

      Serial.println("Pulse low time : " + String(pulseLowTime * 1000) + " mS  ");

      if (pulseLowTime > 2) {

        if (pulseLowTime < 150) {
          pulseCounter ++;
          Serial.println("Pulses : " + String(pulseCounter));
        }
      }
    } else {
      // voltage goes down
      Serial.println("Voltage down                   -");
      pulseWentDownAt = millis();

      pulseHighTime = millis() - pulseWentUpAt;
      Serial.println("Pulse high time : " + String(pulseHighTime * 1000) + "mS");

      if (pulseHighTime > 2) {


        if (pulseHighTime < 100) {
        }

      }
    }


  }

  lastVoltage = voltage;

  if (true) {


    if (voltage == 1) {
      pulseHighTime = millis() - pulseWentUpAt;

      if (pulseHighTime > 100 && pulseCounter > 0) {
        registerDigit(pulseCounter);
        pulseCounter = 0;
      }
    }
  }

}
