# Domotica Arduino
Domotica Arduino is a program written for all Arduino devices.
It let you to control some physical devices in your home with PC or smartphone. All commands are sent using GET method and received responses are all JSON objects string.

## Setup before uploading
In order to use all of the proposed feature, you should edit some parameters before uploading the "sketch" in you Arduino. You should set these line of code for each section.

### Network parameters
Configure Arduino with your network:

    byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };      // MAC Address for Arduino Ethernet Shield (ex: { 0x80, 0xF2, 0xCA, 0x00, 0xA6, 0xB9 })
    byte ip[] = { 0, 0, 0, 0 };                               // Set a static IP
    byte gateway[] = { 0, 0, 0, 0 };                          // Gateway IP
    byte subnet[] = { 0, 0, 0, 0 };                           // Subnet mask
    EthernetServer server = EthernetServer(80);               // Listening port (it's better to select port 80)

### Define constant type
All objects are grouped under some type. Change or add these constants in order to create new types:

    byte LIGHT = 0;
    byte OBJECT = 1;

### Configure connected physical devices
When you connect a device, for example a LED or a transistor to your Arduino, you connect this device to a PIN. In this section, you define if a PIN is enabled and what type of device is connected:

    int activedDevice[] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
    int typeDevice[] = { -1, -1, -1, -1, -1, -1, -1, LIGHT, LIGHT, OBJECT };
    char* nameDevice[] = { "", "", "", "", "", "", "", "Outdoor Garden", "Kitchen", "Fan" };
    int statusDevice[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

All constants are an array object because each array index is referred to an Arduino PIN. In example, the first index value of each arrays are respectively, the enabled status, the device type, the device name and the status of the device (on/off) of the first device connected on the first Arduino PIN.

In particular:

1. <code>activedDevice</code> is used to set if a PIN is enabled or not. Set the PIN to 1 to enable digitalWrite OUTPUT on that PIN.
1. <code>typeDevice</code> set for that device under which type it should be classified
1. <code>nameDevice</code> is the device name
1. <code>statusDevice</code> represents if the device is turned on or off

## Usage (API)
After the sketch is uploaded in your Arduino, you can access to these function using GET method with a traditional web browser:

### Discovery
Use:

    http://<your arduino ip>/?discovery

This will return a JSON response with all connected and configured devices. The syntax of JSON response is:
    
    { "devices" : [ {"id" : "ID device", "type" : "Type of device", "name" : "Name assigned", "status" : "0 or 1"}, {...}, ...] }

Devices object is a JSON array so the example above is repeated for every activedDevice.

### Get status of a device
Use:
    
    http://<your arduino ip>/?status&device=X

This will return a JSON response with the status of device connected to the PIN (X+1). The syntax of JSON response is:

    { "id" : "ID device", "status" : "0 or 1" }

### Set status of a device
Use:

    http://<your arduino ip>/?device=X&status=Y

This will turn on (status=1) or off (status=0) a device connected to the PIN (X+1). The syntax of JSON response, if the switch is succeded, is:

    { "id" : "ID Device", "response" : "ACK" }

No response is sent in case of failure.

## Android device (without ADK)
This project could be used with DomoticaArduinoApp to manage devices with an Android smartphone. Because of this project uses JSON parsing and Android HttpClient, it is *NOT* necessary that your Arduino is an ADK microcontroller.
