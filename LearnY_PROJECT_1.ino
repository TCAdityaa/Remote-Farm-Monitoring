#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#define DHTPIN 7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
byte mac[] = {
               0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
             };

IPAddress ip(192, 168, 1, 99);
EthernetServer server(80);
 
void setup() 
{
  Serial.begin (9600);
  dht.begin( );
  Ethernet.begin(mac, ip);
  server.begin( );
}

void loop( ) 
{
  float h = dht.readHumidity( );
  float t = dht.readTemperature( );
  EthernetClient client = server.available();
  if (client) 
    {
      boolean currentLineIsBlank = true;
      while (client.connected ( ) ) 
        {
          if (client.available ( ) ) 
            {
              char character = client.read ( );
              Serial.write(character);
              if (character == '\n' && currentLineIsBlank) 
                {
                  client.println ("HTTP/1.1 200 OK");
                  client.println ("Content-Type: text/html");
                  client.println ("Connection: close");
                  client.println ("Refresh: 5");
                  client.println ( );
                  client.println ("<!DOCTYPE HTML>");
                  client.println ("<html>");
                  client.print ("<Title>Arduino Ethernet Webserver </Title>");
                  client.print ("<h1>Arduino Ethernet Shield Webserver </h1>");
                  client.print ("<h4>Temperature in C: ");
                  client.print (t);client.print("C");
                  client.print ("</h4><h4>Humidity: ");
                  client.print (h);client.print("%");
                  client.println ("<br />");
                  client.println ("</html>");
                  break;
                }
                 
                if ( character == '\n') 
                  {
                    currentLineIsBlank = true;
                  } 
                else if (character != '\r') 
                  {
                    currentLineIsBlank = false;
            }
        }
    }
    delay(1);
    client.stop();
  }
}
