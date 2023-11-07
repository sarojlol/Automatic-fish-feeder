#include <Arduino.h>
#include <pins_define.h>
#include <stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>

#define BLYNK_TEMPLATE_ID            "TMPL6yRU2JxOI" 
#define BLYNK_TEMPLATE_NAME          "Fish Feeder"
#define BLYNK_AUTH_TOKEN             "2oZEri2JfLLGI_s5OhaAA4ph4nSZrS4P"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Kaisit 2.4G";
char pass[] = "0818808542";

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
ESP32Encoder encoder;

bool blynk_button;
float blynk_rawMs = 2;
int blynk_ms = 2000;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  blynk_button = param.asInt();
}
BLYNK_WRITE(V1)
{
  blynk_rawMs = param.asFloat();
  lcd.setCursor(6, 1);
  lcd.print(blynk_rawMs);
}

void setup()
{
  pinMode(rotaryButton, INPUT_PULLUP);
  Serial.begin(serialBuadrate);
  stepper_begine();
  lcd.begin(16, 2, LCD_5x8DOTS);
  lcd.setCursor(4, 0);
  lcd.print("Automatic");
  lcd.setCursor(2, 1);
  lcd.print("Fish  Feeder");
  delay(2000);
  lcd.clear();
  lcd.home();
  lcd.print("Amount In Second");
  lcd.setCursor(6, 1);
  lcd.print(2.0);

  encoder.attachHalfQuad(rotaryDT, rotaryCLK);
  encoder.setCount ( 0 );

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
  stepper_run();
  static bool blynk_moveOnce;
  static int8_t last_position;
  if ((blynk_button == 1) &! blynk_moveOnce)
  {
    blynk_ms = blynk_rawMs*1000;
    stepper_move(true, blynk_ms);
    blynk_moveOnce = true;
  }
  else if ((blynk_button == 0) && blynk_moveOnce)
  {
    blynk_moveOnce = false;
  }
  int enc_pos = encoder.getCount()/2;
  static int last_encPos;
  static float enc_diff;
  if (enc_pos != last_encPos){
    enc_diff = enc_pos - last_encPos;
    enc_diff = enc_diff/10.0;
    blynk_rawMs  = blynk_rawMs + enc_diff;
    blynk_rawMs = constrain(blynk_rawMs, 0.5, 5.0);
    lcd.setCursor(6, 1);
    lcd.print(blynk_rawMs);
    last_encPos = enc_pos;
  }
  static float lastBlynk_rawMs;
  if (blynk_rawMs != lastBlynk_rawMs)
  {
    Blynk.virtualWrite(V1, blynk_rawMs);
    lastBlynk_rawMs = blynk_rawMs;
  }
  static unsigned long button_filter;
  static bool button_flag;
  bool button_state = digitalRead(rotaryButton);
  if ((millis() - button_filter) > 20)
  {
    if ((button_state == LOW) &! button_flag)
    {
      blynk_ms = blynk_rawMs*1000;
      stepper_move(true, blynk_ms);
      button_flag = true;
    }
    else if ((button_state == HIGH) && button_flag)
    {
      button_flag = false;
    }
    button_filter = millis();
  }

  bool isMoving = stepp_moving();
  static bool last_moving;
  if (isMoving != last_moving){
    if(isMoving){
      Blynk.virtualWrite(V2, 1);
    }
    else if(!isMoving){
      Blynk.virtualWrite(V2, 0);
    }
    last_moving = isMoving;
  }
}
