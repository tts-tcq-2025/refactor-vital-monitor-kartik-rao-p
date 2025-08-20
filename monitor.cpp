#include "./monitor.h" 
#include <assert.h>    
#include <stdio.h>     
#include <string.h>    
#include <time.h>      
#include <unistd.h>    

void sleep_for_seconds(int seconds) {
    usleep(seconds * 1000000); 
}

void displayWarning(const char* message) {
    printf("%s\n", message);
    for (int i = 0; i < 6; i++) {
        printf("\r* ");
        fflush(stdout);
        sleep_for_seconds(1);
        printf("\r *");
        fflush(stdout);
        sleep_for_seconds(1);
    }
    printf("\n");
}

// Helper function for condition checking
int checkAndWarn(int condition, const char* message) {
    if (condition) {
        displayWarning(message);
        return 0;
    }
    return 1;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (!checkAndWarn(temperature > 102.0f || temperature < 95.0f, "Temperature is critical!"))
        return 0;

    if (!checkAndWarn(pulseRate < 60.0f || pulseRate > 100.0f, "Pulse Rate is out of range!"))
        return 0;

    if (!checkAndWarn(spo2 < 90.0f, "Oxygen Saturation out of range!"))
        return 0;

    return 1;
}

void testVitalsOk() {
    printf("Running tests...\n");

    printf("Test Case 1: Critical temperature (high) - Expected 0\n");
    assert(vitalsOk(103.0f, 70.0f, 95.0f) == 0);

    printf("Test Case 2: Critical temperature (low) - Expected 0\n");
    assert(vitalsOk(94.0f, 70.0f, 95.0f) == 0);

    printf("Test Case 3: Low pulse rate - Expected 0\n");
    assert(vitalsOk(98.0f, 50.0f, 95.0f) == 0);

    printf("Test Case 4: High pulse rate - Expected 0\n");
    assert(vitalsOk(98.0f, 110.0f, 95.0f) == 0);

    printf("Test Case 5: Low oxygen saturation - Expected 0\n");
    assert(vitalsOk(98.0f, 70.0f, 85.0f) == 0);

    printf("Test Case 6: All vitals okay - Expected 1\n");
    assert(vitalsOk(98.0f, 70.0f, 95.0f) == 1);

    printf("All tests passed!\n");
}

int main() {
    testVitalsOk();
    return 0;
}
