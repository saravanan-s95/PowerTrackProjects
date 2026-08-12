#include <WiFi.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
#include <DHT.h>

const char *WIFI_SSID = "testwifi";
const char *WIFI_PASSWORD = "00000000";

const char *THINGSBOARD_SERVER = "mqtt.thingsboard.cloud";
const char *TOKEN = "YOUR_DEVICE_ACCESS_TOKEN";

#define DHTPIN 14
#define DHTTYPE DHT11

#define PIR_PIN 25
#define VIBRATION_PIN 26
#define LED_PIN 2

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
        delay(500);
    }

    Serial.println("\nWiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
}

void connectThingsBoard()
{
    while (!tb.connected())
    {
        Serial.println("Connecting to ThingsBoard...");

        if (tb.connect(THINGSBOARD_SERVER, TOKEN))
        {
            Serial.println("Warehouse Device Connected");
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
    pinMode(VIBRATION_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

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

    int motion = digitalRead(PIR_PIN);
    int vibration = digitalRead(VIBRATION_PIN);

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read DHT Sensor");
        delay(2000);
        return;
    }

    bool alert = false;

    if (temperature > 35)
        alert = true;

    if (vibration == HIGH)
        alert = true;

    digitalWrite(LED_PIN, alert);

    Serial.println("--------------------------------------");

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity : ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Motion Status : ");
    if (motion == HIGH)
        Serial.println("Worker / Intruder Detected");
    else
        Serial.println("No Movement");

    Serial.print("Rack Vibration : ");
    if (vibration == HIGH)
        Serial.println("Abnormal Vibration");
    else
        Serial.println("Normal");

    Serial.print("Warehouse Alert : ");
    if (alert)
        Serial.println("ACTIVE");
    else
        Serial.println("NORMAL");

    tb.sendTelemetryData("Warehouse_Temperature", temperature);
    tb.sendTelemetryData("Warehouse_Humidity", humidity);
    tb.sendTelemetryData("Motion_Status", motion);
    tb.sendTelemetryData("Rack_Vibration", vibration);
    tb.sendTelemetryData("Warehouse_Alert", alert);

    Serial.println("Telemetry Uploaded Successfully");

    tb.loop();

    delay(5000);
}