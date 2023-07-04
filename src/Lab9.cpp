/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Rich/CTD2023/Lab9/src/Lab9.ino"
// secrets
#include "env.h"
// libraries
#include <Wire.h>
#include "oled-wing-adafruit.h"
#include "blynk.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
// analog pins input
void setup();
void loop();
void callback();
void resetDisplay();
void checkButtons();
void logInEvent();
void logOutEvent();
void checkRange();
void flashLEDs();
#line 9 "c:/Users/Rich/CTD2023/Lab9/src/Lab9.ino"
#define POTIENT_INPUT A4
#define TEMP_INPUT A3
// digital input
#define BUTTON_SENSE D1
// digital output for LEDs
#define LED_LOWER D5
#define LED_MEDIUM D6
#define LED_HIGHER D7
// virtual output pins for blynk
#define NOTIFY_EVENT_CODE "notify"
#define ENTER_MSG "Range has been entered!"
#define LEAVE_MSG "Range has been left!"
#define SEND_CELSIUS V0
#define SEND_FAHRENHEIT V1
#define VIRTUAL_BUTTON_PIN V2

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

OledWingAdafruit display;
VCNL4040 proximitySensor;
Timer coolThingTimer(10, callback, true);
// internal variables
bool modeAorB = false; // A mode(light) is false, B Mode(temp) is true
bool mediumRangeEntered;
bool timerFuncRunning = false;
bool isSettingRange = true;
bool onLowOrHighSet = false; // false is low, true is high
uint16_t lowRangeValue;
uint16_t highRangeValue;

void setup()
{
  // setup blynk, serial, and display
  // Blynk.begin(BLYNK_AUTH_TOKEN);
  Serial.begin(9600);
  display.setup();
  resetDisplay();
  // Join i2c bus
  Wire.begin();
  // initialize sensor
  proximitySensor.begin();
  // pin modes for all in digital and analog
  pinMode(POTIENT_INPUT, INPUT);
  pinMode(TEMP_INPUT, INPUT);
  pinMode(BUTTON_SENSE, INPUT);
  pinMode(LED_LOWER, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_HIGHER, OUTPUT);
  // turn on proximity sensor channels
  proximitySensor.powerOnAmbient();
  proximitySensor.enableWhiteChannel();
}
void loop()
{
  // callback();
  // Blynk.run();
  display.loop();
  resetDisplay();
  // if (VIRTUAL_BUTTON_PIN){
  //   coolThingTimer.start();
  // }
  while (timerFuncRunning)
  {
    display.println("Please Wait, you impatient flounder.");
    display.display();
  }

  if (!modeAorB)
  {
    if (isSettingRange)
    {
      if (!onLowOrHighSet)
      {
        lowRangeValue = analogRead(POTIENT_INPUT);
        display.println("Use C to set low:");
        display.println(lowRangeValue);
        display.display();
        if (display.pressedC())
        {
          onLowOrHighSet = !onLowOrHighSet;
        }
      }
      else
      {
        highRangeValue = analogRead(POTIENT_INPUT);
        display.println("Use C to set high:");
        display.println(highRangeValue);
        display.display();
        if (display.pressedC())
        {
          isSettingRange = false;
        }
      }
    }
    else
    {
      display.println("Light Level:");
      display.println(proximitySensor.getAmbient());
      display.display();
      flashLEDs();
    }
  }
  else
  {
    int reading = analogRead(TEMP_INPUT);
    double voltage = (reading * 3.3) / 4095.0;
    double temperature = (voltage - 0.5) * 100;
    int fahrenheit = temperature * 1.8 + 32;
    int celsius = temperature;
    display.println("Current temperature: \n");
    display.printf("%u C \n", celsius);
    display.printf("%u F", fahrenheit);
    display.display();
    // Blynk.virtualWrite(SEND_CELSIUS, celsius);
    // Blynk.virtualWrite(SEND_FAHRENHEIT, fahrenheit);
  }
}
void callback()
{
  timerFuncRunning = true;
  digitalWrite(LED_LOWER, HIGH);
  digitalWrite(LED_MEDIUM, HIGH);
  digitalWrite(LED_HIGHER, HIGH);
  delay(300);
  digitalWrite(LED_LOWER, LOW);
  digitalWrite(LED_MEDIUM, LOW);
  digitalWrite(LED_HIGHER, LOW);
  delay(300);
  digitalWrite(LED_LOWER, HIGH);
  delay(300);
  digitalWrite(LED_MEDIUM, HIGH);
  digitalWrite(LED_LOWER, LOW);
  delay(300);
  digitalWrite(LED_MEDIUM, LOW);
  digitalWrite(LED_HIGHER, HIGH);
  delay(300);
  digitalWrite(LED_HIGHER, LOW);
  delay(300);
  digitalWrite(LED_LOWER, HIGH);
  digitalWrite(LED_MEDIUM, HIGH);
  digitalWrite(LED_HIGHER, HIGH);
  delay(300);
  digitalWrite(LED_LOWER, LOW);
  digitalWrite(LED_MEDIUM, LOW);
  digitalWrite(LED_HIGHER, LOW);
  timerFuncRunning = false;
}
void resetDisplay()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // display.display();
}
void checkButtons()
{
  if (display.pressedA())
  {
    modeAorB = false;
  }
  if (display.pressedB())
  {
    modeAorB = true;
  }
}
void logInEvent(){
  // Blynk.logEvent(NOTIFY_EVENT_CODE, ENTER_MSG);
  resetDisplay();
  display.println("Range Entered");
  display.display();
}
void logOutEvent(){
  // Blynk.logEvent(NOTIFY_EVENT_CODE, LEAVE_MSG);
}
// BLYNK_WRITE(SEND_CELSIUS)
// {
//     // Serial.println("Button Tapped");
//     if (param.asInt() == 1)
//     {
//         Blynk.logEvent(NOTIFY_EVENT_CODE, ENTER_MSG);
//     }
// }
// BLYNK_WRITE(SEND_FAHRENHEIT)
// {
//     // Serial.println("Button Tapped");
//     if (param.asInt() == 1)
//     {
//         Blynk.logEvent(NOTIFY_EVENT_CODE, LEAVE_MSG);
//     }
// }
BLYNK_READ(VIRTUAL_BUTTON_PIN){

}
void checkRange(){

}
void flashLEDs(){
  if (proximitySensor.getAmbient() <= lowRangeValue){
    digitalWrite(LED_LOWER, HIGH);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_HIGHER, LOW);
    if (mediumRangeEntered){
      //sendNotification Exit
      mediumRangeEntered = false;
    }else{

    }
  }
  else if (proximitySensor.getAmbient() <= highRangeValue){
    digitalWrite(LED_LOWER, LOW);
    digitalWrite(LED_MEDIUM, HIGH);
    digitalWrite(LED_HIGHER, LOW);
    if(!mediumRangeEntered){
      //send notification Enter
      mediumRangeEntered = true;
    }else{

    }
  }
  else if (proximitySensor.getAmbient() <= highRangeValue){
    digitalWrite(LED_LOWER, LOW);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_HIGHER, HIGH);
    if(mediumRangeEntered){
      //send Notification Exit
      mediumRangeEntered = false;
    }
  }
}