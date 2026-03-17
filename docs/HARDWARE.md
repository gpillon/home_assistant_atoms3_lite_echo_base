# Hardware Setup

## Required Hardware

| Component | Description |
|-----------|-------------|
| **M5Stack AtomS3 Lite** | ESP32-S3 based microcontroller (SKU: C124) |
| **M5Stack Atomic Echo Base** | Audio I/O base with ES8311 codec and built-in speaker/microphone (SKU: A136) |

## Assembly

1. Plug the AtomS3 Lite directly onto the Atomic Echo Base via the Grove/ATOM socket.
2. Connect power via USB-C on the AtomS3 Lite.

No soldering or additional wiring is required.

## Pin Mapping

The Atomic Echo Base uses the following GPIOs on the AtomS3 Lite:

### I2C Bus (ES8311 Codec Control)

| Signal | GPIO |
|--------|------|
| SDA    | 38   |
| SCL    | 39   |

### I2S Bus (Audio Data)

| Signal | GPIO | Direction      |
|--------|------|----------------|
| BCK    | 8    | Bit Clock      |
| WS     | 6    | Word Select    |
| DO     | 5    | Data Out (SPK) |
| DI     | 7    | Data In (MIC)  |

### Other

| Signal     | GPIO | Notes                          |
|------------|------|--------------------------------|
| Button     | 41   | Built-in button (active low, internal pull-up) |
| NeoPixel   | 35   | WS2812 RGB LED (4 LEDs on Echo Base) |

## On-Board ICs

| IC | Address | Role |
|----|---------|------|
| ES8311 | 0x18 | Audio codec (ADC + DAC) |
| PI4IOE5V6408 | 0x43 | I2C GPIO expander (speaker mute control) |

## Audio Specifications

- **Codec**: ES8311 (Everest Semiconductor)
- **Sample Rate**: 16 kHz (optimized for voice assistant)
- **Bit Depth**: 16-bit
- **Channels**: Stereo (2ch I2S)
- **Speaker**: Built-in on the Echo Base
- **Microphone**: Built-in MEMS microphone on the Echo Base
