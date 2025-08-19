#include "./monitor.h" 
#include <assert.h>    
#include <stdio.h>     
#include <string.h>    
#include <time.h>      
#include <unistd.h>    

// Function to simulate sleep for a given number of seconds
void sleep_for_seconds(int seconds) {
    // usleep takes microseconds
    usleep(seconds * 1000000); 
}

// Function to display a warning message and create a blinking effect
void displayWarning(const char* message) {
    printf("%s\n", message);
    for (int i = 0; i < 6; i++) {
        printf("\r* ");
        fflush(stdout); // Flush the output buffer to ensure immediate display
        sleep_for_seconds(1);
        printf("\r *");
        fflush(stdout);
        sleep_for_seconds(1);
    }
    printf("\n"); // Newline after the blinking
}

// Function to check if vital signs are okay
int vitalsOk(float temperature, float pulseRate, float spo2) {
    // Check temperature range
    if (temperature > 102.0f || temperature < 95.0f) {
        displayWarning("Temperature is critical!");
        return 0;
    }
    
    // Check pulse rate range
    if (pulseRate < 60.0f || pulseRate > 100.0f) {
        displayWarning("Pulse Rate is out of range!");
        return 0;
    }
    
    // Check oxygen saturation
    if (spo2 < 90.0f) {
        displayWarning("Oxygen Saturation out of range!");
        return 0;
    }
    
    // All vitals are okay
    return 1;
}

// Example test cases to cover all conditions
void testVitalsOk() {
    printf("Running tests...\n");

    // Test Case 1: Critical temperature (high)
    printf("Test Case 1: Critical temperature (high) - Expected 0\n");
    assert(vitalsOk(103.0f, 70.0f, 95.0f) == 0); 

    // Test Case 2: Critical temperature (low)
    printf("Test Case 2: Critical temperature (low) - Expected 0\n");
    assert(vitalsOk(94.0f, 70.0f, 95.0f) == 0);

    // Test Case 3: Low pulse rate
    printf("Test Case 3: Low pulse rate - Expected 0\n");
    assert(vitalsOk(98.0f, 50.0f, 95.0f) == 0);  

    // Test Case 4: High pulse rate
    printf("Test Case 4: High pulse rate - Expected 0\n");
    assert(vitalsOk(98.0f, 110.0f, 95.0f) == 0); 

    // Test Case 5: Low oxygen saturation
    printf("Test Case 5: Low oxygen saturation - Expected 0\n");
    assert(vitalsOk(98.0f, 70.0f, 85.0f) == 0);  

    // Test Case 6: All vitals okay
    printf("Test Case 6: All vitals okay - Expected 1\n");
    assert(vitalsOk(98.0f, 70.0f, 95.0f) == 1);  

    printf("All tests passed!\n");
}

int main() {
    // Run tests
    testVitalsOk();
    return 0;
}

