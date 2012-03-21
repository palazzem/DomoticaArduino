#include <SPI.h>
#include <Ethernet.h>

/* Network parameters */
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };      // MAC Address for Arduino Ethernet Shield (ex: { 0x80, 0xF2, 0xCA, 0x00, 0xA6, 0xB9 })
byte ip[] = { 0, 0, 0, 0 };                               // Set a static IP
byte gateway[] = { 0, 0, 0, 0 };                          // Gateway IP
byte subnet[] = { 0, 0, 0, 0 };                           // Subnet mask
EthernetServer server = EthernetServer(80);               // Listening port (it's better to select port 80)

/* These constants are used to define every type of connected devices */
byte LIGHT = 0;
byte OBJECT = 1;

/* Client reading string */
String readString = String(30);

/* Devices configuration */
int activedDevice[] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };                                          // Set 0 if the PIN is disabled otherwise 1
int typeDevice[] = { -1, -1, -1, -1, -1, -1, -1, LIGHT, LIGHT, OBJECT };                         // Set -1 if the device is not used, otherwise a type constant
char* nameDevice[] = { "", "", "", "", "", "", "", "Outdoor Garden", "Kitchen", "Fan" };        // Set the device name or "" if the device is not present
int statusDevice[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };                                           // Always set the status of the device to 0 when Arduino is launched

/* Startup actions */
void setup(){
    // Ethernet shield is activated and all devices are initialized
    Ethernet.begin(mac, ip, gateway, subnet);
    server.begin();
    deviceInit();
}

/* Initialization of all connected devices */
void deviceInit(){
  for (int i = 0; i < 10; i++) {
    if (activedDevice[i] == 1) {
      pinMode(i, OUTPUT);
      digitalWrite(i, LOW);
    }
  }
}

/* Get param from a request */
int getParam(int pos) {
  char charValue, *pointer;
  
  charValue = readString.charAt(pos);
  *pointer = charValue;
  return atoi(pointer);
}

/* Loop method */
void loop(){
    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                // Read the received command sent from client
                char c = client.read();
                if (readString.length() < 30) {
                    readString = readString + c;
                }
                
                // The whole command is read
                if (c == '\n') {
                    // Prepare a JSON response
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println();
                    
                    // Request device status
                    if(readString.startsWith("GET /?status")){
                      int idDevice = getParam(20);
                      
                      client.print("{\"id\" : \"");
                      client.print(idDevice);
                      client.print("\" , \"status\" : \"");
                      client.print(statusDevice[idDevice]);
                      client.print("\"}");
                    }
                    
                    // Change device status
                    if(readString.startsWith("GET /?device")){
                      int idDevice = getParam(13);
                      int newStatus = getParam(22);
                      statusDevice[idDevice] = newStatus;
                      
                      digitalWrite(idDevice, newStatus);

                      client.print("{\"id\" : \"");
                      client.print(idDevice);
                      client.print("\" , \"response\" : \"ACK\"}");
                    }
                    
                    // Discovery all devices
                    if(readString.startsWith("GET /?discovery")){
                      client.print("{\"devices\" : [");
                      
                      boolean isFirst = true;
                      for (int i = 0; i < 10; i++) {
                        if (activedDevice[i] == 1) {
                          if (!isFirst) {
                             client.print(" , ");
                          }
                          
                          client.print("{\"id\" : \"");
                          client.print(i);
                          client.print("\" , \"type\" : \"");
                          client.print(typeDevice[i]);
                          client.print("\" , \"name\" : \"");
                          client.print(nameDevice[i]);
                          client.print("\" , \"status\" : \"");
                          client.print(statusDevice[i]);
                          client.print("\"}");
                          
                          isFirst = false;
                        }
                      }
                      
                      client.print("]}");
                    }
            
                    // Reset command string and close connection with client
                    readString = "";
                    client.stop();
                }
            }
        }
    }
}
