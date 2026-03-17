#include "m5echo_base_microphone.h"

namespace esphome {
namespace m5echo_base {

static const char *const TAG = "m5echo_base.microphone";

void M5EchoBaseMicrophone::setup() {
  if (this->hub_ == nullptr) {
    ESP_LOGE(TAG, "Hub not set");
    this->mark_failed();
    return;
  }

  this->audio_stream_info_ = audio::AudioStreamInfo(16, 2, this->hub_->get_sample_rate());
  this->state_ = microphone::STATE_STOPPED;
}

void M5EchoBaseMicrophone::start() {
  if (this->state_ == microphone::STATE_RUNNING || this->state_ == microphone::STATE_STARTING) {
    return;
  }

  this->state_ = microphone::STATE_STARTING;

  if (this->hub_ == nullptr || !this->hub_->is_ready()) {
    this->state_ = microphone::STATE_STOPPED;
    return;
  }

  this->audio_stream_info_ = audio::AudioStreamInfo(16, 2, this->hub_->get_sample_rate());
  this->buffer_.resize(this->audio_stream_info_.ms_to_bytes(20));

  this->state_ = microphone::STATE_RUNNING;
  ESP_LOGI(TAG, "Microphone started");
}

void M5EchoBaseMicrophone::loop() {
  if (this->state_ != microphone::STATE_RUNNING) {
    return;
  }

  if (this->buffer_.empty()) {
    this->buffer_.resize(this->audio_stream_info_.ms_to_bytes(20));
  }

  bool ok = this->hub_->record(this->buffer_.data(), this->buffer_.size());
  if (!ok) {
    ESP_LOGW(TAG, "Microphone capture failed");
    return;
  }

  this->data_callbacks_.call(this->buffer_);
}

void M5EchoBaseMicrophone::stop() {
  if (this->state_ == microphone::STATE_STOPPED || this->state_ == microphone::STATE_STOPPING) {
    return;
  }

  this->state_ = microphone::STATE_STOPPING;
  this->state_ = microphone::STATE_STOPPED;
  ESP_LOGI(TAG, "Microphone stopped");
}

}  // namespace m5echo_base
}  // namespace esphome