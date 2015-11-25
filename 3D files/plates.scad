// 24 mm diameter
// 63.5 by 32 mm outer dimensions
//screws: 3 mm
//length required: 25+115 = 140mm
//arduino width: 53.3mm
//arduino length: 101.6mm
//arduino screws: M3-0.5 or UNC 4-40 screws and standoffs, holes are 3.2mm
$fn = 50;
face_x = 76;
side_face_x = 140;
face_y = 44;
face_z = 3;
wire_d = 3.5;
bolt_d = 4;
epsilon = 0.3;
arduino_bolt_d = 2.8; //smaller size to thread


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
module bottom_bolt_hole(in){
    difference(){
        union(){
            cube([4,8,face_z/2]);
            translate([in, 4, 0]){
                cylinder(h = face_z/2, d = 8);
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
            cube([face_z, face_y, side_face_x]);
            translate([-face_z,face_y/2,side_face_x*2/3]){
                rotate([0,90,0]){
                    cylinder(h = 10, d = 26 + epsilon);
                }
            }
        }
        translate([-4,0,0]){
            bolt_hole(0);
        }
        translate([-4,0,side_face_x-face_z]){
            bolt_hole(0);
        }
        translate([-4,face_y-8,0]){
            bolt_hole(0);
        }
        translate([-4,face_y-8,side_face_x-face_z]){
            bolt_hole(0);
        }
        translate([-4,face_y,side_face_x*3/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_z,side_face_x*3/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_y,side_face_x/4-4]){
            rotate([90,0,0]){
                bolt_hole(0); 
            }
        }
        translate([-4,face_z,side_face_x/4-4]){
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
        translate([face_x/4+4,face_z,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([face_x/4+4,face_y,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([3*face_x/4+4,face_z,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([3*face_x/4+4,face_y,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
    }
}
module back_plate(){
    union(){
        difference(){
            cube ([face_x,face_y,face_z]);
            translate([face_x/2,face_y/2-5,-epsilon]){
                    cube([50,face_y-20,10], center = true);
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
        translate([face_x/10-4,face_z,4+face_z]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([face_x/4-4,face_y,4+face_z]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([9*face_x/10-4,face_z,4+face_z]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([3*face_x/4-4,face_y,4+face_z]){
            rotate([90,90,0]) bolt_hole(0);
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
module bottom_plate(){
    union(){
        difference(){
            cube([face_x,face_z*2,side_face_x+face_z*2]);
            translate([face_x/2+53.3/2,face_z-1.5,side_face_x]){
                    rotate([90,90,180]){
                        arduino_plate();
                    }
            }
        }
        translate([face_x/4+4,face_z*2,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([3*face_x/4+4,face_z*2,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([9*face_x/10-4,face_z*2,side_face_x+face_z*2+4]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([face_x/10-4,face_z*2,side_face_x+face_z*2+4]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([face_x+4,face_z*2,side_face_x/4+face_z+4]){
            rotate([90,180,0]) bolt_hole(0);
        }
        translate([face_x+4,face_z*2,3*side_face_x/4+face_z+4]){
            rotate([90,180,0]) bolt_hole(0);
        }
        translate([-4,face_z*2,3*side_face_x/4+face_z-3.75]){
            rotate([90,0,0]) bolt_hole(0);
        }
        translate([-4,face_z*2,side_face_x/4+face_z-3.75]){
            rotate([90,0,0]) bolt_hole(0);
        }
    }
}
module arduino_plate(){
   // cube([114.3,57.15,10]);
    translate([13.97,2.54,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
    translate([15.24,50.8,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
    translate([90.17,50.8,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
    translate([96.52,2.54,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
    translate([66.04,35.56,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
    translate([66.04,7.62,0]) cylinder(h=face_z*1.5+epsilon,d=arduino_bolt_d);
}

//placement

rotate([90,0,0]){
    front_plate();
    translate([0,-face_z*2-epsilon,0]){
        bottom_plate();
    }
    translate([0,0,side_face_x+face_z]){
        back_plate();
    }
    translate([0,0,20]){
        holder_plate();
    }
    translate([0,0,face_z+epsilon]){
        side_plate();
    }
    mirror([1,0,0]){
        translate([-face_x,0,face_z]){
            side_plate();
        }   
    }
}




