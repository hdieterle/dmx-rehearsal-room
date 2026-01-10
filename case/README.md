# DMX Controller Case - 3D Printing Files

Custom case design for the DMX Rehearsal Room Controller, optimized for the Prusa XL printer.

## Design Overview

**Case Style:** Two-part clamshell (top + bottom halves)
**Dimensions:** 180mm × 140mm × 50mm
**Wall Thickness:** 3mm
**Material:** Black filament recommended
**Print Settings:** 0.2mm layer height, 20% infill minimum

## Component Layout

### Bottom Half
- Arduino UNO R3 mounted internally with standoffs (no external access)
- DMX shield positioned side-by-side with Arduino (connected via jumper wires)
- 4× corner screw posts for case assembly

### Top Half - Front Panel
- USB-C panel mount connector (22mm hole, 28mm bezel)
- SS-22L05 DPDT slide switch (DMX/Programming mode selector)
- Labels: "USB", "DMX | PROG"

### Top Half - Rear Panel
- 2× Neutrik XLR-3 D-series connectors (24mm diameter, 24mm spacing)
  - Left: DMX OUT
  - Right: DMX THRU
- DC barrel jack (8mm hole) - 12V power input
- Labels: "DMX OUT", "DMX THRU", "12V DC"

### Top Half - Top Panel
- 7-segment display (50mm × 20mm rectangular cutout)
- Master toggle switch (DSQ14-R, 12mm hole)
- 2× Momentary buttons (DTQ14-S, 12mm holes)
  - PREV button
  - NEXT button
- Labels: "SCENE", "MASTER", "PREV", "NEXT"

## Files

- `dmx_controller_case.scad` - OpenSCAD source file (parametric, editable)
- `bottom_case.stl` - Bottom half STL (ready to print)
- `top_case.stl` - Top half STL (ready to print)

## How to Generate STL Files

### Method 1: Using OpenSCAD GUI (Recommended)

1. Download and install [OpenSCAD](https://openscad.org/downloads.html)
2. Open `dmx_controller_case.scad`
3. **To export bottom case:**
   - Ensure line 414 is uncommented: `bottom_case();`
   - Comment out lines 417-418 (top case)
   - Press F5 (Preview) or F6 (Render)
   - File → Export → Export as STL → Save as `bottom_case.stl`
4. **To export top case:**
   - Comment out line 414 (bottom case)
   - Uncomment lines 417-418:
     ```scad
     translate([0, case_width + 20, 0])
         top_case();
     ```
   - Press F5 (Preview) or F6 (Render)
   - File → Export → Export as STL → Save as `top_case.stl`

### Method 2: Using OpenSCAD Command Line

```bash
# Generate bottom case
openscad -o bottom_case.stl \
  -D 'RENDER_PART="bottom"' \
  dmx_controller_case.scad

# Generate top case
openscad -o top_case.stl \
  -D 'RENDER_PART="top"' \
  dmx_controller_case.scad
```

## Printing Instructions

### Prusa XL Settings
- **Filament:** Black PLA, PETG, or ABS
- **Layer Height:** 0.2mm (0.15mm for better text definition)
- **Infill:** 20% minimum (30% for more rigidity)
- **Supports:** None required (all overhangs < 45°)
- **Brim:** Optional (recommended for ABS to prevent warping)

### Print Orientation
- **Bottom case:** Print as-is (bottom facing build plate)
- **Top case:** Print upside-down (top panel facing build plate) for best surface finish on labels

### Post-Processing
- Debossed text labels can be filled with acrylic paint or paint markers for better contrast
- Use white or silver paint in debossed areas on black filament
- Wipe excess paint off surface immediately, leaving paint only in recesses

## Assembly Hardware Required

- 4× M3 × 20mm screws (corner posts, joining top and bottom halves)
- 4× M3 nuts or heat-set inserts (optional, for threaded corner posts)
- 4× M3 × 8mm screws (Arduino mounting to standoffs)
- 2× M2 × 8mm screws (SS-22L05 slide switch mounting)
- 4× M2.5 × 8mm screws (7-segment display mounting)
- Dupont jumper wires (Arduino to DMX shield connections: D0, D1, 5V, GND)

## Wiring Connections

### Arduino to DMX Shield (Side-by-Side Configuration)
| Arduino Pin | DMX Shield Pin | Wire Color Suggestion |
|-------------|----------------|-----------------------|
| D0 (RX)     | D0 (RX)        | Yellow                |
| D1 (TX)     | D1 (TX)        | Orange                |
| 5V          | 5V             | Red                   |
| GND         | GND            | Black                 |

### USB-C Extension Cable
- Connect internal USB-C plug to Arduino's USB-C port
- Route cable to front panel USB-C socket

### SS-22L05 Slide Switch Wiring
Wire the switch to disconnect D0/D1 during programming:
- **Position A (DMX mode):** Arduino D0/D1 connected to DMX shield
- **Position B (PROG mode):** Arduino D0/D1 disconnected from DMX shield

```
Arduino D0 ----[Switch Pole 1]---- DMX Shield D0
Arduino D1 ----[Switch Pole 2]---- DMX Shield D1
```

## Customization

The OpenSCAD file is fully parametric. You can easily adjust:
- Case dimensions (lines 9-11)
- Wall thickness (line 12)
- Component positions (lines 75-115)
- Label text (lines 361-407)
- Text depth (line 20)

After making changes, regenerate the STL files using the methods above.

## Troubleshooting

### Labels not visible
- Increase `text_depth` to 0.8mm or 1.0mm (line 20)
- Print with 0.15mm layer height for better detail
- Fill debossed text with white/silver paint

### Components don't fit
- Measure your actual components with calipers
- Update component dimensions in lines 22-73
- Regenerate STL files

### Case doesn't close properly
- Check for print warping (use brim for large flat parts)
- Verify wall thickness is exactly 3mm in slicer preview
- Sand down corner posts if too tight

## License

This case design is part of the DMX Rehearsal Room Controller project and is licensed under GPL-3.0.

## Resources

- [OpenSCAD Documentation](https://openscad.org/documentation.html)
- [Prusa XL Specifications](https://www.prusa3d.com/product/original-prusa-xl-2/)
- [Arduino UNO R3 Dimensions](https://docs.arduino.cc/hardware/uno-rev3)
