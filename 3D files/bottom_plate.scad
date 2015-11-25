include <plates.scad>;
translate([0,0,face_z*2]){
    rotate([-90,0,0]){
        bottom_plate();
    }
}