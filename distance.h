#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"

#define CPAC (*((volatile uint32_t *)0xE000ED88))
#define pi 3.14159265358979323846
#define R  6371e3


float dist(float, float, float, float);