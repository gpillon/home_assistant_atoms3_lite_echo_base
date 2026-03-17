#include "m5echo_base_hub.h"

namespace esphome {
namespace m5echo_base {

static const char *const TAG = "m5echo_base.hub";

int M5EchoBaseHub::map_volume_(float volume) const {
  if (volume <= 0.0f) return 0;
  if (volume >= 1.0f) return this->volume_max_;
  return this->volume_min_ +
         static_cast<int>(volume * (this->volume_max_ - this->volume_min_));
}

bool M5EchoBaseHub::init_if_needed_() {
  if (this->ready_) {
    return true;
  }

  ESP_LOGI(TAG, "Initializing M5EchoBase at %u Hz (volume range %u–%u)",
           this->sample_rate_, this->volume_min_, this->volume_max_);

  this->ready_ = this->echo_.init(
      this->sample_rate_,
      GPIO_NUM_38,  // SDA
      GPIO_NUM_39,  // SCL
      GPIO_NUM_7,   // I2S DI
      GPIO_NUM_6,   // I2S WS
      GPIO_NUM_5,   // I2S DO
      GPIO_NUM_8,   // I2S BCK
      Wire);

  if (!this->ready_) {
    ESP_LOGE(TAG, "M5EchoBase init failed");
    return false;
  }

  this->echo_.setMute(this->mute_);
  this->echo_.setSpeakerVolume(this->map_volume_(this->volume_));
  delay(10);
  return true;
}

void M5EchoBaseHub::setup() {
  if (!this->init_if_needed_()) {
    this->mark_failed();
  }
}

bool M5EchoBaseHub::play(const uint8_t *data, size_t length) {
  if (!this->init_if_needed_()) {
    return false;
  }
  if (data == nullptr || length == 0) {
    return false;
  }

  this->echo_.setMute(this->mute_);
  this->echo_.setSpeakerVolume(this->map_volume_(this->volume_));

  return this->echo_.play(data, static_cast<int>(length));
}

bool M5EchoBaseHub::record(uint8_t *data, size_t length) {
  if (!this->init_if_needed_()) {
    return false;
  }
  if (data == nullptr || length == 0) {
    return false;
  }

  return this->echo_.record(data, static_cast<int>(length));
}

void M5EchoBaseHub::set_volume(float volume) {
  if (volume < 0.0f) volume = 0.0f;
  if (volume > 1.0f) volume = 1.0f;
  this->volume_ = volume;

  if (this->ready_) {
    this->echo_.setSpeakerVolume(this->map_volume_(this->volume_));
  }
}

void M5EchoBaseHub::set_mute(bool mute) {
  this->mute_ = mute;

  if (this->ready_) {
    this->echo_.setMute(mute);
  }
}

}  // namespace m5echo_base
}  // namespace esphome