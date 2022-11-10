#include <DFRobot_MAX31855.h>

float getTemp() {
 DFRobot_MAX31855 max31855;
 float temp = max31855.readCelsius();
 return temp;
}
