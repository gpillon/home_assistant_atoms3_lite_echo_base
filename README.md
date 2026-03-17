# ESPHome M5Stack AtomS3 Lite + Atomic Echo Base

Custom [ESPHome](https://esphome.io/) external component for the **M5Stack AtomS3 Lite** with the **Atomic Echo Base**, enabling full audio I/O integration with **Home Assistant** voice assistants.

This component drives the **ES8311** audio codec on the Echo Base, providing speaker and microphone platforms that plug directly into ESPHome's voice assistant pipeline.

---

## Why This Project

The M5Stack AtomS3 Lite paired with the Atomic Echo Base is one of the most compact and affordable voice assistant hardware combos for Home Assistant. However, there was no ready-to-use ESPHome component for this specific hardware pairing. This project fills that gap with a clean, self-contained external component.

## Features

- **Speaker platform** — plays TTS audio and announcements through the Echo Base speaker
- **Microphone platform** — captures audio from the Echo Base MEMS microphone
- **Voice Assistant ready** — works with ESPHome's `voice_assistant` component (wake word, STT, TTS)
- **Media Player support** — exposes the device as a media player in Home Assistant
- **Volume & mute control** — configurable speaker volume with hardware mute via PI4IOE5V6408 I2C expander
- **LED feedback** — example config with visual state indicators on the built-in WS2812 NeoPixel LEDs
- **Single hub architecture** — shared initialization of I2C, I2S, and codec with a clean hub/platform pattern

## Hardware Requirements

| Component | Description |
|-----------|-------------|
| [M5Stack AtomS3 Lite](https://shop.m5stack.com/products/atoms3-lite-esp32s3-dev-kit) | ESP32-S3 microcontroller (SKU: C124) |
| [M5Stack Atomic Echo Base](https://shop.m5stack.com/products/atomic-echo-base) | Audio base with ES8311 codec, speaker & mic (SKU: A136) |

Just plug the AtomS3 Lite onto the Echo Base — no soldering required.

## Quick Start

### 1. Add the external component

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/YOUR_USERNAME/home_assistant_atoms3_lite_echo_base
      ref: main
    components: [m5echo_base]
```

### 2. Configure the audio hub

```yaml
esphome:
  name: atoms3-echo
  libraries:
    - Wire
    - ESP_I2S

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

m5echo_base:
  id: echo_base_hub
  sample_rate: 16000
```

### 3. Add speaker and microphone

```yaml
speaker:
  - platform: m5echo_base
    id: echo_speaker
    m5echo_base_id: echo_base_hub
    volume: 80%

microphone:
  - platform: m5echo_base
    id: echo_mic
    m5echo_base_id: echo_base_hub
```

### 4. Wire up the voice assistant

```yaml
media_player:
  - platform: speaker
    id: echo_media_player
    name: "AtomS3 Echo Audio"
    announcement_pipeline:
      speaker: echo_speaker
      format: WAV
      sample_rate: 16000
      num_channels: 2
    buffer_size: 4000
    codec_support_enabled: false

voice_assistant:
  id: va
  microphone:
    microphone: echo_mic
    channels: [0]
    gain_factor: 4
  media_player: echo_media_player
  use_wake_word: true
  noise_suppression_level: 2
  auto_gain: 31dBFS
  volume_multiplier: 1.0
```

See [`examples/`](examples/) for complete, ready-to-flash configurations.

## Examples

| File | Description |
|------|-------------|
| [`examples/minimal.yaml`](examples/minimal.yaml) | Bare-minimum config: audio hub + speaker + microphone + media player |
| [`examples/voice-assistant.yaml`](examples/voice-assistant.yaml) | Full voice assistant with wake word, LED feedback, and button toggle |

## Documentation

| Document | Description |
|----------|-------------|
| [`docs/HARDWARE.md`](docs/HARDWARE.md) | Pin mapping, on-board ICs, audio specs |
| [`docs/CONFIGURATION.md`](docs/CONFIGURATION.md) | Full configuration reference for all components |
| [`docs/TROUBLESHOOTING.md`](docs/TROUBLESHOOTING.md) | Common issues and solutions |

## Component Architecture

```
m5echo_base/                    ESPHome external component
├── __init__.py                 Hub component (Python config)
├── m5echo_base_hub.h/.cpp      Hub: shared codec + I2S init
├── M5EchoBase.h/.cpp           Low-level hardware driver
├── es8311.h/.cpp               ES8311 codec driver (Espressif)
├── es8311_reg.h                ES8311 register definitions
├── microphone/
│   ├── __init__.py             Microphone platform config
│   └── m5echo_base_microphone.h/.cpp
└── speaker/
    ├── __init__.py             Speaker platform config
    └── m5echo_base_speaker.h/.cpp
```

The hub pattern ensures the ES8311 codec and I2S bus are initialized once and shared between the speaker and microphone platforms.

## Pin Reference

| Function | GPIO | Notes |
|----------|------|-------|
| I2C SDA | 38 | ES8311 + PI4IOE control bus |
| I2C SCL | 39 | |
| I2S BCK | 8 | Bit clock |
| I2S WS | 6 | Word select (LRCK) |
| I2S DO | 5 | Speaker data out |
| I2S DI | 7 | Microphone data in |
| Button | 41 | Active low, internal pull-up |
| NeoPixel | 35 | WS2812, 4 LEDs |

## Third-Party Code

This project includes vendored driver code with their original licenses:

- **ES8311 codec driver** (`es8311.cpp`, `es8311.h`, `es8311_reg.h`) — Copyright 2015-2022 Espressif Systems, Apache-2.0
- **M5EchoBase driver** (`M5EchoBase.cpp`, `M5EchoBase.h`) — Copyright 2024 M5Stack Technology CO LTD, MIT

## Contributing

Contributions are welcome! Please open an issue first to discuss what you would like to change.

## Acknowledgments

- [M5Stack](https://m5stack.com/) for the AtomS3 Lite and Atomic Echo Base hardware
- [Espressif](https://www.espressif.com/) for the ES8311 codec driver
- [ESPHome](https://esphome.io/) for the incredible home automation firmware framework
- The Home Assistant community for inspiration and testing
