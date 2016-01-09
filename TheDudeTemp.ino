
#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

#include "TheDudeTemp.h"

//Globals
WiFiClient client;
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);
Adafruit_IO_Feed tempFeed = aio.getFeed(FEED_NAME);
unsigned long lastTimeTemp = 0;
double realTemp = 0;
int errorFlag = 0;
int errorWiFiFlag = 0;



//######################################### Setup #########################################
void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WifiConnect();
  aio.begin();
  delay(200);
}

//######################################### Main Loop #########################################
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WifiConnect();
    errorFlag++;
  }
  else {

    if (millis() - lastTimeTemp > 5000) {
      realTemp = getTempFromAnalogValue( analogRead(A0) );
      lastTimeTemp = millis();
      delay(10);
      char buffer[5];
      //snprintf(buffer, sizeof(buffer), "%f", realTemp);
      Double2String(realTemp, buffer);
      if (tempFeed.send(buffer)) {
        DEBUGGING_L(F("Wrote value to feed: "));
        DEBUGGING(buffer);
        DEBUGGING("Going to sleep");
        ESP.deepSleep(1000 * 1000 * SLEEP_SECONDS);
        DEBUGGING("Shouldnt be here!");
      }
      else {
        DEBUGGING(F("Error writing value to feed!"));
         errorFlag++;
      }
      delay(10);
    }
  }
   if (errorFlag >= MAX_ERRORS) 
   {
    ESP.deepSleep(1000 * 1000 * SLEEP_SECONDS);
    DEBUGGING("Shouldnt be here!");
   }
}





//######################################### Wifi Connection #########################################
void WifiConnect() {

  // WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID_ME, PW_ME);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    errorWiFiFlag++;
    if (errorWiFiFlag >= WIFI_TRIES_SEC)
    {
      DEBUGGING("NOT Connected!");
      return;
    }
  }

  DEBUGGING("Connected");
  DEBUGGING(WiFi.localIP());
  errorFlag=0;

}




//######################################### Convert analog value (ADC) to Temperature #########################################
double getTempFromAnalogValue(int AnaVal) {

  double voltVal = ((double)AnaVal / 1024.0) * ADC_MAX_VAL;
  double thermVal = (VCC / voltVal - 1) * SEC_R;
  double Temp;

  int iii = 1;
  delay(0);

  while ( R_LUT[iii] > thermVal && iii < 13 ) {
    iii++;
  }
  double R1, R2, T1, T2;
  R1 = (double)R_LUT[iii - 1];
  R2 = (double)R_LUT[iii];
  T1 = (double)T_LUT[iii - 1];
  T2 = (double)T_LUT[iii];
  Temp = DEGREES_CORRECTION + (((double)thermVal - R2) / (R1 - R2) ) * ( T1 - T2 ) + T2;

  delay(0);

  DEBUGGING_L("sensor = ");
  DEBUGGING_L(AnaVal);
  DEBUGGING_L("  volt = ");
  DEBUGGING_L(voltVal);
  DEBUGGING_L("  term_R = ");
  DEBUGGING_L(thermVal);
  DEBUGGING_L("  RealTemp = ");
  DEBUGGING(Temp);

  return Temp;

  /* double a,b,c;
    double Beta = 4000;
    double R0 = 10000;
    Temp = (1 / ( 1/298.15 + (1/Beta)*log(R/R0)  ) ) -273.15;
  */
  /* a = (1/298.15)-(1/Beta)*log(R0); //0.001129148;
    b = 1/Beta; //0.000234125;
    c = 0;//0.0000000876741;
    Temp = log(R);
    Temp = 1 / (a + (b * Temp) + (c * Temp * Temp * Temp));
    Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  */

}


//######################################### Convert double (<100) to xx.xx string #########################################
void Double2String(double val, char buffer[5])
{
  val = val / 10;
  buffer[0] = 48 + (char)floor(val);
  val = 10 * (val - ( floor(val)));
  buffer[1] = 48 + (char)floor(val);
  buffer[2] = '.';
  val = 10 * (val - ( floor(val)));
  buffer[3] = 48 + (char)floor(val);
  val = 10 * (val - ( floor(val)));
  buffer[4] = 48 + (char)floor(val);
}




