# DMX Controller Case - Assembly Guide

Step-by-step instructions for assembling your custom DMX controller case.

## Required Components

### Electronics
- [x] Arduino UNO R3
- [x] CTC-DRA-10-R2 DMX Shield
- [x] Adafruit 0.56" 4-Digit 7-Segment Display (yellow, with I2C backpack)
- [x] USB-C panel mount extension cable
- [x] SS-22L05 DPDT slide switch (programming mode selector)
- [x] DSQ14-R toggle switch (Master On/Off)
- [x] 2× DTQ14-S momentary push buttons (Preset Prev/Next)
- [x] 2× Neutrik XLR-3 D-series panel mount connectors
- [x] DC barrel jack panel mount (2.1mm, 8mm mounting hole)
- [x] Dupont jumper wires (male-to-female, 10-15cm length)

### Hardware
- [x] 4× M3 × 20mm screws (case assembly)
- [x] 4× M3 nuts or heat-set inserts
- [x] 4× M3 × 8mm screws (Arduino mounting)
- [x] 2× M2 × 8mm screws + nuts (slide switch)
- [x] 4× M2.5 × 8mm screws (display mounting)
- [x] Wire (22-24 AWG) for switch wiring
- [x] Heat shrink tubing

### Tools
- Soldering iron (for switch wiring)
- Wire strippers
- Screwdriver set (Phillips, flathead)
- Flush cutters
- Multimeter (for continuity testing)
- Heat-set insert tool (optional, if using inserts)

## Pre-Assembly: Print the Case

1. **Print both halves:**
   - Open `dmx_controller_case.scad` in OpenSCAD
   - Follow instructions in `README.md` to export `bottom_case.stl` and `top_case.stl`
   - Print on Prusa XL with black filament
   - Settings: 0.2mm layer height, 20%+ infill

2. **Post-process (optional):**
   - Fill debossed labels with white acrylic paint
   - Let dry, then wipe excess from surface
   - Result: white text on black background

## Step 1: Mount Arduino to Bottom Case

1. Place Arduino UNO in bottom case (left side, near front-left corner)
2. Align Arduino mounting holes with standoffs
3. Insert 4× M3 × 8mm screws through Arduino holes into standoffs
4. Tighten screws (don't overtighten, PCB can crack)

**Position:** Arduino should be ~15mm from left wall, ~15mm from front wall

## Step 2: Wire the SS-22L05 Slide Switch (DMX/PROG Mode Selector)

This switch disconnects D0/D1 from the DMX shield during programming.

### Wiring Diagram

```
                       SS-22L05 DPDT Slide Switch
                       (View from solder side)

    Common Pole 1 ----o   o---- NO Contact 1
    Common Pole 2 ----o   o---- NO Contact 2

DMX Mode (Position A):
  Arduino D0 → Common Pole 1 → NO Contact 1 → DMX Shield D0
  Arduino D1 → Common Pole 2 → NO Contact 2 → DMX Shield D1

PROG Mode (Position B):
  Arduino D0 → Common Pole 1 (disconnected)
  Arduino D1 → Common Pole 2 (disconnected)
  DMX Shield D0/D1 → floating (disconnected)
```

### Steps

1. **Cut 4 wires** (~8-10cm each):
   - 2× Orange (for D1 connections)
   - 2× Yellow (for D0 connections)

2. **Solder to switch:**
   - Common Pole 1: Yellow wire to Arduino D0
   - Common Pole 2: Orange wire to Arduino D1
   - NO Contact 1: Yellow wire to DMX Shield D0
   - NO Contact 2: Orange wire to DMX Shield D1

3. **Add heat shrink** to each solder joint

4. **Test with multimeter:**
   - Position A (DMX): Continuity between Arduino D0/D1 and Shield D0/D1
   - Position B (PROG): No continuity

## Step 3: Position DMX Shield

1. Place DMX shield in bottom case (right side, side-by-side with Arduino)
2. Shield will be secured by XLR connectors mounting to rear panel
3. **Do not stack** shield on Arduino (causes XLR connector clearance issues)

**Position:** Shield should be ~103mm from left wall (~20mm gap after Arduino), ~15mm from front wall

## Step 4: Wire Arduino to DMX Shield

Use Dupont jumper wires to connect (remember: D0/D1 go through the slide switch):

| Connection | Wire | Notes |
|------------|------|-------|
| Arduino 5V → Shield 5V | Red jumper | Direct connection |
| Arduino GND → Shield GND | Black jumper | Direct connection |
| Arduino D0 → Switch Pole 1 | Yellow (from Step 2) | Via slide switch |
| Switch NO1 → Shield D0 | Yellow (from Step 2) | Via slide switch |
| Arduino D1 → Switch Pole 2 | Orange (from Step 2) | Via slide switch |
| Switch NO2 → Shield D1 | Orange (from Step 2) | Via slide switch |

## Step 5: Mount Rear Panel Components

### XLR Connectors (2× Neutrik D-series)
1. Insert left XLR through rear panel hole (labeled "DMX OUT")
2. Insert right XLR through rear panel hole (labeled "DMX THRU")
3. Secure each with 4× mounting screws (included with Neutrik connectors)
4. **This secures the DMX shield in place**

### DC Barrel Jack
1. Insert DC jack through rear panel 8mm hole
2. Secure with included nut or mounting screws
3. Wire DC jack to Arduino Vin and GND:
   - Center pin (+) → Arduino Vin
   - Outer sleeve (-) → Arduino GND

## Step 6: Mount Front Panel Components

### USB-C Panel Mount
1. Feed USB-C extension cable through case from inside
2. Insert panel mount socket through front panel 22mm hole
3. Socket bezel should sit flush in recess
4. Connect internal USB-C plug to Arduino's USB-C port

### SS-22L05 Slide Switch
1. Insert switch through front panel rectangular cutout
2. Align mounting holes
3. Secure with 2× M2 × 8mm screws and nuts from inside
4. Verify slider moves freely between DMX and PROG positions
5. Label positions with marker if needed

## Step 7: Mount Top Panel Components

### 7-Segment Display
1. Insert display through 50mm × 20mm rectangular cutout from inside
2. Display should be visible from top, PCB on inside
3. Secure with 4× M2.5 × 8mm screws around display corners
4. Connect display I2C cable to Arduino:
   - VCC → Arduino 5V
   - GND → Arduino GND
   - SDA → Arduino A4
   - SCL → Arduino A5

### Master Toggle Switch (DSQ14-R)
1. Insert switch through 12mm hole (labeled "MASTER")
2. Secure with included nut
3. Wire switch to Arduino D2:
   - One terminal → Arduino D2
   - Other terminal → Arduino GND

### Momentary Buttons (2× DTQ14-S)
1. **PREV button:** Insert through left 12mm hole (labeled "PREV")
2. **NEXT button:** Insert through right 12mm hole (labeled "NEXT")
3. Secure each with included nuts
4. Wire buttons to Arduino:
   - PREV: One terminal → Arduino D3, other → GND
   - NEXT: One terminal → Arduino D4, other → GND

## Step 8: Final Assembly

1. **Test before closing:**
   - Set slide switch to PROG position
   - Connect USB cable to front panel USB-C socket
   - Upload sketch from [proberaum_dmx.ino](../proberaum_dmx/proberaum_dmx.ino)
   - Verify upload succeeds
   - Set slide switch to DMX position
   - Power via DC jack (9-12V)
   - Test all buttons, display, and master switch

2. **Close the case:**
   - Carefully align top and bottom halves
   - Ensure no wires are pinched
   - Insert 4× M3 × 20mm screws through corner posts
   - Tighten screws evenly (diagonal pattern)

3. **Final check:**
   - All labels visible and readable
   - Slide switch moves freely
   - Display shows scene number
   - Buttons respond correctly
   - DMX output working (test with DMX fixture)

## Wiring Color Code Reference

| Connection | Recommended Wire Color |
|------------|------------------------|
| +5V / VCC  | Red                    |
| GND        | Black                  |
| D0 (RX)    | Yellow                 |
| D1 (TX)    | Orange                 |
| D2 (Master)| Blue                   |
| D3 (Prev)  | Green                  |
| D4 (Next)  | Purple                 |
| A4 (SDA)   | White                  |
| A5 (SCL)   | Brown                  |

## Troubleshooting

### Arduino won't program
- **Check:** Slide switch in PROG position?
- **Check:** USB-C cable properly connected?
- **Check:** Arduino visible in Arduino IDE (Tools → Port)?

### Display not working
- **Check:** I2C address (default 0x70, check code)
- **Check:** SDA/SCL connections (A4/A5)
- **Check:** Display power (5V/GND)

### Buttons not responding
- **Check:** Button wiring (one side to Arduino pin, other to GND)
- **Check:** Code pullup resistors enabled (`pinMode(pin, INPUT_PULLUP)`)

### DMX not outputting
- **Check:** Slide switch in DMX position
- **Check:** D0/D1 wired correctly through switch
- **Check:** Shield jumpers set correctly (EN, DE, TX-uart)

### Case won't close
- **Check:** Wires not bunched up or pinched
- **Check:** Arduino seated flat on standoffs
- **Check:** No component interference

## Maintenance

- **Switch cleaning:** Spray contact cleaner into slide switch yearly
- **Display brightness:** Adjustable in code (`display.setBrightness(15)`)
- **Firmware updates:** Set switch to PROG, connect USB, upload new code

## Safety Notes

⚠️ **Important:**
- Always disconnect power before opening case
- Use 9-12V DC power only (center positive)
- Don't exceed 12V (Arduino damage risk)
- Ensure XLR connectors are properly wired (pin 1=GND, 2=Data-, 3=Data+)
- Keep case dry (not waterproof)

## Done!

Your DMX controller is now assembled and ready for use in the rehearsal room. Enjoy the smooth scene transitions! 🎵💡

---

**Next Steps:**
- Read [README.md](../README.md) for usage instructions
- Explore scene customization in [proberaum_dmx.ino](../proberaum_dmx/proberaum_dmx.ino)
- Check [CLAUDE.md](../CLAUDE.md) for technical details
