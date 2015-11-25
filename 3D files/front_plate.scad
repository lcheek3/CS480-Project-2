include <plates.scad>;
translate([0,0,face_z]){
    rotate([180,0,0]){
        front_plate();
    }
}