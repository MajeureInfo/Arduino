
#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "...";     // your network SSID (name)
char pwd[] = "...";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

const int buttonPin1 = 10;   
const int ledPin1 =  9;    // pin to write LED state
const int ledState1 = 8;   // pin to read LED state

long lastCheck = millis();

// Initialize the Wifi server library
WiFiEspServer server(80);


void setup() {
  // initialize the pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledState1, INPUT);
  
  //initialize the connections
  Serial.begin(115200);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  
  // attempt to connect to WiFi network
  connect_to_wifi();

  Serial.println("You're connected to the network");
  // connected
  printWifiStatus();
 
  // start the web server on port 80
  server.begin();
}

void loop() {
  if (millis() - lastCheck > 1000) {
    connect_to_wifi();
    lastCheck = millis();
  }
  int cpt = 0;
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (cpt == 6){
          switch_light(c);
          Serial.print("Room number: ");
          Serial.println(c);
        }
        cpt ++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("End of request");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}

void switch_light(char c) {
  if (c == '8') {
    digitalWrite(ledPin1, LOW);
  }
  else if (c == '9') {
    digitalWrite(ledPin1, HIGH);
  }
   int ledState = 30;
   int ledPin = 30;
  if (c == '1') {
    ledState = ledState1;
    ledPin = ledPin1;
  }
  if (digitalRead(ledState) == LOW){
        // turn LED on:
        digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
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

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
