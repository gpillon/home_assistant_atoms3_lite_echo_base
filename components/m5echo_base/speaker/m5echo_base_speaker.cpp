#include "m5echo_base_speaker.h"

namespace esphome {
namespace m5echo_base {

static const char *const TAG = "m5echo_base.speaker";

void M5EchoBaseSpeaker::setup() {
  if (this->hub_ == nullptr) {
    ESP_LOGE(TAG, "Hub not set");
    this->mark_failed();
    return;
  }

  this->audio_stream_info_ = audio::AudioStreamInfo(16, 2, this->hub_->get_sample_rate());
  this->state_ = speaker::STATE_STOPPED;
  this->set_volume(this->initial_volume_);
  this->set_mute_state(false);
}

void M5EchoBaseSpeaker::start() {
  if (this->state_ == speaker::STATE_RUNNING || this->state_ == speaker::STATE_STARTING) {
    return;
  }

  this->state_ = speaker::STATE_STARTING;

  if (this->hub_ == nullptr || !this->hub_->is_ready()) {
    this->state_ = speaker::STATE_STOPPED;
    return;
  }

  this->hub_->set_mute(this->mute_state_);
  this->hub_->set_volume(this->volume_);
  this->state_ = speaker::STATE_RUNNING;
}

size_t M5EchoBaseSpeaker::play(const uint8_t *data, size_t length) {
  if (data == nullptr || length == 0) {
    return 0;
  }

  if (this->state_ != speaker::STATE_RUNNING) {
    this->start();
  }

  if (this->hub_ == nullptr || this->state_ != speaker::STATE_RUNNING) {
    return 0;
  }

  if (this->mute_state_) {
    return length;
  }

  bool ok = this->hub_->play(data, length);
  if (!ok) {
    ESP_LOGW(TAG, "Playback failed");
    return 0;
  }

  auto &info = this->get_audio_stream_info();
  if (info.get_sample_rate() > 0 && info.get_channels() > 0 && info.get_bits_per_sample() > 0) {
    uint32_t frames = info.bytes_to_frames(length);
    uint32_t usec = info.frames_to_microseconds(frames);
    this->audio_output_callback_.call(static_cast<uint32_t>(length), usec);
  }

  return length;
}

void M5EchoBaseSpeaker::stop() {
  this->state_ = speaker::STATE_STOPPING;

  if (this->hub_ != nullptr) {
    this->hub_->set_mute(true);
  }

  this->state_ = speaker::STATE_STOPPED;
}

void M5EchoBaseSpeaker::finish() {
  this->stop();
}

void M5EchoBaseSpeaker::set_volume(float volume) {
  volume = std::clamp(volume, 0.0f, 1.0f);
  speaker::Speaker::set_volume(volume);

  if (this->hub_ != nullptr) {
    this->hub_->set_volume(volume);
  }
}

void M5EchoBaseSpeaker::set_mute_state(bool mute_state) {
  speaker::Speaker::set_mute_state(mute_state);

  if (this->hub_ != nullptr) {
    this->hub_->set_mute(mute_state);
  }
}

}  // namespace m5echo_base
}  // namespace esphome