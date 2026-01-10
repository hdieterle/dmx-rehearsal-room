#!/bin/bash
# Generate STL files from OpenSCAD design
# Requires OpenSCAD to be installed

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "🔧 DMX Controller Case - STL Generator"
echo "======================================"
echo ""

# Check if OpenSCAD is installed
if ! command -v openscad &> /dev/null; then
    echo "❌ Error: OpenSCAD not found"
    echo ""
    echo "Please install OpenSCAD:"
    echo "  - macOS: brew install --cask openscad"
    echo "  - Or download from: https://openscad.org/downloads.html"
    echo ""
    exit 1
fi

echo "✓ OpenSCAD found: $(which openscad)"
echo ""

# Create temporary SCAD files with render flags
echo "📝 Preparing render files..."

# Bottom case
cat > dmx_controller_case_bottom.scad << 'EOF'
include <dmx_controller_case.scad>

// Render bottom case only
bottom_case();
EOF

# Top case
cat > dmx_controller_case_top.scad << 'EOF'
include <dmx_controller_case.scad>

// Render top case only
top_case();
EOF

echo "✓ Render files created"
echo ""

# Generate bottom case STL
echo "🖨️  Rendering bottom_case.stl..."
openscad -o bottom_case.stl \
    --render \
    --colorscheme=Cornfield \
    dmx_controller_case_bottom.scad

if [ -f bottom_case.stl ]; then
    SIZE=$(ls -lh bottom_case.stl | awk '{print $5}')
    echo "✓ bottom_case.stl generated ($SIZE)"
else
    echo "❌ Failed to generate bottom_case.stl"
    exit 1
fi

echo ""

# Generate top case STL
echo "🖨️  Rendering top_case.stl..."
openscad -o top_case.stl \
    --render \
    --colorscheme=Cornfield \
    dmx_controller_case_top.scad

if [ -f top_case.stl ]; then
    SIZE=$(ls -lh top_case.stl | awk '{print $5}')
    echo "✓ top_case.stl generated ($SIZE)"
else
    echo "❌ Failed to generate top_case.stl"
    exit 1
fi

echo ""

# Clean up temporary files
echo "🧹 Cleaning up temporary files..."
rm -f dmx_controller_case_bottom.scad dmx_controller_case_top.scad
echo "✓ Cleanup complete"

echo ""
echo "✅ Success! STL files generated:"
echo "   - bottom_case.stl"
echo "   - top_case.stl"
echo ""
echo "Next steps:"
echo "1. Import STL files into PrusaSlicer"
echo "2. Use recommended print settings (see README.md)"
echo "3. Print on Prusa XL with black filament"
echo ""
