#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace m62429 {

class M62429Output : public output::FloatOutput, public Component {
 public:
  void set_clk_pin(InternalGPIOPin *pin) { clk_pin_ = pin; }
  void set_data_pin(InternalGPIOPin *pin) { data_pin_ = pin; }
  void set_initial_value(float initial_value) { initial_value_ = initial_value; }
  void set_step_delay(int step_delay) { step_delay_ = step_delay; }

  void setup() override;
  void dump_config() override;

  void set_attn(uint8_t attn);

 protected:
  void write_state(float state) override;
  InternalGPIOPin *clk_pin_;
  InternalGPIOPin *data_pin_;
  float initial_value_;
  float volume_;
  int step_delay_;
};

}  // namespace m62429
}  // namespace esphome
