#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "JoyBoy";
const char* password = "akash101";

const char* mqtt_server = "broker.emqx.io";   

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Topic: ");
    Serial.println(topic);

    String msg="";
    for (int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }

    Serial.print("Payload : ");
    if(strcmp(topic,"Hall/light")==0)
    {
      if(msg=="ON")
      {
        Serial.println(msg);
      }
      else
      {
        Serial.println(msg);
      }
    }
    else if(strcmp(topic,"Hall/fan")==0)
    {
      if(msg=="ON")
      {
        Serial.println(msg);
      }
      else
      {
        Serial.println(msg);
      }

    }
    else if(strcmp(topic,"Hall/TV")==0)
    {
      if(msg=="ON")
      {
        Serial.println(msg);
      }
      else
      {
        Serial.println(msg);
      }

    }
    
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect("ESP32_Client"))
        {
            Serial.println("Connected");
            client.subscribe("Hall/light");
            client.subscribe("Hall/fan");
            client.subscribe("Hall/TV");

        }
        else
        {
            Serial.print("Failed. State = ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected");

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }

    client.loop();
}