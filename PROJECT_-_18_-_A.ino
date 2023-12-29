#include <b64.h>
#include <HttpClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
int lcdColumns = 16;
int lcdRows = 2;
//LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

const char* ssid = "WARKOP3";   // ssid wifi
const char* password = "Ar456789#@123"; // isikan password wifi
//Web/Server address to read/write from
const char* host = "http://192.168.0.102:8000/api/get-data-sensor-A01";   // api
float h = 0, t = 0;
long previousMillis = 0;
long interval = 2000;
void setup() {
  Serial.begin(115200);

  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Send Data Sensor");

  lcd.setCursor(0, 1);
  lcd.print("TO SERVER.......");

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ssid);
  Serial.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  delay(1000);
}

void loop() {
   WiFiClient client;
  HTTPClient http;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String ID, TEMP_SENSOR, HUM_SENSOR, postData;
  ID = "A01";
  TEMP_SENSOR = String(t);
  HUM_SENSOR = String(h);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11  , 1);
  lcd.print(h);
  //Post Data
  postData = "id_alat_a=" + ID + "&level_temp_a=" + TEMP_SENSOR + "&level_hum_a=" + HUM_SENSOR;
  http.begin(client, host ); // need to check
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
  delay(5000);
  http.end();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11  , 1);
  lcd.print(h);
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11  , 1);
  lcd.print(h);
  lcd.clear();
}
