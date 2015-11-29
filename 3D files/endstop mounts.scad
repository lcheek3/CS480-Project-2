include <plates.scad>;
translate([0,0,face_z]){
    rotate([90,0,0]){
        endstop_mount();
    }
    rotate([90,0,0]){
        translate([-3,0,0])
            mirror([1,0,0])
                endstop_mount();
    }
}