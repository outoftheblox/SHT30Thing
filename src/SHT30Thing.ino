#include <Thing.h>
#include <BlinkPattern.h>
#include <WEMOS_SHT3X.h>

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
SHT3X sht30(0x45);

BlinkPattern::Pattern<2> panic{{1,1},25};
BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};

void setup() 
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Client:" + thing.clientId());

  led.setPattern(start);
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  String sensorH;
  sensorH += "sensor/sht30/";
  sensorH += thing.clientId();
  sensorH += "/humidity";
  thing.addSensor(sensorH, 5000, [](Value& value){
    sht30.get();
    float humidity = sht30.humidity;
    Serial.println(String("Read humidity ") + humidity);
    led.setPattern(normal);
    value = humidity;
  });

  String sensorT;
  sensorT += "sensor/sht30/";
  sensorT += thing.clientId();
  sensorT += "/temperature";
  thing.addSensor(sensorT, 5000, [](Value& value){
    sht30.get();
    float temperature = sht30.cTemp;
    Serial.println(String("Read temperature ") + temperature);
    led.setPattern(normal);
    value = temperature;
  });

  String display;
  display += "display/sht30/";
  display += thing.clientId();
  thing.addActuator(display, [](Value& value){
    //Serial.println("Got " +(String) value);
  });

  thing.begin();
}

void loop()
{
  thing.handle();
  led.handle();
}
