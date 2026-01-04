# Rehearsal Room DMX Controller

Arduino-based DMX lighting controller for a rehearsal room with predefined scenes.

## Project Overview

Simple, standalone DMX controller without PC connection. Operation via toggle switch (Master On/Off) and two buttons (preset previous/next). Current scene displayed on 7-segment display.

**Features:**
- Smooth 1-second fade transitions between scenes
- EEPROM memory: Restores last active scene after power cycle
- 20 predefined scenes optimized for rehearsal room use

## Hardware

### Controller
- **Arduino UNO R3** (or compatible clone)
- **CTC-DRA-10-R2** DMX Shield (RS485-based)
- **Adafruit 0.56" 4-Digit 7-Segment Display** with I2C Backpack (yellow)

### Controls
- 1x Toggle switch (Master On/Off)
- 2x Momentary buttons (Preset previous/next)

### DMX Devices (64 channels total)

| Device | Start Address | Channels | Mode |
|--------|--------------|----------|------|
| Fun Generation PicoBeam 30 Quad LED #1 | 1 | 1–13 | 13ch |
| Fun Generation PicoBeam 30 Quad LED #2 | 14 | 14–26 | 13ch |
| Stairville LED Bar 120/4 RGB DMX #1 | 27 | 27–38 | 12ch |
| Stairville LED Bar 120/4 RGB DMX #2 | 39 | 39–50 | 12ch |
| Stairville LED Bar 120/4 RGB DMX #3 | 51 | 51–62 | 12ch |
| Botex MPX-4LED Channel 1 (Couch hanging lamp) | 63 | 63 | Dimmer |
| Botex MPX-4LED Channel 2 (UV light) | 64 | 64 | On/Off |

### Arduino UNO Pin Assignment

| Function | Pin |
|----------|-----|
| Master On/Off (Toggle switch) | D2 |
| Preset previous (Button) | D3 |
| Preset next (Button) | D4 |
| Display SDA | A4 |
| Display SCL | A5 |
| DMX (via Shield) | D0/D1 (UART) |

### CTC-DRA-10-R2 Jumper Settings
- EN (not ̅EN)
- DE (not Slave)
- TX-uart (not TX-io)

## Software

### Dependencies (Arduino Library Manager)
- `DMXSerial` – DMX output
- `Adafruit GFX Library` – Display foundation
- `Adafruit LED Backpack` – 7-segment control
- `Wire` – I2C (included in Arduino Core)
- `EEPROM` – Scene memory (included in Arduino Core)

### File Structure
```
proberaum_dmx/
├── proberaum_dmx.ino    # Main sketch
├── CLAUDE.md            # This file
└── README.md            # User documentation (optional)
```

## Scene Structure

20 predefined scenes (0–19), divided into:

- **0–4:** Working scenes (Blackout, Work light, Rehearsal)
- **5–9:** Mood scenes (Chill, Warm, Nature, Purple, UV)
- **10–18:** Show scenes (Split, Rainbow, Strobe, etc.)
- **19:** Lounge (Couch lamp + subtle bars)

### Scene Data Structure

```cpp
struct PicoBeamState {
  byte pan;      // 0-255 (0=left, 128=center, 255=right, 540° range)
  byte tilt;     // 0-255 (0=up, 128=center, 255=down, 230° range)
  byte speed;    // 0-255 (Pan/Tilt speed, 0=fast)
  byte dimmer;   // 0-255
  byte strobe;   // 0=off, 10-255=strobe
  byte red;      // 0-255
  byte green;    // 0-255
  byte blue;     // 0-255
  byte white;    // 0-255
};

struct LedBarState {
  byte dimmer;   // 0-255
  byte red;      // 0-255
  byte green;    // 0-255
  byte blue;     // 0-255
};

struct Scene {
  const char* name;
  PicoBeamState pico1;
  PicoBeamState pico2;
  LedBarState bar1;
  LedBarState bar2;
  LedBarState bar3;
  byte couchLight;      // 0-255
  bool couchFade;       // Should couch light fade? (typically true)
  byte uvLight;         // 0 or 255
  bool uvFade;          // Should UV light fade? (typically false)
};
```

## Typical Modifications

### Add New Scene
1. Increase `SCENE_COUNT`
2. Add new entry to `scenes[]` array
3. Scenes are stored in PROGMEM (flash), not RAM

### Adjust Pan/Tilt Values
Values depend on the physical mounting position of the PicoBeams. Calibrate on-site after installation:
- Pan 128 = center
- Tilt 128 = horizontal
- Adjust values experimentally

### Add More DMX Devices
1. Adjust `TOTAL_CHANNELS`
2. Define new start address
3. Extend or create new structs
4. Extend `outputScene()`

## Features

### Smooth Scene Transitions
- 1-second linear fade between scenes (configurable)
- 50 interpolation steps for smooth color mixing
- Fades all channels: pan, tilt, colors, dimmer, etc.
- Per-fixture fade control via `couchFade` and `uvFade` flags
  - UV light: `uvFade = false` → snaps instantly for better visual effect
  - Couch light: `couchFade = true` → smooth fade
- Adjustable via `FADE_DURATION_MS` and `FADE_STEPS` constants

### EEPROM Scene Memory
- Automatically saves current scene to EEPROM on change
- Restores last active scene on power-up
- Magic number validation to detect first run
- Uses `EEPROM.update()` to minimize write cycles

## Known Limitations

- LED Bar 12-channel mode: Segment 4 only has R channel, G/B missing
- Scenes only modifiable in code, no runtime programming
- No MIDI input (intentionally kept simple)
- Fade is linear (not eased curves)

## Possible Extensions

- [x] Soft fade between scenes ✓ Implemented
- [x] EEPROM: Restore last scene on startup ✓ Implemented
- [ ] Easing curves for fades (ease-in/ease-out)
- [ ] Adjustable fade time via buttons (hold to configure)
- [ ] Encoder for manual dimmer control
- [ ] MIDI input for external control
- [ ] Scene names on OLED instead of just number

## Resources

- [Fun Generation PicoBeam 30 Quad LED Manual](https://images.static-thomann.de/pics/atg/atgdata/document/manual/372682_c_372682_v3_en_online.pdf)
- [Stairville LED Bar 120/4 RGB DMX Manual](https://images.static-thomann.de/pics/atg/atgdata/document/manual/355287_c_355287_v2_de_online.pdf)
- [DMXSerial Library](https://github.com/mathertel/DMXSerial)
- [Open Fixture Library - PicoBeam](https://open-fixture-library.org/fun-generation/picobeam-30-quad-led)

## Context

This controller is built for the basement rehearsal room. The couch lamp is a hanging lamp above the couch (mood lighting), not main work light. The three LED bars are individually addressed so they can also be used for DJ shows with SoundSwitch.
