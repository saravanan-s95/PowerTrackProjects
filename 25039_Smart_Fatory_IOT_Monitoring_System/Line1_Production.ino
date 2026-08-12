#include <WiFi.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
#include <DHT.h>

const char *WIFI_SSID = "testwifi";
const char *WIFI_PASSWORD = "00000000";

const char *THINGSBOARD_SERVER = "mqtt.thingsboard.cloud";
const char *TOKEN = "yucZPadvU6Ccbt1k6QZt";

#define DHTPIN 13
#define DHTTYPE DHT11

#define PIR_PIN 14
#define TRIG 25
#define ECHO 26
#define LDR 39
#define LED 2

DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient);

void connectWiFi()
{
    Serial.println("Connecting to WiFi...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nWiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
void connectThingsBoard()
{
    while (!tb.connected())
    {
        Serial.println("Connecting to ThingsBoard...");

        if (tb.connect(THINGSBOARD_SERVER, TOKEN))
        {
            Serial.println("Connected to ThingsBoard");
        }
        else
        {
            Serial.println("Connection Failed");
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(PIR_PIN, INPUT);
    pinMode(LED,OUTPUT);
    pinMode(TRIG, OUTPUT);         // Set TRIG as output
    pinMode(ECHO, INPUT); 


    dht.begin();

    connectWiFi();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!tb.connected())
    {
        connectThingsBoard();
    }

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    int ldr = analogRead(LDR);

    int PIR = digitalRead(PIR_PIN);
    digitalWrite(TRIG, LOW);       // Set trigger low
    delayMicroseconds(2);          // Wait 2 microseconds

    digitalWrite(TRIG, HIGH);      // Set trigger high
    delayMicroseconds(10);         // Wait 10 microseconds

    digitalWrite(TRIG, LOW);       // Set trigger low

    int duration = pulseIn(ECHO, HIGH); // Read echo pulse time

    int distance = (duration * 0.034) / 2; 

    if(humidity >= 60)
    {
        digitalWrite(LED,HIGH);
    }
    else
    {
        digitalWrite(LED,LOW);

    }

    Serial.println("--------------------------------");

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity : ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("PIR Status : ");
    Serial.println(PIR);

    Serial.print("LDR Status : ");
    Serial.println(ldr);

    Serial.print("Ultrasonic distance in cm : ");
    Serial.print(distance);
    
    if (PIR == HIGH)
    {
        Serial.println("Motion Detected");
    }
    else
    {
        Serial.println("No Motion");
    }


    tb.sendTelemetryData("Temperature", temperature);
    tb.sendTelemetryData("Humidity", humidity);
    tb.sendTelemetryData("PIR", PIR);
    tb.sendTelemetryData("Ultrasonic",distance);
    tb.sendTelemetryData("LDR",ldr);



    Serial.println("Telemetry Sent to ThingsBoard");

    tb.loop();

    delay(5000);
}