#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "CST816S.h"

#define RST_PIN 0

namespace esphome {
namespace cst816s_touchscreen {

class CST816STouchScreen: public i2c::I2CDevice,
                          public text_sensor::TextSensor,
                          public Component {
 private:
  CST816S *_touchscreen;
  InternalGPIOPin *interrupt_pin_;
  GPIOPin *reset_pin_;

 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_interrupt_pin(InternalGPIOPin *pin) { this->interrupt_pin_ = pin; }
  void set_reset_pin(GPIOPin *pin) { this->reset_pin_ = pin; }
};

}  // namespace cst816s_touchscreen
}  // namespace esphome
