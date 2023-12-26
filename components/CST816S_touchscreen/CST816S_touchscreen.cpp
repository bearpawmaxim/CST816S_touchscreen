#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

#include "CST816S_touchscreen.h"
#include "CST816S.h"

#define I2C_SDA 4
#define I2C_SCL 5
#define TP_RST 1
#define TP_INT 0

namespace esphome {
namespace cst816s_touchscreen {

unsigned long previousMillis = 0UL;
unsigned long interval = 500UL;

static const char *TAG = "cst816s_touchscreen.text_sensor";

// CST816S touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT)	// sda, scl, rst, irq

void CST816STouchScreen::setup() {
  ESP_LOGI("touchscreen", "Starting touchscreen");
  this->_touchscreen = new CST816S(I2C_SDA, I2C_SCL, RST_PIN, TP_INT);
  this->_touchscreen->begin();
}

void CST816STouchScreen::dump_config() { 
  ESP_LOGCONFIG(TAG, "CST816STouchScreen");
}

void CST816STouchScreen::loop() {
  unsigned long currentMillis = millis();
  auto touch = this->_touchscreen;
  if (touch->available()) {
    if (touch->data.gestureID != 0) { //ignore None gesture type
      if (currentMillis - previousMillis > interval) { // debounce
        char buf[20];
        sprintf(buf, "%s", touch->gesture());
        ESP_LOGI("touchscreen", "Gesture: %s", touch->gesture());
        this->publish_state(buf);
        previousMillis = currentMillis;
      }
    }
  }
}

}  // namespace cst816s_touchscreen
}  // namespace esphome
