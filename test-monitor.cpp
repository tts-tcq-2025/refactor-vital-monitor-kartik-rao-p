#include "gtest/gtest.h"

// We need to include the C header with extern "C"
// to ensure the C functions are linked correctly.
extern "C" {
#include "monitor.h"
}

#include <string>
#include <vector>
#include <iostream> // For debugging purposes in test failures

// A mock alert callback to capture messages for testing
std::vector<std::string> captured_alerts;
void MockAlertCallback(const char* message) {
    captured_alerts.push_back(message);
}

// A callback that does nothing, for cases where we don't expect an alert
void NullAlertCallback(const char* message) {
    // Do nothing
}

// Helper to clear captured alerts before each test
void ClearAlerts() {
    captured_alerts.clear();
}

// Test fixture for monitor functions
class MonitorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear alerts before each test case
        ClearAlerts();
    }
};

// Test suite for checkVital function
TEST_F(MonitorTest, CheckVital_NormalValue) {
    VitalCheck vital = {"Temperature", 98.6, 95.0, 102.0};
    ASSERT_TRUE(checkVital(&vital, MockAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}

TEST_F(MonitorTest, CheckVital_BelowMin) {
    VitalCheck vital = {"Temperature", 94.0, 95.0, 102.0};
    ASSERT_FALSE(checkVital(&vital, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 1);
    ASSERT_EQ(captured_alerts[0], "Temperature is out of range!");
}

TEST_F(MonitorTest, CheckVital_AboveMax) {
    VitalCheck vital = {"Pulse Rate", 105.0, 60.0, 100.0};
    ASSERT_FALSE(checkVital(&vital, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 1);
    ASSERT_EQ(captured_alerts[0], "Pulse Rate is out of range!");
}

TEST_F(MonitorTest, CheckVital_AtMinBoundary) {
    VitalCheck vital = {"Temperature", 95.0, 95.0, 102.0};
    ASSERT_TRUE(checkVital(&vital, MockAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}

TEST_F(MonitorTest, CheckVital_AtMaxBoundary) {
    VitalCheck vital = {"Temperature", 102.0, 95.0, 102.0};
    ASSERT_TRUE(checkVital(&vital, MockAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}

// Test suite for areAllVitalsNormal function
TEST_F(MonitorTest, AreAllVitalsNormal_AllNormal) {
    ASSERT_TRUE(areAllVitalsNormal(98.6, 75.0, 98.0, MockAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}

TEST_F(MonitorTest, AreAllVitalsNormal_TemperatureLow) {
    ASSERT_FALSE(areAllVitalsNormal(94.5, 75.0, 98.0, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 1);
    ASSERT_EQ(captured_alerts[0], "Temperature is out of range!");
}

TEST_F(MonitorTest, AreAllVitalsNormal_PulseRateHigh) {
    ASSERT_FALSE(areAllVitalsNormal(98.6, 101.0, 98.0, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 1);
    ASSERT_EQ(captured_alerts[0], "Pulse Rate is out of range!");
}

TEST_F(MonitorTest, AreAllVitalsNormal_Spo2Low) {
    ASSERT_FALSE(areAllVitalsNormal(98.6, 75.0, 89.0, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 1);
    ASSERT_EQ(captured_alerts[0], "Oxygen Saturation is out of range!");
}

TEST_F(MonitorTest, AreAllVitalsNormal_MultipleVitalsAbnormal) {
    // Both temperature and pulse rate are out of range
    ASSERT_FALSE(areAllVitalsNormal(94.0, 101.0, 98.0, MockAlertCallback));
    ASSERT_EQ(captured_alerts.size(), 2);
    // The order of alerts depends on the loop in areAllVitalsNormal
    ASSERT_EQ(captured_alerts[0], "Temperature is out of range!");
    ASSERT_EQ(captured_alerts[1], "Pulse Rate is out of range!");
}

TEST_F(MonitorTest, AreAllVitalsNormal_NoAlertsWhenAllNormal) {
    ASSERT_TRUE(areAllVitalsNormal(98.6, 70.0, 95.0, NullAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}

TEST_F(MonitorTest, AreAllVitalsNormal_AlertsWhenAbnormalEvenWithNullCallback) {
    ASSERT_FALSE(areAllVitalsNormal(94.0, 70.0, 95.0, NullAlertCallback));
    ASSERT_TRUE(captured_alerts.empty());
}
