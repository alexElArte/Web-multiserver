#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

const byte chipSelect = 4;
File dataFile;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 123);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  Serial.begin(1000000);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }*/
    
  Serial.println("Ethernet WebServer");
  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("\tCard failed, or not present");
    while (1);
  }
  Serial.println("\tCard initialized.\n");
  Serial.println("Initializing Ehternet shield...");
  Ethernet.init(10);
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("\tEthernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true);
  }
  // start the server
  server.begin();
  Serial.print(F("\tserver is at "));
  Serial.println(Ethernet.localIP());

}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (!client) {
    return;
  }
  // Print info about the client
  Serial.println(F("New client:"));
  Serial.print(F("\tlocalPort: "));
  Serial.println(client.localPort());
  Serial.print(F("\tremoteIP: "));
  Serial.println(client.remoteIP());
  Serial.print(F("\tremotePort: "));
  Serial.println(client.remotePort());
  // Get request
  String req = client.readStringUntil('\r');
  Serial.print(F("\trequest: "));
  Serial.println(req);

  // No need to know the client's informations
  while (client.available()) client.read();

  byte place = 4; // File start
  bool ext = false; // If there is a type
  String file = "";
  String type = "";
  // Read file name
  while (req[place] != ' ') {
    file += req[place];
    if (ext) type += req[place];
    if (req[place] == '.') ext = true;
    place++;
  }
  Serial.print("\tfile: ");
  Serial.println(file);
  Serial.print("\ttype: ");
  Serial.println(type);

  // I can't read html file from sd card
  // so i transform all html file en text file
  // it didn't change what we saw
  if(type == "") file += ".txt";
  
  if (file == "/") {
    //
  } else if (SD.exists(file)) {
    // Add type if you need
    if (type == "js") {
      type = "javascript";
    } else if(type == ""){ // There is some bugs so we do that
      type = "html";
    }
    client.println(F("HTTP/1.1 200 OK"));
    client.println("Content-Type: text/" + type);
    client.println(F("Connection: close"));
    client.println();
    
    dataFile = SD.open(file);
    if (dataFile) {
      //Send file
      while (dataFile.available()) {
        client.write(dataFile.read());
      }
      dataFile.close();
    }
  } else {
    // Send a blank page
    client.println(F("HTTP/1.1 400"));
    client.println(F("Content-Type: text/html"));
    client.println(F("Connection: close"));
    client.println();
    client.println(F("<!DOCTYPE HTML>"));
    client.println(F("<html>"));
    client.println(F("<head><title>400 Bad Request</title></head>"));
    client.println(F("<body><h1>400 Bad Request</h1><br>File doesn't exist</body>"));
    client.println(F("</html>"));
    Serial.println(F("invalid request"));
  }



  // give the web browser time to receive the data
  delay(1);
  // close the connection:
  client.stop();
  Serial.println("client disconnected\n");
}
