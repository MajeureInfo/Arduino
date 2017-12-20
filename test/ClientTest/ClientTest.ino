/*
 WiFiEsp test: ClientTest
 
 Test client functions. Send a POST request regularly to switch a light on the server
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif


char ssid[] = "...";     // your network SSID (name)
char pwd[] = "...";  // your network password

// Initialize the Wifi client library
WiFiEspClient client;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);
}

void loop()
{
  int c;

  
  assertEquals("Check status WL_DISCONNECTED", WiFi.status(), WL_DISCONNECTED);
  assertEquals("Connect", WiFi.begin(ssid, pwd), WL_CONNECTED);
  assertEquals("Check status WL_CONNECTED", WiFi.status(), WL_CONNECTED);
  
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  assertEquals("Ping", WiFi.ping("www.google.com"), true);
  
  assertEquals("Not connected", client.connected(), false);
  assertEquals("Connect to server", client.connect("pure-island-76277.herokuapp.com", 80), 1);
  assertEquals("Connected", client.connected(), true);


  //--------------------------------------------------------------
  
  delay(5000);
  
  assertEquals("Check status WL_CONNECTED", WiFi.status(), WL_CONNECTED);


  //--------------------------------------------------------------
  // HTTP request with 'Connection: close' command
  
  client.println("POST /api/rooms/1/switch-light HTTP/1.1");
  client.println("Host: pure-island-76277.herokuapp.com");
  client.println();


  // wait for the response
  long startMillis = millis();
  while (!client.available() and (millis() - startMillis < 2000))
  {
  }
  
  assertEquals("Response received", (millis() - startMillis < 2000), true);
  
  while( (c = client.read()) > 0)
  Serial.print((char)c);


  //--------------------------------------------------------------
  
  assertEquals("Check status WL_CONNECTED", WiFi.status(), WL_CONNECTED);
  assertEquals("Not connected", client.connected(), false);

  assertEquals("Ping", WiFi.ping("www.google.com"), true);

  assertEquals("Connect", WiFi.disconnect(), WL_DISCONNECTED);
  assertEquals("Check status WL_DISCONNECTED", WiFi.status(), WL_DISCONNECTED);

  Serial.println("\nEND OF TESTS");
  delay(30000);
}


////////////////////////////////////////////////////////////////////////////////////


void assertEquals(const char* test, int actual, int expected)
{
  if(actual==expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}


void pass(const char* test)
{
  Serial.print(F("***** "));
  Serial.print(test);
  Serial.println(" > PASSED");
}

void fail(const char* test, char* actual, char* expected)
{
  Serial.print(F("***** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  delay(10000);
}

void fail(const char* test, int actual, int expected)
{
  Serial.print(F("***** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=");
  Serial.print(actual);
  Serial.print(", expected=");
  Serial.print(expected);
  Serial.println(")");
  delay(10000);
}


