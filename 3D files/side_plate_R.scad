include <plates.scad>;
mirror([1,0,0]){
    translate([0,0,face_z]){
        rotate([0,90,0]){
            side_plate();
        }
    }
}
