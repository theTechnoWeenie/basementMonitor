#include <Wire.h>
#include <SPI.h>
#include <Adafruit_WINC1500.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Credentials.h"

//Temp/humid/pressure sensor
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//wifi
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);
Adafruit_WINC1500Client client;

// Credentials.h should look like this:
//
//const char ssid[] = "My Wifi Name";     //  your network SSID (name)
//const char pass[] = "totallySecretPassword";  // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "192.168.1.15";
#define SEND_PERIOD 15*60*1000/100 //works out to 9 seconds

void setup() {
  #ifdef WINC_EN
    pinMode(WINC_EN, OUTPUT);
    digitalWrite(WINC_EN, HIGH);
  #endif

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;
      delay(1000);
    }
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {

    Serial.println("===Sensor Data===");
    Serial.print("Temperature = ");
    int degC = bme.readTemperature();
    int degF = (int)((degC * 1.8 )) + 32;
    int pressure = bme.readPressure();
    int humidity = bme.readHumidity();
    Serial.print(degC);
    Serial.println(" *C");

    Serial.print("              ");
    Serial.print(degF);
    Serial.println(" *F");

    Serial.print("Pressure = ");

    Serial.print(pressure / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.println("======");
    Serial.println();

    sendData(degC, humidity, pressure);
    delay(SEND_PERIOD);
}

void sendData(int temp, int humidity, int pressure){
  if (client.connect(server, 8080)) {
    Serial.print("connected to server: ");
    Serial.println(server);
    // Make a HTTP request:
    client.print("GET ");
    client.print("/?temp=");
    client.print(temp);
    client.print("&humidity=");
    client.print(humidity);
    client.print("&pressure=");
    client.print(pressure);
    client.println(" HTTP/1.1");
    client.print("Host: sensor-bot"); client.println();
    client.println("Connection: close");
    client.println();
    client.flush();
  }

  Serial.println("<-----[rx]");
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  Serial.println("<-----");

  if (!client.connected()) {
    client.stop();
    Serial.println();
    Serial.println("closed connection");
  }
}
