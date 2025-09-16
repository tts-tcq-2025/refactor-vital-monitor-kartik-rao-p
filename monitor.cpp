#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "./monitor.h"

// Define a type for the alert callback function
typedef void (*AlertCallback)(const char*);

// Helper function to concatenate strings for alert messages
void createAlertMessage(char* buffer, size_t buffer_size, const char* vitalName, const char* message) {
    snprintf(buffer, buffer_size, "%s %s", vitalName, message);
}

void PrintAlertMessage(const char* message) {
    printf("%s\n", message);
    for (int i = 0; i < 6; ++i) {
        printf("\r* ");
        fflush(stdout);
        sleep(1);
        printf("\r *");
        fflush(stdout);
        sleep(1);
    }
}

bool checkVital(const VitalCheck* vital, AlertCallback alert) {
    if (vital->value < vital->min || vital->value > vital->max) {
        char alertMessage[256];
        createAlertMessage(alertMessage, sizeof(alertMessage), vital->name, "is out of range!");
        alert(alertMessage);
        return false;
    }
    return true;
}

bool areAllVitalsNormal(float temperature, float pulseRate, float spo2, AlertCallback alert) {
    const VitalCheck vitals[] = {
        {"Temperature", temperature, 95.0, 102.0},
        {"Pulse Rate", pulseRate, 60.0, 100.0},
        {"Oxygen Saturation", spo2, 90.0, 100.0}
    };

    bool allVitalsOk = true;
    for (int i = 0; i < 3; ++i) {
        allVitalsOk = checkVital(&vitals[i], alert) && allVitalsOk;
    }

    return allVitalsOk;
}
