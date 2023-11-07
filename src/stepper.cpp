#include <Arduino.h>
#include <pins_define.h>

int pins[4] = {IN1, IN2, IN3, IN4};

void stepper_begine(){
    for(int i=0; i<4; i++){pinMode(pins[i], OUTPUT);}
}

int stepper_ms;
bool stepper_dir;
bool isMoving;
void stepper_move(bool dir, int time_ms){
    stepper_ms = time_ms/2;
    stepper_dir = dir;
}
void stepper_run(){
    static unsigned long last_stepDelay;
    static int step;
    if ((millis() - last_stepDelay) > 2){
        stepper_ms --;
        stepper_ms = max(stepper_ms, 0);
        if (stepper_dir){
            step ++;
            if (step >= 4){
                step = 0;
            }
        }
        else if (!stepper_dir){
            step --;
            if (step <= -1){
                step = 3;
            }
        }
        last_stepDelay = millis();
    }
    if (stepper_ms > 0){
        isMoving = true;
        switch (step)
            {
            case 0:
                digitalWrite(pins[0], HIGH);
                digitalWrite(pins[1], LOW);
                digitalWrite(pins[2], LOW);
                digitalWrite(pins[3], LOW);
                break;
            case 1:
                digitalWrite(pins[0], LOW);
                digitalWrite(pins[1], HIGH);
                digitalWrite(pins[2], LOW);
                digitalWrite(pins[3], LOW);
                break;
            case 2:
                digitalWrite(pins[0], LOW);
                digitalWrite(pins[1], LOW);
                digitalWrite(pins[2], HIGH);
                digitalWrite(pins[3], LOW);
                break;
            case 3:
                digitalWrite(pins[0], LOW);
                digitalWrite(pins[1], LOW);
                digitalWrite(pins[2], LOW);
                digitalWrite(pins[3], HIGH);
                break;
        }
    }
    else if (stepper_ms <= 0){
        isMoving = false;
        digitalWrite(pins[0], LOW);
        digitalWrite(pins[1], LOW);
        digitalWrite(pins[2], LOW);
        digitalWrite(pins[3], LOW);
    }
}

bool stepp_moving(){
    return isMoving;
}