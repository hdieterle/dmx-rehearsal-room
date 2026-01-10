# DMX Controller Case - Design Summary

## ✅ What's Been Created

All design files for your custom 3D-printable DMX controller case are ready!

### Files in `/case` directory:

1. **dmx_controller_case.scad** - OpenSCAD parametric design (fully editable)
2. **README.md** - Printing instructions and specifications
3. **ASSEMBLY.md** - Step-by-step assembly guide with wiring diagrams
4. **generate_stls.sh** - Helper script to generate STL files
5. **DESIGN_SUMMARY.md** - This file

## 📐 Case Specifications

| Parameter | Value |
|-----------|-------|
| **Dimensions** | 180mm × 140mm × 50mm |
| **Wall Thickness** | 3mm |
| **Case Style** | Two-part clamshell (top + bottom) |
| **Material** | Black filament (PLA/PETG/ABS) |
| **Print Volume** | ~162,000 mm³ (fits Prusa XL easily) |
| **Estimated Print Time** | ~8-10 hours total (both halves) |
| **Assembly Hardware** | 4× M3 screws + nuts |

## 🔧 Component Layout

### Bottom Half
- Arduino UNO R3 mounted on internal standoffs
- DMX shield positioned side-by-side (connected via jumper wires)
- 4× corner screw posts for case assembly

### Top Half Panels

**Front Panel:**
- USB-C panel mount (22mm hole, 28mm bezel recess)
- SS-22L05 DPDT slide switch (DMX/PROG mode selector)
- Debossed labels: "USB", "DMX | PROG"

**Rear Panel:**
- 2× Neutrik XLR-3 connectors (24mm dia., D-series mounting)
- DC barrel jack (8mm hole, 2.1mm center-positive)
- Debossed labels: "DMX OUT", "DMX THRU", "12V DC"

**Top Panel:**
- 7-segment display (50mm × 20mm cutout)
- Master toggle switch (12mm hole)
- PREV button (12mm hole)
- NEXT button (12mm hole)
- Debossed labels: "SCENE", "MASTER", "PREV", "NEXT"

## 🖨️ How to Generate STL Files

### Option 1: OpenSCAD GUI (Easiest)

1. **Open OpenSCAD:**
   - Launch OpenSCAD from Applications folder
   - File → Open → Select `dmx_controller_case.scad`

2. **Export Bottom Case:**
   - Edit line 414: Ensure `bottom_case();` is uncommented
   - Comment out lines 417-418 (top case render)
   - Press **F6** (Render - this may take 1-2 minutes)
   - File → Export → Export as STL
   - Save as `bottom_case.stl`

3. **Export Top Case:**
   - Comment out line 414: `// bottom_case();`
   - Uncomment lines 417-418:
     ```scad
     translate([0, case_width + 20, 0])
         top_case();
     ```
   - Press **F6** (Render - this may take 1-2 minutes)
   - File → Export → Export as STL
   - Save as `top_case.stl`

### Option 2: Shell Script (If CLI works)

```bash
cd /Users/hdieterle/GitHub/dmx-rehearsal-room/case
chmod +x generate_stls.sh
./generate_stls.sh
```

*Note: OpenSCAD CLI may not work properly on macOS without Rosetta 2 or if running headless. Use GUI method if script fails.*

## 🎨 Customization Options

The design is fully parametric - you can easily adjust:

### Common Tweaks:
```scad
// Line 9-12: Case dimensions
case_length = 180;  // Make longer/shorter
case_width = 140;   // Make wider/narrower
case_height = 50;   // Make taller/shorter
wall_thickness = 3; // Make stronger/lighter

// Line 20: Label text depth
text_depth = 0.7;   // Deeper (0.8-1.0) for better visibility

// Lines 77-115: Component positions
// Adjust X/Y positions if components don't align
```

After editing, regenerate STL files.

## 🔌 Key Design Features

### ✅ Solved Problems:
1. **Arduino + Shield Side-by-Side:** Prevents XLR connector interference
2. **DMX/PROG Mode Switch:** No need to remove shield for programming
3. **USB-C Front Panel:** Easy programming access
4. **Integrated Labels:** No Dymo labels needed
5. **Desktop Stable:** No mounting holes, sits flat on table

### 🎯 Design Decisions:
- **3mm walls:** Good balance of strength and print time
- **Debossed text:** Better than embossed for black filament (paint-fillable)
- **No supports needed:** All overhangs < 45°
- **Screw posts in corners:** Easy assembly/disassembly
- **Component-specific cutouts:** Precise fit for all parts

## 📋 Next Steps

1. **Generate STL files** using OpenSCAD GUI (see above)
2. **Slice in PrusaSlicer:**
   - Import bottom_case.stl and top_case.stl
   - Use 0.2mm layer height, 20% infill
   - No supports needed
   - Consider 0.15mm layers for better label detail
3. **Print on Prusa XL** with black filament
4. **Post-process (optional):**
   - Fill debossed labels with white acrylic paint
   - Wipe excess from surface
   - Result: professional-looking white-on-black labels
5. **Assemble** following [ASSEMBLY.md](ASSEMBLY.md)

## 🛠️ If You Need Changes

The OpenSCAD file is designed for easy modifications:

### Want different labels?
→ Edit lines 361-407 in `dmx_controller_case.scad`

### Component doesn't fit?
→ Update dimensions in lines 22-73
→ Measure actual component with calipers
→ Regenerate STL

### Want different case size?
→ Change lines 9-11 (case_length, case_width, case_height)
→ Regenerate STL

### Want thicker walls?
→ Change line 12 (wall_thickness)
→ Regenerate STL

## 📚 Documentation Files

- **README.md** - Printing specs, hardware list, troubleshooting
- **ASSEMBLY.md** - Detailed assembly steps, wiring diagrams, safety notes
- **CLAUDE.md** - (in parent dir) Technical project documentation

## 🎉 Design Complete!

All files are ready. You now have:
- ✅ Parametric OpenSCAD design
- ✅ Component specifications
- ✅ Printing instructions
- ✅ Assembly guide
- ✅ Wiring diagrams
- ✅ Helper scripts

**Total Design Time:** ~2 hours
**Estimated Print Time:** ~8-10 hours
**Estimated Assembly Time:** ~2-3 hours

---

## 💡 Tips

1. **First print?** Start with bottom case only to test fit
2. **Labels hard to read?** Increase text_depth to 0.8mm or paint them white
3. **Case won't close?** Check for print warping, sand corner posts if needed
4. **Components loose?** Add small pieces of foam tape for snug fit
5. **Want ventilation?** Add small holes in bottom case (edit line ~125)

## 📞 Support

If you need design modifications or have questions:
- Check [README.md](README.md) for printing FAQs
- Check [ASSEMBLY.md](ASSEMBLY.md) for assembly help
- Review OpenSCAD comments for parameter explanations
- Test fit components before final assembly

**Enjoy your custom DMX controller case!** 🎵💡
