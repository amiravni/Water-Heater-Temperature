#define SSID_ME "YourSSID"
#define PW_ME "YourPW"
#define AIO_KEY    "ADAFRUIT KEY"
#define FEED_NAME "ADAFRUIT FEED"

#define ADC_MAX_VAL 1
#define VCC 3.3
#define SEC_R 1000
#define SLEEP_SECONDS 180
#define WIFI_TRIES_SEC 10
#define MAX_ERRORS 4
#define DEGREES_CORRECTION 4

#ifndef DEBUGGING
#define DEBUGGING(...)
#endif
#ifndef DEBUGGING_L
#define DEBUGGING_L(...)
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include "Adafruit_IO_Client.h"

// Look-up table - Resistance vs. Temperature
int T_LUT[14] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65 }; 
long R_LUT[14] = {35000, 27000, 20000, 15000, 12000, 10000, 8000, 6000, 5000, 4000, 3300, 2700, 2200, 1900};

// functions definitions
void WifiConnect();
void Double2String(double val, char buffer[5]);
double getTempFromAnalogValue(int AnaVal);
