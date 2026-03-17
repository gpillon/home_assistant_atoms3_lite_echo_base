#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/microphone/microphone.h"
#include "esphome/components/audio/audio.h"

#include "../m5echo_base_hub.h"

#include <vector>

namespace esphome {
namespace m5echo_base {

class M5EchoBaseMicrophone : public Component, public microphone::Microphone {
 public:
  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void start() override;
  void stop() override;

  void set_hub(M5EchoBaseHub *hub) { this->hub_ = hub; }

 protected:
  M5EchoBaseHub *hub_{nullptr};
  std::vector<uint8_t> buffer_;
};

}  // namespace m5echo_base
}  // namespace esphome