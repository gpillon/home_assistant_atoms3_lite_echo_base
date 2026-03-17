#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/speaker/speaker.h"
#include "esphome/components/audio/audio.h"

#include "../m5echo_base_hub.h"

namespace esphome {
namespace m5echo_base {

class M5EchoBaseSpeaker : public Component, public speaker::Speaker {
 public:
  void setup() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  size_t play(const uint8_t *data, size_t length) override;

#ifdef USE_ESP32
  size_t play(const uint8_t *data, size_t length, TickType_t ticks_to_wait) override {
    return this->play(data, length);
  }
#endif

  void start() override;
  void stop() override;
  void finish() override;
  bool has_buffered_data() const override { return false; }

  void set_volume(float volume) override;
  void set_mute_state(bool mute_state) override;

  void set_hub(M5EchoBaseHub *hub) { this->hub_ = hub; }
  void set_initial_volume(float volume) { this->initial_volume_ = volume; }

 protected:
  M5EchoBaseHub *hub_{nullptr};
  float initial_volume_{0.8f};
};

}  // namespace m5echo_base
}  // namespace esphome