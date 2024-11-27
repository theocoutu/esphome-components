#include "m62429.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m62429 {

static const char *const TAG = "m62429.output";


void M62429Output::set_attn(uint8_t attn) {
  uint16_t volume_data = 0x0200;            //  D9 (latch bit)
  volume_data |= ((attn & 0x03) << 7);      //  D8 - D7
  volume_data |= (attn & 0x7C);             //  D6 - D2
  //if channel == 2 do nothing as 0x00 is default
  //if (channel == 0) volume_data |= 0x03;  //  D0 - D1 for left
  //if (channel == 1) volume_data |= 0x02;  //  D0 - D1 for right

  // write D0-9
  for (uint8_t i = 0; i < 10; i++)
  {
    this->data_pin_->digital_write(volume_data & 0x01);
    volume_data >>= 1;
    this->clk_pin_->digital_write(1);
    delayMicroseconds(this->step_delay_);
    
    this->data_pin_->digital_write(0);
    this->clk_pin_->digital_write(0);
    delayMicroseconds(this->step_delay_);
  }

  // Send D10 latch HI
  this->data_pin_->digital_write(1);
  this->clk_pin_->digital_write(1);
  delayMicroseconds(this->step_delay_);
  
  // deselect
  this->data_pin_->digital_write(1);
  this->clk_pin_->digital_write(0);
  delayMicroseconds(this->step_delay_);
  
  this->data_pin_->digital_write(0);
  delayMicroseconds(this->step_delay_);
}

void M62429Output::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M62429 Digital Stereo Volume Control with initial value of %f", this->initial_value_);

  this->data_pin_->get_pin();
  this->data_pin_->setup();
  this->data_pin_->digital_write(false);

  this->clk_pin_->get_pin();
  this->clk_pin_->setup();
  this->clk_pin_->digital_write(false);

  this->volume_ = this->initial_value_;
  this->write_state(this->initial_value_);
}

void M62429Output::write_state(float state) {
  this->set_attn(static_cast<uint8_t>(roundf(state * 87)));
  this->volume_ = state;
}

void M62429Output::dump_config() {
  ESP_LOGCONFIG(TAG, "M62429 Volume Control Output:");
  LOG_PIN("  Clock Pin: ", this->clk_pin_);
  LOG_PIN("  Data Pin: ", this->data_pin_);
  ESP_LOGCONFIG(TAG, "  Initial Value: %f", this->initial_value_);
  ESP_LOGCONFIG(TAG, "  Step Delay: %d", this->step_delay_);
  LOG_FLOAT_OUTPUT(this);
}

}  // namespace m62429
}  // namespace esphome
