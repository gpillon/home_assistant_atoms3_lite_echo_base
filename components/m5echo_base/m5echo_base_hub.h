#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "M5EchoBase.h"

namespace esphome {
namespace m5echo_base {

class M5EchoBaseHub : public Component {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void set_sample_rate(uint32_t sample_rate) { this->sample_rate_ = sample_rate; }
  uint32_t get_sample_rate() const { return this->sample_rate_; }

  bool play(const uint8_t *data, size_t length);
  bool record(uint8_t *data, size_t length);

  void set_volume(float volume);
  void set_mute(bool mute);

  bool is_ready() const { return this->ready_; }

 protected:
  bool init_if_needed_();

  M5EchoBase echo_;
  bool ready_{false};
  uint32_t sample_rate_{16000};
  float volume_{0.8f};
  bool mute_{false};
};

}  // namespace m5echo_base
}  // namespace esphome