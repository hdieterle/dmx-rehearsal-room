# Bill of Materials (BOM)

Complete parts list for building the DMX Rehearsal Room Controller.

## Electronics

| Qty | Component | Description | Notes |
|-----|-----------|-------------|-------|
| 1 | Arduino UNO R3 | Microcontroller board | Official or compatible clone |
| 1 | CTC-DRA-10-R2 DMX Shield | RS485-based DMX interface | - |
| 1 | Adafruit 0.56" 4-Digit 7-Segment Display | Yellow, with I2C backpack | Part #: 878 or 879 |
| 1 | USB-C Panel Mount Extension Cable | 22mm hole, 28mm bezel | Male-to-female, ~30cm cable length |
| 1 | SS-22L05 DPDT Slide Switch | DMX/Programming mode selector | 6-pin, through-hole mounting |
| 1 | DSQ14-R Toggle Switch | Master On/Off, 12mm mounting hole | SPST or SPDT |
| 2 | DTQ14-S Momentary Push Buttons | Preset Prev/Next, 12mm mounting hole | Normally open (NO) |
| 2 | Neutrik XLR-3 D-Series Panel Mount | 24mm mounting hole | NC3FD-LX (female) or similar |
| 1 | DC Barrel Jack Panel Mount | 2.1mm center-positive, 8mm mounting hole | 9-12V input |
| 1 | 12V DC Power Supply | Center-positive, 2.1mm barrel plug | Minimum 1A, recommend 2A+ |
| 1 | USB-A to USB-B Cable | For Arduino programming | ~1-2m length recommended |

## Wiring & Connectors

| Qty | Component | Description | Notes |
|-----|-----------|-------------|-------|
| 1 | Dupont Jumper Wire Set | Male-to-female, 10-15cm length | For Arduino-to-shield connections |
| 1m | 22-24 AWG Stranded Wire | Assorted colors | For switch wiring |
| 1m | Heat Shrink Tubing | Assorted sizes | For wire insulation |

## Hardware (Case Assembly)

| Qty | Component | Description | Notes |
|-----|-----------|-------------|-------|
| 4 | M3 × 20mm Screws | Corner posts (case assembly) | Phillips or socket head |
| 4 | M3 Nuts | For corner posts | Or use heat-set inserts |
| 4 | M3 × 8mm Screws | Arduino mounting | Phillips |
| 2 | M2 × 8mm Screws | Slide switch mounting | Phillips |
| 2 | M2 Nuts | Slide switch mounting | - |
| 4 | M2.5 × 8mm Screws | Display mounting | Phillips |

## 3D Printing Materials

| Qty | Component | Description | Notes |
|-----|-----------|-------------|-------|
| ~200g | Black Filament | PLA, PETG, or ABS | For bottom case |
| ~100g | Black Filament | PLA, PETG, or ABS | For top case (lid) |

**Print Settings:**
- Layer height: 0.2mm (or 0.15mm for better label detail)
- Infill: 20% minimum (30% recommended)
- Supports: None required
- Estimated print time: 8-10 hours total

## Optional Components

| Qty | Component | Description | Notes |
|-----|-----------|-------------|-------|
| 1 | White Acrylic Paint | For filling debossed labels | Optional, improves label visibility |
| 4 | M3 Heat-Set Inserts | Alternative to M3 nuts | For cleaner assembly |
| 1 | Cable Management Kit | Zip ties, adhesive mounts | For internal wire routing |

## DMX Fixtures (Not Included)

The controller is designed for the following DMX fixtures (not included in this BOM):

| Qty | Fixture | Start Address | Channels |
|-----|---------|---------------|----------|
| 2 | Fun Generation PicoBeam 30 Quad LED | 1, 14 | 13ch each (26ch total) |
| 3 | Stairville LED Bar 120/4 RGB DMX | 27, 39, 51 | 12ch each (36ch total) |
| 1 | Botex MPX-4LED (Channel 1) | 63 | 1ch (dimmer) |
| 1 | Botex MPX-4LED (Channel 2) | 64 | 1ch (on/off) |

**Total DMX Channels Used:** 64

## Tools Required

### Soldering
- Soldering iron (for switch wiring)
- Solder (60/40 rosin core)
- Wire strippers
- Flush cutters
- Helping hands or PCB holder

### Assembly
- Phillips screwdrivers (various sizes)
- Small flathead screwdriver
- Multimeter (for continuity testing)
- Calipers (optional, for measuring components)

### 3D Printing
- 3D printer (Prusa XL or similar, min. 180×140×60mm build volume)
- Slicer software (PrusaSlicer recommended)

### Optional
- Heat-set insert tool (soldering iron with tip works)
- Deburring tool (for cleaning print edges)
- Small paintbrush (for filling labels)

## Estimated Total Cost

| Category | Estimated Cost (EUR) |
|----------|---------------------|
| Electronics | €80-120 |
| Wiring & Connectors | €10-15 |
| Hardware | €5-10 |
| 3D Printing Materials | €10-15 |
| **Total** | **€105-160** |

*Prices are approximate and may vary by supplier and region.*

## Recommended Suppliers

### Europe
- **Electronics:** Reichelt, Conrad, Mouser, DigiKey
- **DMX Components:** Thomann, Musicstore
- **Hardware:** Würth, local hardware store
- **3D Printing:** Prusa3D filament, local suppliers

### General
- **Arduino/Electronics:** Amazon, AliExpress (clones)
- **Adafruit Products:** Adafruit.com, authorized distributors
- **Neutrik Connectors:** Thomann, official Neutrik distributors

## Notes

1. **Arduino Clones:** Compatible Arduino UNO R3 clones work fine and are significantly cheaper than official boards.

2. **Switch Compatibility:** The DSQ14-R and DTQ14-S switch part numbers may vary by supplier. Look for 12mm mounting hole toggle/momentary switches.

3. **Power Supply:** Ensure the power supply is center-positive and rated for at least 1A. The Arduino draws ~50-100mA, plus any connected fixtures draw power from their own supplies.

4. **XLR Connectors:** The Neutrik D-series (NC3FD-LX) are panel-mount female XLR connectors. The "D" designation refers to the D-shaped mounting flange.

5. **Filament Color:** Black filament is recommended for debossed labels. The labels can optionally be filled with white acrylic paint for better contrast.

6. **Heat-Set Inserts:** If using heat-set inserts instead of M3 nuts, use M3×5.7mm (OD) inserts designed for 3D printing.

## Assembly Instructions

Refer to [case/ASSEMBLY.md](case/ASSEMBLY.md) for detailed step-by-step assembly instructions.

## Design Files

- OpenSCAD source: [case/dmx_controller_case.scad](case/dmx_controller_case.scad)
- Arduino sketch: [proberaum_dmx/proberaum_dmx.ino](proberaum_dmx/proberaum_dmx.ino)
- Printing guide: [case/README.md](case/README.md)
