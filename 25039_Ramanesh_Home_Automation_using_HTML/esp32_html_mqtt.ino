#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "testwifi";
const char* password = "00000000";

const char* mqtt_server = "broker.emqx.io";

#define LIGHT_LED 14
#define FAN_LED   12
#define TV_LED    13

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length)
{
    String msg = "";

    for(int i=0;i<length;i++)
    {
        msg += (char)payload[i];
    }

    Serial.print("Topic : ");
    Serial.println(topic);

    Serial.print("Payload : ");
    Serial.println(msg);

    if(strcmp(topic,"Hall/light")==0)
    {
        if(msg=="ON")
        {
            digitalWrite(LIGHT_LED,HIGH);
            Serial.println("Light ON");
        }
        else if(msg=="OFF")
        {
            digitalWrite(LIGHT_LED,LOW);
            Serial.println("Light OFF");
        }
    }

    if(strcmp(topic,"Hall/fan")==0)
    {
        if(msg=="ON")
        {
            digitalWrite(FAN_LED,HIGH);
            Serial.println("Fan ON");
        }
        else if(msg=="OFF")
        {
            digitalWrite(FAN_LED,LOW);
            Serial.println("Fan OFF");
        }
    }

    if(strcmp(topic,"Hall/TV")==0)
    {
        if(msg=="ON")
        {
            digitalWrite(TV_LED,HIGH);
            Serial.println("TV ON");
        }
        else if(msg=="OFF")
        {
            digitalWrite(TV_LED,LOW);
            Serial.println("TV OFF");
        }
    }
}

void reconnect()
{
    while(!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if(client.connect("ESP32_Client"))
        {
            Serial.println("Connected");

            client.subscribe("Hall/light");
            client.subscribe("Hall/fan");
            client.subscribe("Hall/TV");
        }
        else
        {
            Serial.print("Failed, State = ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LIGHT_LED,OUTPUT);
    pinMode(FAN_LED,OUTPUT);
    pinMode(TV_LED,OUTPUT);

    WiFi.begin(ssid,password);

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("ESP32 IP : ");
    Serial.println(WiFi.localIP());

    client.setServer(mqtt_server,1883);
    client.setCallback(callback);
}

void loop()
{
    if(!client.connected())
    {
        reconnect();
    }

    client.loop();
}