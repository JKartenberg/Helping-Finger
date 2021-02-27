#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>

// define the GPIO connected with Relays and switches
#include <Servo.h>
Servo servoblau;

#define wifiLed   LED_BUILTIN   //D0
#define delay2 1000
#define servopin D3

//int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1

// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);

// WiFi Credentials
const char* ssid = "Starlink";
const char* password = "JaJoLaKa122";

// device names
String Device_1_Name = "MotorD1Mini";

boolean wifiConnected = false;

Espalexa espalexa;

//our callback functions
void firstLightChanged(uint8_t brightness)
{
  servoblau.write(brightness);
  Serial.println(brightness);
  //Control the device
  //if (brightness == 255)
  //  {
  //    digitalWrite(RelayPin1, LOW);
  //    Serial.println("Device1 ON");
  //    toggleState_1 = 1;
  //  }
  //else
  //{
  //  digitalWrite(RelayPin1, HIGH);
  //  Serial.println("Device1 OFF");
  //  toggleState_1 = 0;
  //}
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void addDevices(){
  // Define your devices here.
  espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off

  espalexa.begin();
}


void setup()
{
  Serial.begin(115200);

  servoblau.attach(servopin);

  pinMode(wifiLed, OUTPUT);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    addDevices();
  }
  else
  {
    Serial.println("Cannot connect to WiFi. So in Manual Mode");
    delay(1000);
  }
}

void loop()
{
   if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WiFi Not Connected ");
    Serial.println(wifiConnected);
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED    
  }
  else
  {
    //Serial.print("WiFi Connected  ");
    //Serial.println(wifiConnected);
    digitalWrite(wifiLed, LOW);

    //WiFi Control
    if (wifiConnected){
      espalexa.loop();
      delay(1);
    }
    else {
      wifiConnected = connectWifi(); // Initialise wifi connection
      if(wifiConnected){
      addDevices();
      }
    }
  }
}
