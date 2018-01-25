#include <ESP8266WiFi.h>
#include <OneWire.h>
#include<DallasTemperature.h>
#include <WiFiRestClient.h>

#define ONE_WIRE_BUS D3 // DS18B20 on arduino pin4 corresponds to D3 on physical board

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float temp_0;
float temp_1;

/* WiFi credentials */
const char* host = "192.168.0.39";
const char* route = "/ground.php?";
char ssid[] = "YOUR SSID";
char pass[] = "YOUR PASSWORD";

char strTD[6];

void setup() 
{
  Serial.begin(115200);
  DS18B20.begin();
  Serial.println(" ");
  Serial.println("Testing Dual Sensor data");
}

void loop() 
{
  getSendData();
}

/***************************************************
 * Send Sensor data to Server
 **************************************************/
void getSendData()
{
  DS18B20.requestTemperatures(); 
  temp_0 = DS18B20.getTempCByIndex(0); // Sensor 0 will capture Temp in Celcius
  temp_1 = DS18B20.getTempCByIndex(1); // Sensor 0 will capture Temp in Fahrenheit

//neu
  delay(5000);

if (WiFi.status() != WL_CONNECTED) {
WiFi.begin(ssid, pass);
delay(100);
}
if (WiFi.waitForConnectResult() != WL_CONNECTED) {
return;
}

if (WiFi.status() == WL_CONNECTED)
{
}

  Serial.println(" %");
  
  Serial.println();
  WiFiRestClient restClient( host );

  Serial.print( "Posting to http://" );
  Serial.print( host );
  Serial.println( route );
  //ende neu
  
  // Create some strings
char temp0Stg[10];
char temp1Stg[10];

// Convert the floats to strings
dtostrf(temp_0, 5, 2, temp0Stg);
dtostrf(temp_1, 5, 2, temp1Stg);

char postStg[80]; 

  char buf[256];
  //sprintf(postStg, "temp_pond=%s&temp_pool=%s", temp0Stg, temp1Stg);
  sprintf(buf, "temp_pond=%s&temp_pool=%s", temp0Stg, temp1Stg);  //buf 
  Serial.print( buf );
  Serial.println( " " );
  
  int statusCode = restClient.post( route, buf);  //buf

  Serial.print ("Status received: " );
  Serial.println( statusCode );
  
  Serial.println( "Waiting 30 seconds" );
  delay(30000);
