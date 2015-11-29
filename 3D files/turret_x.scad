include <plates.scad>;
translate([0,0,turr_x]){
    rotate([90,90,0]){
        turret_x();
    }
}