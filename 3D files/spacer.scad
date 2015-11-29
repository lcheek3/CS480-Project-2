//chip: 1.83mm
//screw:11.8mm
//10mm of free space
//4.5mm of hole
//6mm height
$fn = 50;
difference(){
    cylinder(h = 6, d = 5);
    translate([0,0,-0.05])cylinder(h = 6.1, d = 3);
}