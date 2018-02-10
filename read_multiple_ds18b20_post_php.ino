#include <ESP8266WiFi.h>
#include <OneWire.h>
#include<DallasTemperature.h>
#include <WiFiRestClient.h>

#define ONE_WIRE_BUS D3 // DS18B20 on arduino pin4 corresponds to D3 on physical board
#define SLEEP_LENGTH 300

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float temp_0;
float temp_1;
float temp_2;

/* WiFi credentials */
const char* host = "192.168.0.39";
const char* route = "/ground.php?";
char ssid[] = "YOUR SSID";
char pass[] = "YOUR PASSWORD";

char strTD[6];

void setup() 
{
  {
  Serial.begin(115200);
  DS18B20.begin();
  Serial.println(" ");
  Serial.println("Testing Dual Sensor data");

  DS18B20.requestTemperatures(); 
  temp_0 = DS18B20.getTempCByIndex(0); // Sensor will capture Temp in Celcius
  temp_1 = DS18B20.getTempCByIndex(1); 
  temp_2 = DS18B20.getTempCByIndex(2);
  delay(5000);

/////
 Serial.print("Connecting to ");
 Serial.println(ssid);

WiFi.begin(ssid, pass);
delay(1000);
Serial.print(".");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

delay(1000);

  Serial.println();
  WiFiRestClient restClient( host );

  Serial.print( "Posting to http://" );
  Serial.print( host );
  Serial.println( route );
  //ende neu
  
  // Create some strings
  char temp0Stg[10];
  char temp1Stg[10];
  char temp2Stg[10];

// Convert the floats to strings
  dtostrf(temp_0, 5, 2, temp0Stg);
  dtostrf(temp_1, 5, 2, temp1Stg);
  dtostrf(temp_2, 5, 2, temp2Stg);

  char postStg[80]; 
  char buf[256];
  sprintf(buf, "%s,%s,%s", temp0Stg, temp1Stg, temp2Stg);  //buf 
  Serial.print( buf );
  Serial.println( " " );
  
  int statusCode = restClient.post( route, buf);  //buf

  Serial.print ("Status received: " );
  Serial.println( statusCode );
  
}
  

  Serial.println("Going into deep sleep for 5 Minutes");
  delay(100);
  ESP.deepSleep(SLEEP_LENGTH * 1000000,WAKE_RF_DEFAULT); //5 minutes

}

void loop() 
{

}
