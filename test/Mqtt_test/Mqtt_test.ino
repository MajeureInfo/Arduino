
#include <PubSubClient.h>
#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "...";     // your network SSID (name)
char pwd[] = "...";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char host[] = "pure-island-76277.herokuapp.com";
char broker_url[] = "192.168.43.48"; //"m23.cloudmqtt.com";

const int buttonPin = 10;   
const int ledPin =  9;    // pin to write LED state
const int ledState = 8;   // pin to read LED state

int buttonState = 0;      // variable for reading the button status
int lastButtonState = 0;  // variable to remember last button state to detect rising edges
long lastSend = millis();

// Initialize the Wifi client library
WiFiEspClient espClient;

PubSubClient client(espClient);

void setup() {
  // initialize the pins
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledState, INPUT);
  
  //initialize the connections
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  client.setServer( broker_url, 1883 );
  client.setCallback(callback);
  
  // attempt to connect to WiFi network
  connect_to_wifi();

  // attempt to connect to cloudmqtt
  connect_to_broker();

  // connected
  printWifiStatus();
 
  delay(3000);
  Serial.println("*** Ready to start ***");
}

void loop() {
  if (millis() - lastSend > 1000) {
    // Maintain the network connection
    connect_to_wifi();
  
    // Maintain the mqtt connection
    connect_to_broker();
    
    lastSend = millis();
    client.publish("outTopic", "hello");

    client.loop();
    delay(1000);
  }
  
}

void switch_light() {
  if (digitalRead(ledState) == LOW){
        // turn LED on:
        digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  switch_light();
  Serial.println();
}

void connect_to_broker() {
  // Loop until we're connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, subscribe
      client.subscribe("rooms");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void connect_to_wifi() {
  // create of check the connection to the wifi network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pwd);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
