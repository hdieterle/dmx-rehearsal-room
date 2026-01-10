// DMX Rehearsal Room Controller Case
// Two-part clamshell design for Prusa XL
// All dimensions in millimeters

// ========== GLOBAL PARAMETERS ==========
$fn = 60; // Circle resolution (higher = smoother, slower render)

// Case dimensions
case_length = 180;  // X direction (front to back)
case_width = 140;   // Y direction (left to right)
case_height = 50;   // Z direction (total height)
wall_thickness = 3; // Wall thickness

// Screw posts for case assembly
screw_post_diameter = 6;
screw_hole_diameter = 3.2; // M3 screw clearance
screw_post_count = 4; // Corner posts

// Label text depth
text_depth = 0.7; // Debossed text for black filament

// ========== COMPONENT DIMENSIONS ==========

// Arduino UNO R3
arduino_length = 68.6;
arduino_width = 53.3;
arduino_hole_spacing_length = 50.8;
arduino_hole_spacing_width = 15.2;
arduino_hole_offset_x = 13.97;
arduino_hole_offset_y = 2.54;
arduino_mount_hole = 3.2; // M3 clearance
arduino_standoff_height = 8; // Height above case bottom

// DMX Shield (approximate based on XLR positions)
shield_length = 70;
shield_width = 60;
shield_standoff_height = 8;

// XLR Connectors (Neutrik D-series)
xlr_diameter = 24;
xlr_spacing = 24; // Center to center
xlr_screw_spacing = 19; // Horizontal screw spacing
xlr_screw_vertical = 24; // Vertical position from center
xlr_screw_hole = 3.5; // Mounting screw size

// USB-C Panel Mount
usbc_hole_diameter = 22;
usbc_bezel_diameter = 28;
usbc_bezel_depth = 1.5;

// SS-22L05 DPDT Slide Switch
slide_switch_width = 19;
slide_switch_height = 6.9;
slide_switch_slot_length = 5;
slide_switch_slot_width = 2.5;
slide_switch_screw_spacing = 19; // Horizontal M2 screw spacing
slide_switch_screw_hole = 2.3; // M2 clearance

// Toggle Switch (DSQ14-R)
toggle_hole_diameter = 12;

// Momentary Buttons (DTQ14-S)
button_hole_diameter = 12;

// 7-Segment Display (Adafruit 0.56" 4-digit)
display_width = 50;
display_height = 20;
display_screw_spacing_x = 45; // Approximate
display_screw_spacing_y = 15; // Approximate
display_screw_hole = 2.5; // M2.5 or small screw

// DC Barrel Jack
dc_jack_hole_diameter = 8;

// ========== COMPONENT POSITIONS ==========

// Arduino position (bottom left area)
arduino_pos_x = 15;
arduino_pos_y = 15;

// DMX Shield position (bottom right area, side-by-side with Arduino)
shield_pos_x = arduino_pos_x + arduino_length + 20; // 20mm gap for wiring
shield_pos_y = 15;

// Front panel components (front = Y=0 end)
usbc_pos_x = case_length * 0.3;
usbc_pos_y = 0;
usbc_pos_z = case_height / 4;  // Center in the wall height

slide_switch_pos_x = case_length * 0.7;
slide_switch_pos_y = 0;
slide_switch_pos_z = case_height / 4;  // Center in the wall height

// Rear panel components (rear = Y=case_width end)
xlr_pos_x = case_length * 0.35;
xlr_pos_y = case_width;
xlr_pos_z = case_height / 4;  // Center in the wall height
xlr_left_offset_x = 0;
xlr_right_offset_x = xlr_spacing;

dc_jack_pos_x = case_length * 0.15;
dc_jack_pos_y = case_width;
dc_jack_pos_z = case_height / 4;  // Center in the wall height

// Top panel components
display_pos_x = case_length * 0.5;
display_pos_y = case_width * 0.5;

toggle_pos_x = case_length * 0.2;
toggle_pos_y = case_width * 0.5;

button_prev_pos_x = case_length * 0.75;
button_prev_pos_y = case_width * 0.35;

button_next_pos_x = case_length * 0.75;
button_next_pos_y = case_width * 0.65;

// ========== MODULES ==========

// Bottom case half
module bottom_case() {
    difference() {
        // Main shell
        union() {
            // Bottom plate
            cube([case_length, case_width, wall_thickness]);

            // Walls
            translate([0, 0, 0])
                cube([wall_thickness, case_width, case_height/2]);
            translate([case_length - wall_thickness, 0, 0])
                cube([wall_thickness, case_width, case_height/2]);
            translate([0, 0, 0])
                cube([case_length, wall_thickness, case_height/2]);
            translate([0, case_width - wall_thickness, 0])
                cube([case_length, wall_thickness, case_height/2]);

            // Corner screw posts
            create_corner_posts();

            // Arduino mounting posts
            create_arduino_posts();

            // DMX Shield mounting posts (will screw into XLR connectors)
            // (Not needed as XLRs mount to rear panel)
        }

        // Screw holes in corner posts (threaded from top)
        corner_post_positions() {
            translate([0, 0, -1])
                cylinder(h=case_height/2 + 2, d=screw_hole_diameter);
        }

        // Front panel cutouts (Y=0 wall) - bottom half
        translate([usbc_pos_x, usbc_pos_y, usbc_pos_z])
            rotate([90, 0, 0])
                usbc_cutout();

        translate([slide_switch_pos_x, slide_switch_pos_y, slide_switch_pos_z])
            rotate([90, 0, 0])
                slide_switch_cutout();

        // Rear panel cutouts (Y=case_width wall) - bottom half
        translate([xlr_pos_x + xlr_left_offset_x, xlr_pos_y, xlr_pos_z])
            rotate([90, 0, 0])
                xlr_cutout();

        translate([xlr_pos_x + xlr_right_offset_x, xlr_pos_y, xlr_pos_z])
            rotate([90, 0, 0])
                xlr_cutout();

        translate([dc_jack_pos_x, dc_jack_pos_y, dc_jack_pos_z])
            rotate([90, 0, 0])
                cylinder(h=wall_thickness + 2, d=dc_jack_hole_diameter, center=true);
    }
}

// Top case half
module top_case() {
    difference() {
        // Main shell
        union() {
            // Top plate
            translate([0, 0, case_height/2 - wall_thickness])
                cube([case_length, case_width, wall_thickness]);

            // Walls
            translate([0, 0, 0])
                cube([wall_thickness, case_width, case_height/2]);
            translate([case_length - wall_thickness, 0, 0])
                cube([wall_thickness, case_width, case_height/2]);
            translate([0, 0, 0])
                cube([case_length, wall_thickness, case_height/2]);
            translate([0, case_width - wall_thickness, 0])
                cube([case_length, wall_thickness, case_height/2]);

            // Corner screw posts
            create_corner_posts();
        }

        // Top panel cutouts
        translate([display_pos_x, display_pos_y, case_height/2 - wall_thickness - 1])
            display_cutout();

        translate([toggle_pos_x, toggle_pos_y, case_height/2 - wall_thickness - 1])
            cylinder(h=wall_thickness + 2, d=toggle_hole_diameter);

        translate([button_prev_pos_x, button_prev_pos_y, case_height/2 - wall_thickness - 1])
            cylinder(h=wall_thickness + 2, d=button_hole_diameter);

        translate([button_next_pos_x, button_next_pos_y, case_height/2 - wall_thickness - 1])
            cylinder(h=wall_thickness + 2, d=button_hole_diameter);

        // Front panel cutouts (Y=0 wall)
        translate([usbc_pos_x, usbc_pos_y, usbc_pos_z])
            rotate([90, 0, 0])
                usbc_cutout();

        translate([slide_switch_pos_x, slide_switch_pos_y, slide_switch_pos_z])
            rotate([90, 0, 0])
                slide_switch_cutout();

        // Rear panel cutouts (Y=case_width wall)
        translate([xlr_pos_x + xlr_left_offset_x, xlr_pos_y, xlr_pos_z])
            rotate([90, 0, 0])
                xlr_cutout();

        translate([xlr_pos_x + xlr_right_offset_x, xlr_pos_y, xlr_pos_z])
            rotate([90, 0, 0])
                xlr_cutout();

        translate([dc_jack_pos_x, dc_jack_pos_y, dc_jack_pos_z])
            rotate([90, 0, 0])
                cylinder(h=wall_thickness + 2, d=dc_jack_hole_diameter, center=true);

        // Screw holes in corner posts (clearance from bottom)
        corner_post_positions() {
            translate([0, 0, -1])
                cylinder(h=case_height/2 + 2, d=screw_hole_diameter);
        }

        // Labels (debossed text)
        labels();
    }
}

// Corner screw post positions and creation
module corner_post_positions() {
    offset = screw_post_diameter / 2 + wall_thickness;

    for (pos = [
        [offset, offset, wall_thickness],
        [case_length - offset, offset, wall_thickness],
        [offset, case_width - offset, wall_thickness],
        [case_length - offset, case_width - offset, wall_thickness]
    ]) {
        translate(pos)
            children();
    }
}

// Create corner screw posts (solid cylinders)
module create_corner_posts() {
    corner_post_positions()
        cylinder(h=case_height/2 - wall_thickness, d=screw_post_diameter);
}

// Arduino mounting post positions
module arduino_post_positions() {
    hole_positions = [
        [0, 0],
        [arduino_hole_spacing_length, 0],
        [0, arduino_hole_spacing_width],
        [arduino_hole_spacing_length, arduino_hole_spacing_width]
    ];

    for (pos = hole_positions) {
        translate([
            arduino_pos_x + arduino_hole_offset_x + pos[0],
            arduino_pos_y + arduino_hole_offset_y + pos[1],
            wall_thickness
        ])
            children();
    }
}

// Create Arduino mounting posts with screw holes
module create_arduino_posts() {
    arduino_post_positions()
        difference() {
            cylinder(h=arduino_standoff_height, d=6);
            translate([0, 0, -1])
                cylinder(h=arduino_standoff_height + 2, d=arduino_mount_hole);
        }
}

// USB-C panel mount cutout
module usbc_cutout() {
    // Main hole
    cylinder(h=wall_thickness + 2, d=usbc_hole_diameter, center=true);

    // Bezel recess
    translate([0, 0, wall_thickness/2 - usbc_bezel_depth/2])
        cylinder(h=usbc_bezel_depth + 1, d=usbc_bezel_diameter);
}

// Slide switch cutout (SS-22L05)
module slide_switch_cutout() {
    // Slider slot
    translate([-slide_switch_slot_length/2, -slide_switch_slot_width/2, -wall_thickness/2 - 1])
        cube([slide_switch_slot_length, slide_switch_slot_width, wall_thickness + 2]);

    // Body clearance (rectangular cutout)
    translate([-slide_switch_height/2, -slide_switch_width/2, -wall_thickness/2 - 1])
        cube([slide_switch_height, slide_switch_width, wall_thickness + 2]);

    // M2 mounting screw holes
    for (offset = [-slide_switch_screw_spacing/2, slide_switch_screw_spacing/2]) {
        translate([0, offset, 0])
            cylinder(h=wall_thickness + 2, d=slide_switch_screw_hole, center=true);
    }
}

// XLR cutout (Neutrik D-series)
module xlr_cutout() {
    // Main circular hole
    cylinder(h=wall_thickness + 2, d=xlr_diameter, center=true);

    // D-shape notch (simplified as small rectangular cutout at bottom)
    translate([-xlr_diameter/2, -xlr_diameter/2 - 2, -wall_thickness/2 - 1])
        cube([xlr_diameter, 2, wall_thickness + 2]);

    // Mounting screw holes
    for (x_offset = [-xlr_screw_spacing/2, xlr_screw_spacing/2]) {
        translate([x_offset, -xlr_screw_vertical/2, 0])
            cylinder(h=wall_thickness + 2, d=xlr_screw_hole, center=true);
        translate([x_offset, xlr_screw_vertical/2, 0])
            cylinder(h=wall_thickness + 2, d=xlr_screw_hole, center=true);
    }
}

// Display cutout (rectangular)
module display_cutout() {
    translate([-display_width/2, -display_height/2, 0])
        cube([display_width, display_height, wall_thickness + 2]);

    // Mounting screw holes around display
    for (x = [-display_screw_spacing_x/2, display_screw_spacing_x/2]) {
        for (y = [-display_screw_spacing_y/2, display_screw_spacing_y/2]) {
            translate([x, y, 0])
                cylinder(h=wall_thickness + 2, d=display_screw_hole);
        }
    }
}

// Labels (debossed text)
module labels() {
    font = "Liberation Sans:style=Bold";

    // Top panel labels
    translate([display_pos_x, display_pos_y + display_height/2 + 8, case_height/2 - text_depth])
        linear_extrude(height=text_depth + 1)
            text("SCENE", size=5, halign="center", valign="center", font=font);

    translate([toggle_pos_x, toggle_pos_y - 15, case_height/2 - text_depth])
        linear_extrude(height=text_depth + 1)
            text("MASTER", size=4, halign="center", valign="center", font=font);

    translate([button_prev_pos_x - 18, button_prev_pos_y, case_height/2 - text_depth])
        linear_extrude(height=text_depth + 1)
            text("PREV", size=4, halign="center", valign="center", font=font);

    translate([button_next_pos_x + 18, button_next_pos_y, case_height/2 - text_depth])
        linear_extrude(height=text_depth + 1)
            text("NEXT", size=4, halign="center", valign="center", font=font);

    // Front panel labels (Y=0 wall)
    translate([usbc_pos_x, wall_thickness + text_depth, usbc_pos_z + 10])
        rotate([90, 0, 0])
            linear_extrude(height=text_depth + 1)
                text("USB", size=4, halign="center", valign="center", font=font);

    translate([slide_switch_pos_x, wall_thickness + text_depth, slide_switch_pos_z + 10])
        rotate([90, 0, 0])
            linear_extrude(height=text_depth + 1)
                text("DMX | PROG", size=3.5, halign="center", valign="center", font=font);

    // Rear panel labels (Y=case_width wall)
    translate([xlr_pos_x + xlr_left_offset_x, case_width - wall_thickness - text_depth - 1, xlr_pos_z + 15])
        rotate([90, 0, 0])
            linear_extrude(height=text_depth + 1)
                text("DMX OUT", size=3.5, halign="center", valign="center", font=font);

    translate([xlr_pos_x + xlr_right_offset_x, case_width - wall_thickness - text_depth - 1, xlr_pos_z + 15])
        rotate([90, 0, 0])
            linear_extrude(height=text_depth + 1)
                text("DMX THRU", size=3.5, halign="center", valign="center", font=font);

    translate([dc_jack_pos_x, case_width - wall_thickness - text_depth - 1, dc_jack_pos_z + 10])
        rotate([90, 0, 0])
            linear_extrude(height=text_depth + 1)
                text("12V DC", size=3.5, halign="center", valign="center", font=font);
}

// ========== RENDER ==========

// Uncomment the part you want to render/export:

// Bottom case (render this first)
bottom_case();

// Top case (render this second, then export separately)
// translate([0, case_width + 20, 0]) // Offset for visualization
//     top_case();

// Both cases together for visualization
// bottom_case();
// translate([0, 0, case_height/2])
//     top_case();
