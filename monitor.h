#pragma once

#ifndef MONITOR_H
#define MONITOR_H

#include <stdbool.h>

// Structure to hold vital check parameters
typedef struct {
    const char* name;
    float value;
    float min;
    float max;
} VitalCheck;

// Declare the alert callback function type
typedef void (*AlertCallback)(const char*);

// Function prototypes
void PrintAlertMessage(const char* message);
bool checkVital(const VitalCheck* vital, AlertCallback alert);
bool areAllVitalsNormal(float temperature, float pulseRate, float spo2, AlertCallback alert);

#endif

