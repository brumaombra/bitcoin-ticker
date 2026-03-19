#ifndef MATRIX_H
#define MATRIX_H

#include "../config/config.h"

// Temporary switch for firmware tests without the LED matrix connected.
#define LED_MATRIX_ENABLED 0

void printOnLedMatrix(const char* message, const byte stringLength, uint16_t messageStill = scrollPause);
void setMatrixIntensity(uint8_t intensity);
void setMatrixSpeed(uint8_t speed);
void setupLedMatrix();
void manageLedMatrix();

#endif