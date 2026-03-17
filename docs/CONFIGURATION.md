# Configuration Reference

## External Component Source

To use this component from a Git repository, add the following to your ESPHome device YAML:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/gpillon/home_assistant_atoms3_lite_echo_base
      ref: main
    components: [m5echo_base]
```

## Component: `m5echo_base`

The hub component that initializes the ES8311 codec, I2S bus, and I2C GPIO expander.

```yaml
m5echo_base:
  id: echo_base_hub
  sample_rate: 16000
  volume_min: 45
  volume_max: 85
```

### Configuration Variables

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `id` | ID | *Required* | Component ID used to reference the hub from speaker/microphone |
| `sample_rate` | int | `16000` | Audio sample rate in Hz. Only `16000` is currently supported |
| `volume_min` | int | `45` | ES8311 register value mapped to user volume 1%. Values below this are inaudible on the Echo Base hardware |
| `volume_max` | int | `85` | ES8311 register value mapped to user volume 100%. Values above this cause distortion on the Echo Base hardware |

### Volume Mapping

The ES8311 codec on the Echo Base has a hardware DAC volume range of 0–100, but only a portion of that range produces clean audio. Below ~45 the output is inaudible, and above ~85 it clips/distorts.

This component automatically maps the user-facing volume (0%–100%) to the usable hardware range:

- **0%** → hardware 0 (true silence)
- **1%** → hardware `volume_min` (45 by default, threshold of audibility)
- **100%** → hardware `volume_max` (85 by default, maximum clean output)

If your specific unit behaves differently, you can tune `volume_min` and `volume_max` to match.

---

## Platform: `speaker` (m5echo_base)

Registers an ESPHome speaker using the Echo Base DAC output.

```yaml
speaker:
  - platform: m5echo_base
    id: echo_speaker
    m5echo_base_id: echo_base_hub
    volume: 80%
```

### Configuration Variables

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `id` | ID | *Required* | Speaker component ID |
| `m5echo_base_id` | ID | *Required* | Reference to the `m5echo_base` hub |
| `volume` | percentage | `80%` | Initial speaker volume (0%–100%) |

---

## Platform: `microphone` (m5echo_base)

Registers an ESPHome microphone using the Echo Base ADC input.

```yaml
microphone:
  - platform: m5echo_base
    id: echo_mic
    m5echo_base_id: echo_base_hub
```

### Configuration Variables

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `id` | ID | *Required* | Microphone component ID |
| `m5echo_base_id` | ID | *Required* | Reference to the `m5echo_base` hub |

---

## Usage with Voice Assistant

This component is designed to work with the ESPHome `voice_assistant` and `media_player` components. A typical voice assistant setup:

```yaml
media_player:
  - platform: speaker
    id: echo_media_player
    name: "Echo Audio"
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

### Notes

- `codec_support_enabled: false` is required because the ESP32-S3 on the AtomS3 Lite does not have enough resources for software codec decoding.
- `channels: [0]` selects the left channel from the stereo I2S stream for the voice pipeline.
- `gain_factor: 4` boosts microphone sensitivity for better wake word detection.
- `buffer_size: 4000` is a balanced value for the limited RAM on this device.

## Usage with LED Feedback

The AtomS3 Lite has a built-in WS2812 NeoPixel strip (4 LEDs on GPIO35) that can be used for voice assistant state feedback:

```yaml
light:
  - platform: esp32_rmt_led_strip
    id: atom_led
    name: "Status LED"
    pin: GPIO35
    chipset: WS2812
    num_leds: 4
    rgb_order: GRB
    default_transition_length: 0s
    effects:
      - pulse:
          name: "Slow Pulse"
          transition_length: 250ms
          update_interval: 250ms
          min_brightness: 30%
          max_brightness: 100%
      - pulse:
          name: "Fast Pulse"
          transition_length: 100ms
          update_interval: 100ms
          min_brightness: 30%
          max_brightness: 100%
```

See the [full example](../examples/voice-assistant.yaml) for a complete voice assistant configuration with LED state feedback.
