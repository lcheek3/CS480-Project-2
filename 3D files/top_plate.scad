include <plates.scad>;
translate([0,0,face_z]){
    rotate([-90,0,0]){
        top_plate();
    }
}