// Required Libries
#include <Arduino.h>
#include <TM1637Display.h>

// ROTARY CODE PINOUTS
#define dt_pin 2
#define clk_pin 3
#define rotaryEncoderButton_pin 4
// 7 SEG DISPLAY PINOUTS
#define CLK A5
#define DIO A4

// Blank display data for 7 seg.
TM1637Display display(CLK, DIO);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

volatile int envoder_value = 50;
volatile int duty = 0;

unsigned long previousMillies = 0;
const long blinkInterval = 1000;
bool ledstate = true;

bool pause = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");

  pinMode(dt_pin, INPUT_PULLUP);
  pinMode(clk_pin, INPUT_PULLUP);
  pinMode(rotaryEncoderButton_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(clk_pin), speedChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(clk_pin), speedChange, CHANGE);

  // Set up the display. 
  display.setBrightness(0x0F);
  display.showNumberDecEx(envoder_value);
  speedChange();
}




void loop() {

  // put your main code here, to run repeatedly
    rotaryEncoderButton();
    pauseMode();
    PWMOUT();
}


void pauseMode() {
  // If pause is active then blink.

  if(!pause) {

    return;
  }

  if((millis() - previousMillies) < blinkInterval) {

    return;
  }
  previousMillies = millis();
    Serial.println("stage 2");


  if(ledstate) {

    display.showNumberDecEx(-envoder_value, 0x40);
    ledstate = false;
    Serial.println("stage 3");

  } else if(!ledstate){

    display.showNumberDecEx(envoder_value);
    ledstate = true;
    Serial.println("stage 4");
  }
}


void setHome() {


}

void PWMOUT() {
  display.showNumberDecEx(envoder_value);

  if(envoder_value == 100) {

    digitalWrite(9, LOW);
  } 
  else if(envoder_value == 0) {

    digitalWrite(9, HIGH);
  }
  else {

    analogWrite(9, (255-duty));
  }
}


void speedChange() {
  // Read the pot value when changed.
  // Update the PWM Speed of pump.

  if(pause) {
    // If pause is true then do nothing.

    return;
  }

  if((digitalRead(clk_pin) == digitalRead(dt_pin)) && (envoder_value != 100)) {

    envoder_value++;
  } 
  else if(digitalRead(clk_pin) != digitalRead(dt_pin) && (envoder_value != 0)) {
    envoder_value--;
  }

  duty = map(envoder_value, 0, 100, 0, 255);

  PWMOUT();
  }

  void rotaryEncoderButton() {
    // Reads the button on the encoder if pressed.
    int button = digitalRead(rotaryEncoderButton_pin);
    if(button == LOW)
    {

      envoder_value = 0;
      display.showNumberDecEx(envoder_value);
    }
  }
