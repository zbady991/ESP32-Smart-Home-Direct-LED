#include <EEPROM.h>
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <AceButton.h>
using namespace ace_button;

#define EEPROM_SIZE 10

const char *service_name = "PROV_12345";
const char *pop = "1234567";

char deviceName_1[] = "LED One";
char deviceName_2[] = "LED Two";
char deviceName_3[] = "LED Three";
char deviceName_4[] = "LED Four";

static uint8_t LedPin1 = 23;
static uint8_t LedPin2 = 22;
static uint8_t LedPin3 = 21;
static uint8_t LedPin4 = 19;

static uint8_t SwitchPin1 = 13;
static uint8_t SwitchPin2 = 12;
static uint8_t SwitchPin3 = 14;
static uint8_t SwitchPin4 = 27;

static uint8_t wifiLed    = 2;
static uint8_t gpio_reset = 0;

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;

ButtonConfig config1, config2, config3, config4;
AceButton button1(&config1);
AceButton button2(&config2);
AceButton button3(&config3);
AceButton button4(&config4);

static Switch my_switch1(deviceName_1, NULL);
static Switch my_switch2(deviceName_2, NULL);
static Switch my_switch3(deviceName_3, NULL);
static Switch my_switch4(deviceName_4, NULL);

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx);
void sysProvEvent(arduino_event_t *sys_event);

void writeEEPROM(int addr, bool state) {
  EEPROM.write(addr, state);
  EEPROM.commit();
}

bool readEEPROM(int addr) {
  return EEPROM.read(addr);
}

void setLED(uint8_t pin, int addr, bool state) {
  digitalWrite(pin, state);
  writeEEPROM(addr, state);
  Serial.printf("LED pin %d = %s\n", pin, state ? "ON" : "OFF");
}

void handleButton(AceButton* button, uint8_t eventType, uint8_t btnState,
                  uint8_t ledPin, int eepromAddr, Switch &sw, bool &state) {
  if (eventType != AceButton::kEventReleased) return;
  state = !state;
  setLED(ledPin, eepromAddr, state);
  sw.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, state);
}

void sysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
      Serial.println("Provisioning started!");
      printQR(service_name, pop, "ble");
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
      Serial.println("Wi-Fi credentials received!");
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("Provisioning successful!");
      break;
    case ARDUINO_EVENT_PROV_END:
      Serial.println("Provisioning done!");
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("Wi-Fi connected!");
      digitalWrite(wifiLed, HIGH);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Wi-Fi disconnected.");
      digitalWrite(wifiLed, LOW);
      break;
  }
}

void write_callback(Device *device, Param *param, const param_val_t val,
                    void *priv_data, write_ctx_t *ctx) {
  const char *device_name = device->getDeviceName();
  const char *param_name  = param->getParamName();

  if (strcmp(param_name, "Power") == 0) {
    bool newState = val.val.b;
    if (strcmp(device_name, deviceName_1) == 0) {
      setLED(LedPin1, 0, newState); toggleState_1 = newState;
      my_switch1.updateAndReportParam(param_name, newState);
    } else if (strcmp(device_name, deviceName_2) == 0) {
      setLED(LedPin2, 1, newState); toggleState_2 = newState;
      my_switch2.updateAndReportParam(param_name, newState);
    } else if (strcmp(device_name, deviceName_3) == 0) {
      setLED(LedPin3, 2, newState); toggleState_3 = newState;
      my_switch3.updateAndReportParam(param_name, newState);
    } else if (strcmp(device_name, deviceName_4) == 0) {
      setLED(LedPin4, 3, newState); toggleState_4 = newState;
      my_switch4.updateAndReportParam(param_name, newState);
    }
    Serial.printf("App/Voice: %s = %d\n", device_name, newState);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== ESP32 RainMaker LED Control ===");

  EEPROM.begin(EEPROM_SIZE);

  toggleState_1 = readEEPROM(0);
  toggleState_2 = readEEPROM(1);
  toggleState_3 = readEEPROM(2);
  toggleState_4 = readEEPROM(3);

  Serial.printf("Restored: %d %d %d %d\n",
    toggleState_1, toggleState_2, toggleState_3, toggleState_4);

  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2, OUTPUT);
  pinMode(LedPin3, OUTPUT);
  pinMode(LedPin4, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(gpio_reset, INPUT);

  setLED(LedPin1, 0, toggleState_1);
  setLED(LedPin2, 1, toggleState_2);
  setLED(LedPin3, 2, toggleState_3);
  setLED(LedPin4, 3, toggleState_4);
  digitalWrite(wifiLed, LOW);

  config1.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    handleButton(b, e, s, LedPin1, 0, my_switch1, toggleState_1);
  });
  config2.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    handleButton(b, e, s, LedPin2, 1, my_switch2, toggleState_2);
  });
  config3.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    handleButton(b, e, s, LedPin3, 2, my_switch3, toggleState_3);
  });
  config4.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    handleButton(b, e, s, LedPin4, 3, my_switch4, toggleState_4);
  });

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  Node my_node = RMaker.initNode("ESP32 LED Controller");

  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);

  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);

  RMaker.enableOTA(OTA_USING_PARAMS);
  RMaker.enableTZService();
  RMaker.enableSchedule();
  RMaker.start();

  WiFi.onEvent(sysProvEvent);
  WiFiProv.beginProvision(
    WIFI_PROV_SCHEME_BLE,
    WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
    WIFI_PROV_SECURITY_1,
    pop,
    service_name
  );

  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);

  Serial.println("Setup complete! Waiting for provisioning...");
}

void loop() {
  if (digitalRead(gpio_reset) == LOW) {
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int duration = millis() - startTime;
    if (duration > 10000) {
      Serial.println("Factory reset!");
      RMakerFactoryReset(2);
    } else if (duration > 3000) {
      Serial.println("WiFi reset!");
      RMakerWiFiReset(2);
    }
  }

  digitalWrite(wifiLed, WiFi.status() == WL_CONNECTED);

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}