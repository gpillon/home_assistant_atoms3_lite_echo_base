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

  void set_volume_range(uint8_t min_vol, uint8_t max_vol) {
    this->volume_min_ = min_vol;
    this->volume_max_ = max_vol;
  }

  bool play(const uint8_t *data, size_t length);
  bool record(uint8_t *data, size_t length);

  void set_volume(float volume);
  void set_mute(bool mute);

  bool is_ready() const { return this->ready_; }

 protected:
  bool init_if_needed_();
  int map_volume_(float volume) const;

  M5EchoBase echo_;
  bool ready_{false};
  uint32_t sample_rate_{16000};
  float volume_{0.8f};
  bool mute_{false};
  uint8_t volume_min_{45};
  uint8_t volume_max_{85};
};

}  // namespace m5echo_base
}  // namespace esphome