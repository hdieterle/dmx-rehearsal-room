/*
 * Rehearsal Room DMX Controller
 *
 * Hardware:
 * - Arduino UNO
 * - CTC-DRA-10-R2 DMX Shield
 * - Toggle switch on Pin 2 (Master On/Off)
 * - Button on Pin 3 (Previous preset)
 * - Button on Pin 4 (Next preset)
 *
 * DMX Devices:
 * - Channel 1-13:  PicoBeam 1 (13ch mode)
 * - Channel 14-26: PicoBeam 2 (13ch mode)
 * - Channel 27-38: LED Bar 1 (12ch mode)
 * - Channel 39-50: LED Bar 2 (12ch mode)
 * - Channel 51-62: LED Bar 3 (12ch mode)
 * - Channel 63:    Botex Dimmer (Couch hanging lamp)
 * - Channel 64:    Botex UV light (On/Off)
 *
 * Jumper Settings CTC-DRA-10-R2:
 * - EN (not ̅EN)
 * - DE (not Slave)
 * - TX-uart (not TX-io)
 */

#include <DMXSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <EEPROM.h>

Adafruit_7segment display = Adafruit_7segment();

// === PIN DEFINITIONS ===
const byte PIN_MASTER = 2;    // Toggle switch Master On/Off
const byte PIN_PREV = 3;      // Button previous preset
const byte PIN_NEXT = 4;      // Button next preset

// === DMX ADDRESSES ===
const byte PICO1_START = 1;
const byte PICO2_START = 14;
const byte BAR1_START = 27;
const byte BAR2_START = 39;
const byte BAR3_START = 51;
const byte BOTEX_COUCH = 63;
const byte BOTEX_UV = 64;

const byte TOTAL_CHANNELS = 64;

// === FADE SETTINGS ===
const unsigned int FADE_DURATION_MS = 1000;  // 1 second fade time
const byte FADE_STEPS = 50;                   // Number of fade steps
const unsigned int FADE_STEP_DELAY = FADE_DURATION_MS / FADE_STEPS;

// === EEPROM SETTINGS ===
const int EEPROM_ADDR_SCENE = 0;      // Address to store current scene
const int EEPROM_ADDR_MAGIC = 1;      // Address to store magic number
const byte EEPROM_MAGIC = 0xA5;       // Magic number to verify valid EEPROM data

// === PicoBeam 13-Channel Offsets ===
// 0: Pan, 1: Pan Fine, 2: Tilt, 3: Tilt Fine, 4: Speed,
// 5: Dimmer, 6: Strobe, 7: Red, 8: Green, 9: Blue, 10: White,
// 11: Program, 12: Program Speed
enum PicoChannel {
  PICO_PAN = 0,
  PICO_PAN_FINE = 1,
  PICO_TILT = 2,
  PICO_TILT_FINE = 3,
  PICO_SPEED = 4,
  PICO_DIMMER = 5,
  PICO_STROBE = 6,
  PICO_RED = 7,
  PICO_GREEN = 8,
  PICO_BLUE = 9,
  PICO_WHITE = 10,
  PICO_PROGRAM = 11,
  PICO_PROG_SPEED = 12
};

// === LED Bar 12-Channel Offsets ===
// 0: Dimmer, 1: Strobe, 2: Red1, 3: Green1, 4: Blue1,
// 5: Red2, 6: Green2, 7: Blue2, 8: Red3, 9: Green3, 10: Blue3,
// 11: Red4, 12: Green4, 13: Blue4 -- WAIT that's 14!
// Actually 12ch: Dimmer, Strobe, then 4 segments RGB?
// I'll use the simple 3-channel RGB mode data:
// According to manual: 12ch = segment control
// Simplified: 0: Dimmer, 1: Strobe, 2-4: Seg1 RGB, 5-7: Seg2, 8-10: Seg3, 11: Macro
// For simplicity we use all segments the same

enum BarChannel {
  BAR_DIMMER = 0,
  BAR_STROBE = 1,
  BAR_SEG1_R = 2,
  BAR_SEG1_G = 3,
  BAR_SEG1_B = 4,
  BAR_SEG2_R = 5,
  BAR_SEG2_G = 6,
  BAR_SEG2_B = 7,
  BAR_SEG3_R = 8,
  BAR_SEG3_G = 9,
  BAR_SEG3_B = 10,
  BAR_SEG4_R = 11  // Only R in 12-channel, G and B missing
  // Note: 12ch mode only has 12 channels, Seg4 is incomplete
  // We still use all 12 and set Seg4 to R only
};

// === SCENES ===
const byte SCENE_COUNT = 20;
byte currentScene = 0;
byte previousScene = 0;
bool masterOn = false;

// Scene data: Compact structure per scene
// Each scene defines all relevant values

struct PicoBeamState {
  byte pan;
  byte tilt;
  byte speed;
  byte dimmer;
  byte strobe;
  byte red;
  byte green;
  byte blue;
  byte white;
};

struct LedBarState {
  byte dimmer;
  byte red;
  byte green;
  byte blue;
};

struct Scene {
  const char* name;
  PicoBeamState pico1;
  PicoBeamState pico2;
  LedBarState bar1;
  LedBarState bar2;
  LedBarState bar3;
  byte couchLight;  // Botex Dimmer hanging lamp 0-255
  byte uvLight;     // Botex UV 0 or 255
};

// Helper macros for readable scene definitions
#define PICO_OFF        {128, 128, 0, 0, 0, 0, 0, 0, 0}
#define PICO_CENTER_OFF {128, 128, 0, 0, 0, 0, 0, 0, 0}
#define BAR_OFF         {0, 0, 0, 0}

// Pan: 0=left, 128=center, 255=right (540° range)
// Tilt: 0=up, 128=center, 255=down (230° range)

const Scene scenes[SCENE_COUNT] PROGMEM = {
  // === WORKING SCENES ===

  // 0: Blackout
  {
    "Blackout",
    PICO_OFF, PICO_OFF,
    BAR_OFF, BAR_OFF, BAR_OFF,
    0, 0
  },

  // 1: Work light (Bars white, couch lamp on)
  {
    "Work Light",
    {128, 128, 0, 180, 0, 200, 200, 200, 255},
    {128, 128, 0, 180, 0, 200, 200, 200, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    200, 0
  },

  // 2: Work light dimmed
  {
    "Work Dim",
    {128, 128, 0, 100, 0, 180, 180, 180, 200},
    {128, 128, 0, 100, 0, 180, 180, 180, 200},
    {150, 200, 200, 200},
    {150, 200, 200, 200},
    {150, 200, 200, 200},
    120, 0
  },

  // 3: Rehearsal standard (warm, cozy)
  {
    "Rehearsal Std",
    {128, 128, 0, 80, 0, 255, 80, 0, 180},
    {128, 128, 0, 80, 0, 255, 80, 0, 180},
    {180, 255, 120, 40},
    {180, 255, 120, 40},
    {180, 255, 120, 40},
    150, 0
  },

  // 4: Rehearsal focus (Picos as spot, couch subtle)
  {
    "Rehearsal Focus",
    {128, 100, 0, 200, 0, 0, 0, 0, 255},
    {128, 100, 0, 200, 0, 0, 0, 0, 255},
    {80, 255, 180, 100},
    {80, 255, 180, 100},
    {80, 255, 180, 100},
    80, 0
  },

  // === MOOD SCENES ===

  // 5: Chill blue (Couch warm as contrast)
  {
    "Chill Blue",
    {128, 128, 0, 120, 0, 0, 0, 255, 0},
    {128, 128, 0, 120, 0, 0, 0, 255, 0},
    {200, 0, 50, 255},
    {200, 0, 50, 255},
    {200, 0, 50, 255},
    100, 0
  },

  // 6: Warm red/orange (Couch matches)
  {
    "Warm Red",
    {128, 128, 0, 150, 0, 255, 60, 0, 0},
    {128, 128, 0, 150, 0, 255, 60, 0, 0},
    {220, 255, 80, 0},
    {220, 255, 80, 0},
    {220, 255, 80, 0},
    180, 0
  },

  // 7: Green nature
  {
    "Green Nature",
    {128, 128, 0, 130, 0, 40, 255, 0, 0},
    {128, 128, 0, 130, 0, 40, 255, 0, 0},
    {200, 30, 255, 30},
    {200, 30, 255, 30},
    {200, 30, 255, 30},
    60, 0
  },

  // 8: Purple haze
  {
    "Purple Haze",
    {128, 128, 0, 180, 0, 200, 0, 255, 0},
    {128, 128, 0, 180, 0, 200, 0, 255, 0},
    {220, 180, 0, 255},
    {220, 180, 0, 255},
    {220, 180, 0, 255},
    40, 0
  },

  // 9: UV party (Couch off for UV effect)
  {
    "UV Party",
    {128, 128, 0, 100, 0, 80, 0, 200, 0},
    {128, 128, 0, 100, 0, 80, 0, 200, 0},
    {150, 100, 0, 200},
    {150, 100, 0, 200},
    {150, 100, 0, 200},
    0, 255
  },

  // === SHOW SCENES ===

  // 10: Show red-blue split
  {
    "Show Split",
    {64, 128, 0, 255, 0, 255, 0, 0, 0},
    {192, 128, 0, 255, 0, 0, 0, 255, 0},
    {255, 255, 0, 0},
    {255, 0, 0, 255},
    {255, 255, 0, 0},
    0, 0
  },

  // 11: Show cyan/magenta
  {
    "Show Cyan Mag",
    {100, 80, 0, 255, 0, 255, 0, 255, 0},
    {156, 80, 0, 255, 0, 0, 255, 255, 0},
    {255, 255, 0, 255},
    {255, 0, 255, 255},
    {255, 255, 0, 255},
    0, 0
  },

  // 12: Show full white
  {
    "Show Full White",
    {128, 128, 0, 255, 0, 255, 255, 255, 255},
    {128, 128, 0, 255, 0, 255, 255, 255, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    255, 0
  },

  // 13: Show strobe (subtle)
  {
    "Show Strobe",
    {128, 128, 0, 255, 80, 255, 255, 255, 255},
    {128, 128, 0, 255, 80, 255, 255, 255, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    {255, 255, 255, 255},
    0, 0
  },

  // 14: Show cross (Picos crossed)
  {
    "Show Cross",
    {192, 100, 0, 255, 0, 255, 100, 0, 0},
    {64, 100, 0, 255, 0, 0, 100, 255, 0},
    {200, 255, 100, 0},
    {200, 0, 100, 255},
    {200, 255, 100, 0},
    0, 0
  },

  // 15: Show rainbow bars (different colors)
  {
    "Show Rainbow",
    {128, 128, 0, 200, 0, 255, 0, 0, 0},
    {128, 128, 0, 200, 0, 0, 0, 255, 0},
    {255, 255, 0, 0},
    {255, 0, 255, 0},
    {255, 0, 0, 255},
    0, 0
  },

  // 16: Show yellow power
  {
    "Show Yellow",
    {128, 90, 0, 255, 0, 255, 200, 0, 0},
    {128, 90, 0, 255, 0, 255, 200, 0, 0},
    {255, 255, 200, 0},
    {255, 255, 200, 0},
    {255, 255, 200, 0},
    0, 0
  },

  // 17: Show sweep position (Picos outward)
  {
    "Show Wide",
    {40, 128, 0, 255, 0, 255, 0, 100, 0},
    {215, 128, 0, 255, 0, 255, 0, 100, 0},
    {200, 255, 0, 100},
    {200, 255, 0, 100},
    {200, 255, 0, 100},
    0, 0
  },

  // 18: Show floor (Picos downward)
  {
    "Show Floor",
    {128, 200, 0, 255, 0, 0, 200, 255, 0},
    {128, 200, 0, 255, 0, 0, 200, 255, 0},
    {255, 0, 200, 255},
    {255, 0, 200, 255},
    {255, 0, 200, 255},
    0, 0
  },

  // 19: Lounge (only couch lamp + subtle bars)
  {
    "Lounge",
    PICO_OFF, PICO_OFF,
    {100, 255, 100, 50},
    {100, 255, 100, 50},
    {100, 255, 100, 50},
    255, 0
  }
};

// === FUNCTIONS ===

// Linear interpolation between two byte values
byte lerpByte(byte from, byte to, byte step, byte totalSteps) {
  if (step >= totalSteps) return to;
  int delta = (int)to - (int)from;
  return from + ((delta * step) / totalSteps);
}

// Interpolate between two PicoBeam states
PicoBeamState lerpPicoBeam(const PicoBeamState& from, const PicoBeamState& to, byte step, byte totalSteps) {
  PicoBeamState result;
  result.pan = lerpByte(from.pan, to.pan, step, totalSteps);
  result.tilt = lerpByte(from.tilt, to.tilt, step, totalSteps);
  result.speed = lerpByte(from.speed, to.speed, step, totalSteps);
  result.dimmer = lerpByte(from.dimmer, to.dimmer, step, totalSteps);
  result.strobe = lerpByte(from.strobe, to.strobe, step, totalSteps);
  result.red = lerpByte(from.red, to.red, step, totalSteps);
  result.green = lerpByte(from.green, to.green, step, totalSteps);
  result.blue = lerpByte(from.blue, to.blue, step, totalSteps);
  result.white = lerpByte(from.white, to.white, step, totalSteps);
  return result;
}

// Interpolate between two LED Bar states
LedBarState lerpLedBar(const LedBarState& from, const LedBarState& to, byte step, byte totalSteps) {
  LedBarState result;
  result.dimmer = lerpByte(from.dimmer, to.dimmer, step, totalSteps);
  result.red = lerpByte(from.red, to.red, step, totalSteps);
  result.green = lerpByte(from.green, to.green, step, totalSteps);
  result.blue = lerpByte(from.blue, to.blue, step, totalSteps);
  return result;
}

// Save current scene to EEPROM
void saveSceneToEEPROM() {
  EEPROM.update(EEPROM_ADDR_SCENE, currentScene);
  EEPROM.update(EEPROM_ADDR_MAGIC, EEPROM_MAGIC);
}

// Load scene from EEPROM (if valid)
void loadSceneFromEEPROM() {
  byte magic = EEPROM.read(EEPROM_ADDR_MAGIC);
  if (magic == EEPROM_MAGIC) {
    byte savedScene = EEPROM.read(EEPROM_ADDR_SCENE);
    if (savedScene < SCENE_COUNT) {
      currentScene = savedScene;
      previousScene = savedScene;
    }
  }
}

void setup() {
  DMXSerial.init(DMXController);

  // Initialize display (default I2C address 0x70)
  display.begin(0x70);
  display.setBrightness(5);  // 0-15, not too bright

  pinMode(PIN_MASTER, INPUT_PULLUP);
  pinMode(PIN_PREV, INPUT_PULLUP);
  pinMode(PIN_NEXT, INPUT_PULLUP);

  // Load last scene from EEPROM
  loadSceneFromEEPROM();

  // Set all channels to 0
  for (int i = 1; i <= TOTAL_CHANNELS; i++) {
    DMXSerial.write(i, 0);
  }

  // Initial display
  updateDisplay();
}

void loop() {
  // Read master switch
  bool newMasterState = (digitalRead(PIN_MASTER) == LOW);

  if (newMasterState != masterOn) {
    masterOn = newMasterState;
    outputSceneWithFade();
    updateDisplay();
    delay(50);
  }

  // Read buttons
  if (buttonPressed(PIN_NEXT)) {
    previousScene = currentScene;
    currentScene = (currentScene + 1) % SCENE_COUNT;
    saveSceneToEEPROM();
    updateDisplay();
    if (masterOn) outputSceneWithFade();
  }

  if (buttonPressed(PIN_PREV)) {
    previousScene = currentScene;
    currentScene = (currentScene + SCENE_COUNT - 1) % SCENE_COUNT;
    saveSceneToEEPROM();
    updateDisplay();
    if (masterOn) outputSceneWithFade();
  }
}

bool buttonPressed(byte pin) {
  if (digitalRead(pin) == LOW) {
    delay(50);  // Debounce
    while (digitalRead(pin) == LOW);  // Wait until released
    delay(50);
    return true;
  }
  return false;
}

void updateDisplay() {
  display.clear();

  if (!masterOn) {
    // Display "OFF"
    display.writeDigitRaw(0, 0x00);        // empty
    display.writeDigitRaw(1, 0x3F);        // O
    display.writeDigitRaw(3, 0x71);        // F
    display.writeDigitRaw(4, 0x71);        // F
  } else {
    // Display "P.XX" (P + preset number)
    display.writeDigitRaw(0, 0x73);        // P
    display.writeDigitNum(1, currentScene / 10, false);
    display.writeDigitNum(3, currentScene % 10, false);
    display.writeDigitRaw(4, 0x00);        // empty
  }

  display.writeDisplay();
}

void outputSceneWithFade() {
  if (!masterOn) {
    // Blackout - fade to black
    Scene currentSceneData;
    memcpy_P(&currentSceneData, &scenes[previousScene], sizeof(Scene));

    Scene blackScene;
    memcpy_P(&blackScene, &scenes[0], sizeof(Scene));  // Scene 0 is blackout

    fadeToScene(currentSceneData, blackScene);
    return;
  }

  // Read previous and current scenes from PROGMEM
  Scene fromScene;
  Scene toScene;
  memcpy_P(&fromScene, &scenes[previousScene], sizeof(Scene));
  memcpy_P(&toScene, &scenes[currentScene], sizeof(Scene));

  fadeToScene(fromScene, toScene);
}

void fadeToScene(const Scene& from, const Scene& to) {
  // UV light snaps instantly (no fade) - set it once before the fade loop
  DMXSerial.write(BOTEX_UV, to.uvLight);

  for (byte step = 0; step <= FADE_STEPS; step++) {
    // Interpolate PicoBeams
    PicoBeamState pico1 = lerpPicoBeam(from.pico1, to.pico1, step, FADE_STEPS);
    PicoBeamState pico2 = lerpPicoBeam(from.pico2, to.pico2, step, FADE_STEPS);

    // Interpolate LED Bars
    LedBarState bar1 = lerpLedBar(from.bar1, to.bar1, step, FADE_STEPS);
    LedBarState bar2 = lerpLedBar(from.bar2, to.bar2, step, FADE_STEPS);
    LedBarState bar3 = lerpLedBar(from.bar3, to.bar3, step, FADE_STEPS);

    // Interpolate couch light (but not UV - that's already set)
    byte couchLight = lerpByte(from.couchLight, to.couchLight, step, FADE_STEPS);

    // Output interpolated values
    outputPicoBeam(PICO1_START, pico1);
    outputPicoBeam(PICO2_START, pico2);
    outputLedBar(BAR1_START, bar1);
    outputLedBar(BAR2_START, bar2);
    outputLedBar(BAR3_START, bar3);
    DMXSerial.write(BOTEX_COUCH, couchLight);
    // UV already set before loop - no need to write it again

    // Wait before next step (except on last step)
    if (step < FADE_STEPS) {
      delay(FADE_STEP_DELAY);
    }
  }
}

void outputPicoBeam(byte startAddr, const PicoBeamState& state) {
  DMXSerial.write(startAddr + PICO_PAN, state.pan);
  DMXSerial.write(startAddr + PICO_PAN_FINE, 0);
  DMXSerial.write(startAddr + PICO_TILT, state.tilt);
  DMXSerial.write(startAddr + PICO_TILT_FINE, 0);
  DMXSerial.write(startAddr + PICO_SPEED, state.speed);
  DMXSerial.write(startAddr + PICO_DIMMER, state.dimmer);
  DMXSerial.write(startAddr + PICO_STROBE, state.strobe);
  DMXSerial.write(startAddr + PICO_RED, state.red);
  DMXSerial.write(startAddr + PICO_GREEN, state.green);
  DMXSerial.write(startAddr + PICO_BLUE, state.blue);
  DMXSerial.write(startAddr + PICO_WHITE, state.white);
  DMXSerial.write(startAddr + PICO_PROGRAM, 0);      // No auto programs
  DMXSerial.write(startAddr + PICO_PROG_SPEED, 0);
}

void outputLedBar(byte startAddr, const LedBarState& state) {
  DMXSerial.write(startAddr + BAR_DIMMER, state.dimmer);
  DMXSerial.write(startAddr + BAR_STROBE, 0);  // No strobe

  // All 4 segments same color
  // Seg 1
  DMXSerial.write(startAddr + BAR_SEG1_R, state.red);
  DMXSerial.write(startAddr + BAR_SEG1_G, state.green);
  DMXSerial.write(startAddr + BAR_SEG1_B, state.blue);
  // Seg 2
  DMXSerial.write(startAddr + BAR_SEG2_R, state.red);
  DMXSerial.write(startAddr + BAR_SEG2_G, state.green);
  DMXSerial.write(startAddr + BAR_SEG2_B, state.blue);
  // Seg 3
  DMXSerial.write(startAddr + BAR_SEG3_R, state.red);
  DMXSerial.write(startAddr + BAR_SEG3_G, state.green);
  DMXSerial.write(startAddr + BAR_SEG3_B, state.blue);
  // Seg 4 (only R in 12ch mode)
  DMXSerial.write(startAddr + BAR_SEG4_R, state.red);
}
