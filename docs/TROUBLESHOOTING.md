# Troubleshooting

## Common Issues

### No audio output (speaker silent)

1. **Check volume**: Make sure the speaker volume is set above 0% in your YAML config.
2. **Check mute via PI4IOE**: The I2C GPIO expander (PI4IOE5V6408 at 0x43) controls the speaker amplifier enable. If I2C initialization fails, the speaker stays muted. Check logs for `PI4IOE5V6408 initialization failed`.
3. **Check I2C bus**: Ensure no other component is conflicting on the I2C bus (SDA=38, SCL=39).

### No microphone input / Voice assistant not hearing

1. **Check gain_factor**: In the `voice_assistant` microphone config, try increasing `gain_factor` (e.g., from 4 to 8).
2. **Check channel selection**: The microphone data comes on stereo I2S. Use `channels: [0]` in the voice_assistant config.
3. **Check logs**: Look for `Microphone capture failed` in the ESPHome logs.

### ES8311 codec initialization failed

- Verify the I2C connection between AtomS3 Lite and Echo Base is solid (device properly seated).
- Check that no other I2C devices are conflicting at address 0x18.
- Make sure `Wire` and `ESP_I2S` libraries are included (handled automatically by the component).

### Build fails: `ESP_I2S.h: No such file`

This component requires the Arduino framework with ESP-IDF v5.x. Make sure your board config uses:

```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino
```

### Voice assistant connects but does not respond

1. Make sure you have a voice assistant pipeline configured in Home Assistant (Settings > Voice assistants).
2. Check that `use_wake_word: true` is set and a wake word engine is installed (e.g., openWakeWord via the Wyoming integration).
3. Verify the device appears in Home Assistant and the API connection is active.

### Audio playback is distorted or choppy

- `buffer_size: 4000` in the media_player config is recommended. Increasing it may help but uses more RAM.
- Keep `codec_support_enabled: false` — enabling it on the AtomS3 Lite causes RAM issues.
- Make sure the sample rate matches across all components (16000 Hz).

## Getting Logs

Enable verbose logging for the component:

```yaml
logger:
  level: VERBOSE
  logs:
    m5echo_base.hub: DEBUG
    m5echo_base.speaker: DEBUG
    m5echo_base.microphone: DEBUG
```

Connect via USB or OTA and monitor the logs in the ESPHome dashboard or CLI:

```bash
esphome logs your-device.yaml
```
