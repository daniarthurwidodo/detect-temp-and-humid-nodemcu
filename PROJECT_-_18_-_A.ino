#include <ESP8266WiFi.h>
// #include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
int lcdColumns = 16;
int lcdRows = 2;
// LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

const char *ssid = "HOTSPOT"; // ssid wifi
const char *password = "duaduadua"; // isikan password wifi

float h = 0, t = 0;
long previousMillis = 0;
long interval = 2000;
void setup()
{
  Serial.begin(115200);

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Kirim data sensor");

  lcd.setCursor(0, 1);
  lcd.print("ke server ...");

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ssid);
  Serial.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP()); // IP address assigned to your ESP
  delay(1000);
}

void loop()
{
  WiFiClient client;
  HTTPClient http;

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  String ID, TEMP_SENSOR, HUM_SENSOR, postData;
  // gudangRetail gudangDistributor gudangPetani
  ID = "gudangPetani";
  TEMP_SENSOR = String(t);
  HUM_SENSOR = String(h);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11, 1);
  lcd.print(h);

  //  Post Data
  //  postData = "id_alat_a=" + ID + "&level_temp_a=" + TEMP_SENSOR + "&level_hum_a=" + HUM_SENSOR;
  //  Web Server address to read/write from
  //  http://localhost:4001/monitor/tambah/01/33/75
  //  http://103.23.199.113:4001/monitor/add/01/33/75
  // http://103.23.199.113:4001/monitor/tambah/01/33/75
  String serverPath = "https://103.49.238.7:8082/monitor/tambah/" + ID + "/" + TEMP_SENSOR + "/" + HUM_SENSOR; // api
  http.begin(client, serverPath.c_str());                                                                      // change to string
  //  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();
  //  int httpCode = http.POST(postData);
  String payload = http.getString();
  Serial.println(serverPath);
  delay(5000);
  http.end();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11, 1);
  lcd.print(h);
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatur: ");
  lcd.setCursor(11, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(11, 1);
  lcd.print(h);
  lcd.clear();
}