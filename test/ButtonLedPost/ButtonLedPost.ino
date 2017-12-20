/*
Change the state of the LED each time the button is pressed
and send a POST request to switch the light on the server.
*/

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

const int buttonPin = 10;   
const int ledPin =  9;    // pin to write LED state
const int ledState = 8;   // pin to read LED state

int buttonState = 0;      // variable for reading the button status
int lastButtonState = 0;  // variable to remember last button state to detect rising edges
long t = millis();

// Initialize the Wifi client library
WiFiEspClient client;

void setup() {
  // initialize the pins
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledState, INPUT);
  
  //initialize the connections
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pwd);
  }

  // connected
  printWifiStatus();
 
  delay(3000);
  Serial.println("*** Ready to start ***");
}

void loop() {
  if(millis() - t > 50){
    // read the state of the button:
    buttonState = digitalRead(buttonPin);
    
    // detect a rising edge on the button state (LOW then HIGH)
    if (buttonState == HIGH && lastButtonState == LOW) {
      Serial.println("\nbutton pressed");  
      if (digitalRead(ledState) == LOW){
        // turn LED on:
        digitalWrite(ledPin, HIGH);
      } else {
        // turn LED off:
        digitalWrite(ledPin, LOW);
      }

      // connect to the server
      while (!client.connected()){
        client.connect(host, 80);
      }
      Serial.println("Connected to the server");
      
      // send HTTP request with 'Connection: close' command
      client.println("POST /api/rooms/1/switch-light HTTP/1.1");
      client.print("Host: ");
      client.println(host);
      client.println("Connection: close");
      client.println();
      client.stop();
    }
    lastButtonState = digitalRead(buttonPin);
    t = millis();
  }
}

void printWifiStatus()
{
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
