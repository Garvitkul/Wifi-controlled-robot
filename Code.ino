#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define relay            4 // You can assign any pin of the NodeMCU

#define WLAN_SSID       "wifi"             // Your SSID
#define WLAN_PASS       "password"        // Your password

/********* Adafruit.io Setup ***********/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "username"            // Replace it with your username
#define AIO_KEY         "aio_key"   // Replace with your Project Auth Key

/**** Global State (you don't need to change this!) ******/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/********** Feeds *************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Smart_control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Smart_control"); // FeedName

void MQTT_connect();

void setup() {
  Serial.begin(9600);

  pinMode(relay, OUTPUT);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Smart_control);
}

void loop() {
 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Smart_control) {
      Serial.print(F("Got: "));
      Serial.println((char *)Smart_control.lastread);
    char *value = (char *)Smart_control.lastread;
    String message = String(value);
    message.trim();
    if (message == "ON") {digitalWrite(relay, LOW);}
    //if (message == "OFF") {digitalWrite(relay, HIGH);}
    else {digitalWrite(relay, HIGH);}
      //const char Smart_control_State = Smart_control.lastread);
     // if (Smart_control_State = "ON"){
     //   digitalWrite(relay, HIGH);
      //}
      }
      }
  }


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
