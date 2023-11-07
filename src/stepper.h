#include <Arduino.h>
void stepper_begine();
void stepper_run();
void stepper_move(bool dir, int time_ms);
bool stepp_moving();