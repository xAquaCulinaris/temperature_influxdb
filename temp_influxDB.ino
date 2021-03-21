#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h>
#include "DHT.h"


#define DHTPIN 2
#define DHTTYPE DHT11
   
// Set WiFi credentials
char ssid[] =  "x";
char pass[] =  "x";

// Set influxd server ip
#define INFLUXDB_HOST "x"

DHT dht(DHTPIN, DHTTYPE);


//WiFiUDP udp;
Influxdb influx(INFLUXDB_HOST);

void setup() {
  Serial.begin(9600);
  dht.begin();

  delay(10);
 
  // Connect WiFi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.hostname("sensor1");
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  influx.setDb("room_temperature");
  Serial.println("selected room_temperature database");  
}


int loopCount = 0;

void loop() {
  delay(10000);

  
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.println(String("temp: " + String(temp)));
  Serial.println(String("hum: " + String(hum)));


  loopCount++;
  InfluxData row("marians_room");
  row.addTag("device", "DHT11");
  row.addTag("sensorID", "01");
  row.addTag("unit", "celsius");
  row.addValue("temp", temp);
  row.addValue("hum", hum);
  influx.write(row);
}
