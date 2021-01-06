/*
  Joseph Ogbonna Web Browser Led Controlled Project
  using Esp32 Cam 
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
#define red 12
#define blue 15
#define green 13

// Set these to your desired credentials.
const char *ssid = "JOSEPH";
const char *password = "11111111";

WiFiServer server(80);


void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  digitalWrite(red, LOW);
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<h1>THIS IS A SIMPLE WEB CONTROL </h>");
            client.print(" <a href=\"/red-led-on\"><button >Red Led</button></a> to turn ON the red LED.<br>");
            client.print("Click <a href=\"/red-led-off\"><button >Red Led</button></a> to turn OFF the red LED.<br>");
            client.print(" <a href=\"/blue-led-on\"><button >Blue Led</button></a> to turn ON the blue LED.<br>");
            client.print("Click <a href=\"/blue-led-off\"><button >Blue Led</button></a> to turn OFF the blue LED.<br>");
            client.print(" <a href=\"/green-led-on\"><button >Green Led</button></a> to turn ON the green LED.<br>");
            client.print("Click <a href=\"/green-led-off\"><button >Green Led</button></a> to turn OFF the green LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /red-led-on")) {
          digitalWrite(red, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /red-led-off")) {
          digitalWrite(red, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /blue-led-on")) {
          digitalWrite(blue, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /blue-led-off")) {
          digitalWrite(blue, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /green-led-on")) {
          digitalWrite(green, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /green-led-off")) {
          digitalWrite(green, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
