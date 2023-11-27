#include <Thing.h>
#include <BlinkPattern.h>
#include <WEMOS_SHT3X.h>
#include <ArduinoJson.h>

using namespace ootb;

Thing thing;
BlinkPattern led(BUILTIN_LED);
SHT3X sht30(0x45);

BlinkPattern::Pattern<2> panic{{1, 1}, 25};
BlinkPattern::Pattern<2> start{{1, 9}, 25};
BlinkPattern::Pattern<2> normal{{1, 39}, 25};

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("Client:" + thing.clientId());

    led.begin();
    led.setPattern(start);

    thing.onStateChange([](const String &msg)
    {
        Serial.println(msg);
    });

    thing.begin();


    String topic = "things/" + thing.clientId() + "/sht30/values";
    thing.addSensor(topic, 5000, [](Value &value)
    {
        sht30.get();
        float humidity = sht30.humidity;
        float temperature = sht30.cTemp;
        const int capacity = JSON_OBJECT_SIZE(3);
        StaticJsonBuffer<capacity> jb;
        JsonObject& doc = jb.createObject();
        //DynamicJsonDocument doc(1024);
        doc["humidity"] = humidity;
        doc["temperature"] = temperature;
        String output;
        doc.printTo(output);
        value = output;
        Serial.println(output);
    });

    String topicHumidity = "things/" + thing.clientId() + "/sht30/humidity";
    thing.addSensor(topicHumidity, 60000, [](Value &value)
    {
        sht30.get();
        float humidity = sht30.humidity;
        Serial.println(String("Read humidity ") + humidity);
        led.setPattern(normal);
        value = humidity;
    });

    String topicTemperature = "things/" + thing.clientId() + "/sht30/temperature";
    thing.addSensor(topicTemperature, 60000, [](Value &value)
    {
        sht30.get();
        float temperature = sht30.cTemp;
        Serial.println(String("Read temperature ") + temperature);
        led.setPattern(normal);
        value = temperature;
    });

    String topicDisplay = "things/" + thing.clientId() + "/display";
    thing.addActuator(topicDisplay, [](Value &value)
    {
        Serial.println("Got " + String(value));
    });
}

void loop()
{
    thing.handle();
    led.handle();
}
