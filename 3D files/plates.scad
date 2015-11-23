// 24 mm diameter
// 63.5 by 32 mm outer dimensions
//screws: 3 mm
$fn = 50;
face_x = 76;
face_y = 44;
face_z = 3;
wire_d = 3.5;
bolt_d = 4;
epsilon = 0.3;

//shape modules
module bolt_hole(in){ 
    difference(){
        union(){
            cube([4,8,face_z]);
            translate([in, 4, 0]){
                cylinder(h = face_z, d = 8);
            }
        }
        translate([in, 4, -epsilon]){
            cylinder(h = face_z+epsilon*2, d = bolt_d);
        }
    }
}
module side_plate(){
    union(){
        difference(){
            cube([face_z, face_y, face_x]);
            translate([-face_z,face_y/2,face_x*2/3]){
                rotate([0,90,0]){
                    cylinder(h = 10, d = 26 + epsilon);
                }
            }
        }
        translate([-4,0,0]){
            bolt_hole(0);
        }
        translate([-4,0,face_x-face_z]){
            bolt_hole(0);
        }
        translate([-4,face_y-8,0]){
            bolt_hole(0);
        }
        translate([-4,face_y-8,face_x-face_z]){
            bolt_hole(0);
        }
        translate([-4,face_y,face_x*3/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_z,face_x*3/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_y,face_x/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_z,face_x/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
    }
}
module front_plate(){
    union(){
        difference(){
            cube ([face_x,face_y,face_z]);
            translate([face_x/2,face_y/2,-epsilon]){
                cylinder(h = 10, d = 26 + epsilon);
            }
            translate([face_x/2 + 30,face_y/2 + 16,-epsilon]){
                cylinder(h = 10, d = bolt_d);
            }
            translate([face_x/2 - 30,face_y/2 + 16,-epsilon]){
                cylinder(h = 10, d = bolt_d);
            }
            translate([face_x/2 - 30,face_y/2 - 16,-epsilon]){
                cylinder(h = 10, d = bolt_d);
            }
            translate([face_x/2 + 30,face_y/2 - 16,-epsilon]){
                cylinder(h = 10, d = bolt_d);
            }
        }
        translate([face_x,0,0]){
            bolt_hole(4);
        }
        translate([face_x,face_y-8,0]){
            bolt_hole(4);
        }
        translate([-4,face_y-8,0]){
            bolt_hole(0);
        }
        translate([-4,0,0]){
            bolt_hole(0);
        }
    }
}
module back_plate(){
    union(){
        difference(){
            cube ([face_x,face_y,face_z]);
            translate([face_x/2,face_y/2,-epsilon]){
                cylinder(h = 10, d = 26 + epsilon);
            }
        }
        translate([face_x,0,0]){
            bolt_hole(4);
        }
        translate([face_x,face_y-8,0]){
            bolt_hole(4);
        }
        translate([-4,face_y-8,0]){
            bolt_hole(0);
        }
        translate([-4,0,0]){
            bolt_hole(0);
        }
    }
}
module holder_plate(){
    difference(){
        translate([4,2,0]){
            cube ([face_x-8,face_y-4,face_z]);
        }
        translate([face_x/2 + 30,face_y/2 + 16,-epsilon]){
            cylinder(h = 10, d = bolt_d);
        }
        translate([face_x/2 - 30,face_y/2 + 16,-epsilon]){
            cylinder(h = 10, d = bolt_d);
        }
        translate([face_x/2 - 30,face_y/2 - 16,-epsilon]){
            cylinder(h = 10, d = bolt_d);
        }
        translate([face_x/2 + 30,face_y/2 - 16,-epsilon]){
            cylinder(h = 10, d = bolt_d);
        }
        translate([face_x/2-2.5, face_y/2-5,-epsilon]){
            cube([5, face_y/2+5+epsilon, face_z*2]);
            translate([2.5,0,0]){
                cylinder(h=face_z*2, d=5);
            }
        }
        translate([face_x/2 - 10, face_y/2 - 10, -epsilon]){
            cylinder(h = 10, d = 8 + epsilon);
        }
        translate([face_x/2 + 10, face_y/2 - 10, -epsilon]){
            cylinder(h = 10, d = 8 + epsilon);
        }
    }
}

//placement

rotate([90,0,0]){
    front_plate();
    translate([0,0,face_x+face_z+epsilon*2]){
        back_plate();
    }
    translate([0,0,20]){
        holder_plate();
    }
    translate([0,0,face_z+epsilon]){
        side_plate();
    }
    mirror([1,0,0]){
        translate([-face_x,0,face_z+epsilon]){
            side_plate();
        }   
    }
}

