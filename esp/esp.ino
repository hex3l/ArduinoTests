#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial esp8266(3,2);

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CWSAP=\"DroneTest\",\"testwifi\",1,2,4,0\r\n",1000,DEBUG); // get ip address //192.168.4.1
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}

void loop() {
  if(esp8266.available()){
    String response = "";
    if(esp8266.find("+IPD,"))
    {
        int connectionId = esp8266.read()-48;
         if(esp8266.find("/?"))
          {
            while(esp8266.available())
              {
              
              // The esp has data so display its output to the serial window
              char c = esp8266.read(); // read the next character.
              response+=c;
              }
          }
          response.remove(response.indexOf(" HTTP/"));
        Serial.print(response);
        // make close command
       String closeCommand = "AT+CIPCLOSE="; 
       closeCommand+=connectionId; // append connection id
       closeCommand+="\r\n";
       
       sendData(closeCommand,0,DEBUG); // close connection
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  
  esp8266.print(command); // send the read character to the esp8266
  
  long int time = millis();
  
  while( (time+timeout) > millis())
  {
    while(esp8266.available())
    {
    
    // The esp has data so display its output to the serial window
    char c = esp8266.read(); // read the next character.
    response+=c;
    }
  }
  
  if(debug)
  {
    Serial.print(response);
  }
  
  return response;
}
