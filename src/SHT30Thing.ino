#include <Thing.h>
#include <BlinkPattern.h>
#include <WEMOS_SHT3X.h>

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

    String topicHumidity = "things/" + thing.clientId() + "/sht30/humidity";
    thing.addSensor(topicHumidity, 5000, [](Value &value)
    {
        sht30.get();
        float humidity = sht30.humidity;
        Serial.println(String("Read humidity ") + humidity);
        led.setPattern(normal);
        value = humidity;
    });

    String topicTemperature = "things/" + thing.clientId() + "/sht30/temperature";
    thing.addSensor(topicTemperature, 5000, [](Value &value)
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
