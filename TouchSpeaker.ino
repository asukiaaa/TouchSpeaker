#include <SoftwareSerial.h>

#define SWITCH_LEFT_PIN 1
#define SWITCH_RIGHT_PIN 0

#define TOUCH_MIN_VALUE 10
#define TOUCH_MIN_MILLISEC 50
#define COMMAND_WAIT_MILLIS 1000

String command = "";
String soundString = "";
unsigned long commandLastPushedAt = 0;

SoftwareSerial soundSerial(9,8);

class AnalogSwitch {
  public:
  int switchPin;
  int freeState;
  unsigned long touchFromMillisec;
  int touchMinValue;
  unsigned long touchMinMillisec;
  int currentValue;
  int prevValue;

  AnalogSwitch(int _switchPin,
               int _touchMinValue = TOUCH_MIN_VALUE,
               unsigned long _touchMinMillisec = (unsigned long) TOUCH_MIN_MILLISEC);
  void update();
  bool isTouched();
};

AnalogSwitch::AnalogSwitch(int _switchPin, int _touchMinValue, unsigned long _touchMinMillisec) {
  switchPin = _switchPin;
  touchMinValue = _touchMinValue;
  touchMinMillisec = _touchMinMillisec;
}

void AnalogSwitch::update() {
  prevValue = currentValue;
  currentValue = analogRead(switchPin);
  if (prevValue < touchMinValue && currentValue >= touchMinValue) {
    touchFromMillisec = millis();
  }
}

bool AnalogSwitch::isTouched() {
  return (currentValue > touchMinValue && touchFromMillisec < millis() - touchMinMillisec);
}

AnalogSwitch switchL(SWITCH_LEFT_PIN);
AnalogSwitch switchR(SWITCH_RIGHT_PIN);

void setup() {
  Serial.begin(115200);
  switchL.freeState = 0;
  switchR.freeState = 0;
  soundSerial.begin(9600);
  soundSerial.print("?");
}

void loop() {
  switchL.update();
  switchR.update();
  //Serial.print("valL: ");
  //Serial.println(switchL.currentValue);
  //Serial.print("valR: ");
  //Serial.println(switchR.currentValue);

  if (switchR.isTouched()) {
    if (switchR.freeState == 0) {
      command.concat("r");
      switchR.freeState = 1;
      commandLastPushedAt = millis();
    }
  } else {
    if (switchR.freeState == 1) {
      switchR.freeState = 0;
    }
  }

  if (switchL.isTouched()) {
    if (switchL.freeState == 0) {
      command.concat("l");
      switchL.freeState = 1;
      commandLastPushedAt = millis();
    }
  } else {
    if (switchL.freeState == 1) {
      switchL.freeState = 0;
    }
  }

  // execute command
  if (commandLastPushedAt < millis() - COMMAND_WAIT_MILLIS && command != "") {
    Serial.println(command);
    soundString = getSoundStringByCommand(command);
    Serial.println(soundString);
    soundSerial.print(soundString);
    soundSerial.print("\r");
    command = "";
  }
}

String getSoundStringByCommand(String command) {
  if (command == "l") {
    return "ha'i";
  } else if (command == "r") {
    return "i'ie";
  } else if (command == "ll") {
    return "shokuji.";
  } else if (command == "rr") {
    return "to'ire";
  } else if (command == "llr") {
    return "sanpo";
  } else if (command == "lrr") {
    return "oyatu.";
  } else if (command == "lll") {
    return "okaasan.";
  } else if (command == "rrr" ) {
    return "otousan";
  } else if (command == "lllr") {
    return "atama itai.";
  } else if (command == "1rrr") {
    return "onaka itai.";
  }
  return "";
}

