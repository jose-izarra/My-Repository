/* PSEUDOCODE

//include all the library imports
// define the variables we will use, each corresponding to their own arduino pin
// initialize the arduino varibles in the setup

start loop 
  save the current value of the light sensor to a variable, and print it
  save the current value of the soil moisture sensor to a variable, and print it
  save the current time to a variable 

  if the current time is within 9:00h and 20:00h, run the following code  // there is daylight outside
    if the light sensor value is less than the threshold for the plant    // checks if the plant is receiving enough sunlight
      turn on the blue LED, while the red LED is off
      display a message on the screen that says "Good Lighting"

    else, the plant is not getting enough sunlight
      display a message on the screen to move the plant                   
      turn on the red LED, turn off the blue LED

  else, it is dark outside
    turn off the display screen


  // second if statement for the humidity that operates on its own

  if  the soil moisture sensor value is below the threshold, then the plant has enough water 
    send a signal to the relay to stay at 0V
  
  else, the plant is above the threshold, the plant needs water
    send a signal to the relay to turn on the water pump 

  add a delay to this loop, so it checks the conditions of the plant every x seconds
end loop 
*/



// library imports 
#include <LiquidCrystal.h>  // Liquid Crystal Display --> screen
#include <RTClib.h>
#include <Wire.h>

// defining variables
RTC_DS3231 rtc;
char t[32];
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // configuring the digital pins connected to the arduino board

int LDRinput = A0;                  // ldr stands for light controlled sensor
int blueLED = 8;                    // sets the blue LED to pin 3
int humiditySensorInput = A1;       // soil moisture sensor is connected to analog pin A1
int relayInput = 10;                // the input is given by the pin 10

void setup() {
   // starting the serial monitor
  Serial.begin(9600);  // starts the serial monitor

  // setting up LCD 
  lcd.begin(16,2); // first value is for gnd, second is for 5v (gnd,5V)

  // setting up the RTC 
  Wire.begin();
  rtc.begin();
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));          // run this code to set the current time from your computer
  rtc.adjust(DateTime(2023, 5, 25, 2, 0, 0));                 // run this code to set the date and time manually

  pinMode(LDRinput, INPUT); // defines the light sensor as input
  pinMode(blueLED, OUTPUT);  // defines blue LED as the output indicator
  pinMode(relayInput, OUTPUT); // defines the relay output to pin 10
}

void loop() {
  // Storing light sensor and printing its values
  int lightValue = analogRead(LDRinput); // reads the value of the light sensor and saves it to a variable
  Serial.print("Light sensor value: ");
  Serial.println(lightValue); // prints the value of the light sensor to the serial monitor

  // Storing soil moisture values and printing it
  int humidityValue = analogRead(humiditySensorInput);
  Serial.print("Soil moisture sensor value: ");
  Serial.println(humidityValue);
  Serial.println("");

  DateTime now = rtc.now();

  // firt if statement to check if it is daytime
  if (now.hour() > 9 && now.hour() < 20)  {
    // if statement for the light sensor values
    if (lightValue < 500) {
      digitalWrite(blueLED,HIGH);                             // if the plant is receiving good light the blue LED lights up
      lcd.print(" Good Lighting ");
    } else { 
      // otherwise, it needs more light and should me moved 
      digitalWrite(blueLED,LOW);                              // due to the not gate, the red LED turns on when we set the blue LED to low
      lcd.print(" Move the Plant! ");
    }
  } else { 
    // turn off the display screen
    lcd.noDisplay();
  }

  // if statement for the soil moisture sensor
  if (humidityValue < 900) {  // the higher the value the drier it is  // 900
    // if the moisture is above 900, then flower the plant
    digitalWrite(relayInput, LOW);
  } else {
    digitalWrite(relayInput, HIGH);
  }

  delay(1000);  // 1000ms = 1sec delay 
}
