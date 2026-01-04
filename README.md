# Rehearsal Room DMX Controller

A simple, standalone Arduino-based DMX lighting controller for rehearsal rooms with 20 predefined scenes.

## Features

- **Standalone Operation**: No PC required - works completely independently
- **Simple Controls**: Toggle switch for master on/off, two buttons for scene navigation
- **20 Preset Scenes**: Organized into working, mood, and show lighting categories
- **Smooth Transitions**: 1-second fade between scenes for professional look
- **Scene Memory**: Automatically remembers last scene after power cycle (EEPROM)
- **Visual Feedback**: 7-segment display shows current scene number
- **Multi-Device Support**: Controls moving heads, LED bars, and additional lighting

## Hardware Requirements

### Controller Components
- Arduino UNO R3 (or compatible)
- CTC-DRA-10-R2 DMX Shield (RS485-based)
- Adafruit 0.56" 4-Digit 7-Segment Display with I2C Backpack
- 1x Toggle Switch (Master On/Off)
- 2x Momentary Push Buttons (Scene navigation)

### DMX Fixtures
- 2x Fun Generation PicoBeam 30 Quad LED (moving heads, 13-channel mode)
- 3x Stairville LED Bar 120/4 RGB DMX (12-channel mode)
- 1x Botex MPX-4LED (dimmer channel for couch lamp)
- 1x Botex MPX-4LED (on/off channel for UV light)

## Installation

### 1. Hardware Setup

#### Arduino Pin Connections
| Component | Arduino Pin |
|-----------|-------------|
| Master On/Off Switch | D2 |
| Previous Scene Button | D3 |
| Next Scene Button | D4 |
| 7-Segment Display (I2C SDA) | A4 |
| 7-Segment Display (I2C SCL) | A5 |
| DMX Output (via Shield) | D0/D1 |

#### DMX Shield Jumper Settings (CTC-DRA-10-R2)
- **EN** (not ̅EN)
- **DE** (not Slave)
- **TX-uart** (not TX-io)

### 2. Software Installation

#### Required Libraries
Install these libraries via Arduino IDE Library Manager:
- `DMXSerial` by Matthias Hertel
- `Adafruit GFX Library`
- `Adafruit LED Backpack`
- `Wire` (included with Arduino IDE)
- `EEPROM` (included with Arduino IDE)

#### Upload the Sketch
1. Open `proberaum_dmx.ino` in Arduino IDE
2. Select **Board**: Arduino UNO
3. Select the correct **Port**
4. Click **Upload**

### 3. DMX Addressing

Configure your DMX fixtures with these start addresses:

| Fixture | DMX Start Address | Channels Used | Mode |
|---------|------------------|---------------|------|
| PicoBeam #1 | 1 | 1-13 | 13ch |
| PicoBeam #2 | 14 | 14-26 | 13ch |
| LED Bar #1 | 27 | 27-38 | 12ch |
| LED Bar #2 | 39 | 39-50 | 12ch |
| LED Bar #3 | 51 | 51-62 | 12ch |
| Couch Lamp (Dimmer) | 63 | 63 | - |
| UV Light (On/Off) | 64 | 64 | - |

## Usage

### Basic Operation
1. **Power on** the Arduino controller
   - Controller automatically restores the last active scene from memory
2. Use the **Master Switch** to enable/disable all lights (displays "OFF" when disabled)
   - Turning off fades smoothly to blackout
3. Press **Next/Previous buttons** to cycle through scenes 0-19
   - Scenes fade smoothly over 1 second
   - Current scene is saved to memory automatically
4. The display shows the current scene number (e.g., "P.05" for scene 5)

### Scene Categories

#### Working Scenes (0-4)
- **0 - Blackout**: All lights off
- **1 - Work Light**: Bright white lighting for rehearsals
- **2 - Work Dim**: Dimmed white lighting
- **3 - Rehearsal Std**: Warm, cozy standard rehearsal lighting
- **4 - Rehearsal Focus**: Focused white spots with subtle ambience

#### Mood Scenes (5-9)
- **5 - Chill Blue**: Cool blue atmosphere with warm couch accent
- **6 - Warm Red**: Warm red/orange ambience
- **7 - Green Nature**: Natural green tones
- **8 - Purple Haze**: Purple/magenta mood
- **9 - UV Party**: UV blacklight with blue accents

#### Show Scenes (10-18)
- **10 - Show Split**: Red/blue color split
- **11 - Show Cyan Mag**: Cyan and magenta contrast
- **12 - Show Full White**: Maximum brightness white
- **13 - Show Strobe**: Subtle strobe effect
- **14 - Show Cross**: Crossed moving head positions
- **15 - Show Rainbow**: Rainbow colors across bars
- **16 - Show Yellow**: Bright yellow energy
- **17 - Show Wide**: Wide spread moving head positions
- **18 - Show Floor**: Downward-facing moving heads

#### Special Scenes
- **19 - Lounge**: Couch lamp only with subtle LED bar accents

## Customization

### Adding New Scenes
1. Open `proberaum_dmx.ino`
2. Increase `SCENE_COUNT` constant
3. Add a new scene entry to the `scenes[]` array
4. Upload the modified sketch

### Adjusting Pan/Tilt Values
Pan and tilt values depend on the physical mounting position of your moving heads:
- **Pan**: 0 = full left, 128 = center, 255 = full right (540° range)
- **Tilt**: 0 = up, 128 = horizontal, 255 = down (230° range)

Adjust these values in the scene definitions based on your room layout.

### Adding More DMX Fixtures
1. Update `TOTAL_CHANNELS` constant
2. Define new DMX start addresses
3. Create or extend data structures for the new fixtures
4. Modify `outputScene()` function to output to the new channels

## Technical Details

- **Memory**: Scenes stored in PROGMEM (Flash) to conserve RAM
- **Scene Transitions**: 1-second linear fade with 50 interpolation steps
  - Per-fixture fade control via boolean flags in scene definitions
  - UV light: instant snap (better visual effect)
  - Couch light: smooth fade
  - PicoBeams and LED bars: always fade
- **Scene Memory**: EEPROM-based persistence with magic number validation
- **Display Update**: Immediate when changing scenes
- **Button Debouncing**: 50ms debounce delay implemented
- **Fade Customization**: Adjustable via `FADE_DURATION_MS` and `FADE_STEPS` constants

## Known Limitations

- Fade transitions use linear interpolation (no easing curves)
- Scenes can only be modified by uploading new code (no runtime programming)
- LED Bar segment 4 has limited color control in 12-channel mode (R only, G/B missing)
- No MIDI input support
- EEPROM has ~100,000 write cycle limit (scene changes only, very long lifespan)

## Future Enhancements

Potential improvements for future versions:
- ✅ Soft fade transitions between scenes (implemented)
- ✅ EEPROM storage to remember last active scene (implemented)
- Easing curves for fades (ease-in/ease-out)
- Adjustable fade time via buttons
- Manual dimmer control via rotary encoder
- MIDI input for external control
- OLED display showing scene names instead of numbers

## Troubleshooting

### Display shows nothing
- Check I2C connections (SDA/SCL)
- Verify display address is 0x70
- Adjust brightness with `display.setBrightness()`

### No DMX output
- Verify DMX shield jumper settings
- Check DMX cable connections
- Ensure fixtures are powered and addressed correctly

### Buttons not responding
- Check pull-up resistor configuration
- Verify button wiring (should pull pin LOW when pressed)
- Test button continuity with multimeter

## Resources

- [DMXSerial Library Documentation](https://github.com/mathertel/DMXSerial)
- [PicoBeam 30 Manual](https://images.static-thomann.de/pics/atg/atgdata/document/manual/372682_c_372682_v3_en_online.pdf)
- [LED Bar 120/4 Manual](https://images.static-thomann.de/pics/atg/atgdata/document/manual/355287_c_355287_v2_de_online.pdf)
- [Open Fixture Library - PicoBeam](https://open-fixture-library.org/fun-generation/picobeam-30-quad-led)

## License

GNU General Public License v3.0 - See LICENSE file for details

## Contributing

This is a personal project for a specific rehearsal room setup. Feel free to fork and adapt it for your own needs.

## Author

Built for a basement rehearsal room with the goal of simple, reliable lighting control without requiring a computer.
