#include <Arduino.h>
#include <stepper.h>

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID            "TMPL6yRU2JxOI" 
#define BLYNK_TEMPLATE_NAME          "Fish Feeder"
#define BLYNK_AUTH_TOKEN             "2oZEri2JfLLGI_s5OhaAA4ph4nSZrS4P"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Kaisit 2.4G";
char pass[] = "0818808542";

bool blynk_button;
int blynk_distance;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  blynk_button = param.asInt();
}
BLYNK_WRITE(V1)
{
  blynk_distance = param.asFloat() * 1000;
}

void setup()
{
  Serial.begin(115200);
  stepper_begine();
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
    stepper_move(true, blynk_distance);
    blynk_moveOnce = true;
  }
  else if ((blynk_button == 0) && blynk_moveOnce)
  {
    blynk_moveOnce = false;
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
