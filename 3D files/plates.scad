// 24 mm diameter
// 63.5 by 32 mm outer dimensions
//screws: 3 mm
//length required: 25+115 = 140mm
//arduino width: 53.3mm
//arduino length: 101.6mm
//arduino screws: M3-0.5 or UNC 4-40 screws and standoffs, holes are 3.2mm
//space for horiz servo from holder plate: 26mm (30mm)
//servo length: 22.66mm
//servo width: 12.54mm
//servo bracket w: 5.16mm
//servo screw w: 2mm
//servo bracket length: 32mm
//servo bracket width: 16mm
//distance between edge of bracket and first hole: 1.2 mm
//front to center of servo: 36mm
//endstop switch: 30mm * 5.7mm * 8.6mm
//40mm screw L: 12mm
//distance between endstop boltholes:18.45mm
//laser L: 21.04 mm
//laser d: 7.07 mm
//distance from front of servo to center of rotator: 5mm
//distance from plate to end of installed motor: 10mm
$fn = 50;
face_x = 76;
side_face_x = 140;
face_y = 44;
face_z = 3;
wire_d = 3.5;
bolt_d = 4;
epsilon = 0.3;
arduino_bolt_d = 2.8; //smaller size to thread
endstop_circle = 65;
turr_y = 45;
turr_x = (16+face_z)*2;

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
module top_plate(){
    union(){
        difference(){
            cube([face_x,face_z,side_face_x+face_z*2]);
            translate([face_x/2-6.5,-epsilon,30])cube([13,face_z+epsilon*2,23]);
            translate([face_x/2,face_z+epsilon,30-2.5])
                rotate([90,0,0])
                    cylinder(h=face_z+epsilon*2, d=1.8);
            translate([face_x/2,face_z+epsilon,30+23+2.5])
                rotate([90,0,0])
                    cylinder(h=face_z+epsilon*2, d=1.8);
            translate([face_x/2,face_z+epsilon,2*side_face_x/3+face_z])
               rotate([90,0,0])
                    cylinder(h=face_z+epsilon*2, d=26+epsilon);
           // translate([face_x/2,face_z+epsilon/2,11])
               // rotate([90,0,0])
                   // endstop_horiz_type1();
            
        }
        translate([face_x/4+4,face_z,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([3*face_x/4+4,face_z,-4]){
            rotate([90,-90,0]) bolt_hole(0);
        }
        translate([3*face_x/4-4,face_z,side_face_x+face_z*2+4]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([face_x/4-4,face_z,side_face_x+face_z*2+4]){
            rotate([90,90,0]) bolt_hole(0);
        }
        translate([face_x+4,face_z,side_face_x/4+face_z+4]){
            rotate([90,180,0]) bolt_hole(0);
        }
        translate([face_x+4,face_z,3*side_face_x/4+face_z+4]){
            rotate([90,180,0]) bolt_hole(0);
        }
        translate([-4,face_z,3*side_face_x/4+face_z-3.75]){
            rotate([90,0,0]) bolt_hole(0);
        }
        translate([-4,face_z,side_face_x/4+face_z-3.75]){
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
module endstop_horiz_type1(){
    linear_extrude(face_z+epsilon){
        union(){
            difference(){
                circle(d=endstop_circle);
                circle(d=endstop_circle-bolt_d*2);
                translate([-endstop_circle/2,-endstop_circle+bolt_d/2,0])
                    square([endstop_circle,endstop_circle+epsilon]);
                translate([-bolt_d,endstop_circle/2-bolt_d*2,0])
                    square([bolt_d*2,bolt_d*2]);
            }
            difference(){
                circle(d=endstop_circle-bolt_d*3);
                circle(d=endstop_circle-bolt_d*5);
                translate([-endstop_circle/2,-endstop_circle+bolt_d/2,0])
                    square([endstop_circle,endstop_circle+epsilon]);
                translate([-bolt_d,endstop_circle/2-bolt_d*2-4,0])
                    square([bolt_d*2,bolt_d*2]);
            }
            translate([endstop_circle/2-bolt_d/2-epsilon+.1,bolt_d/2+epsilon,0])
                circle(d=bolt_d);
            translate([-endstop_circle/2+bolt_d/2+epsilon-.1,bolt_d/2+epsilon,0])
                circle(d=bolt_d);
            translate([bolt_d+epsilon,endstop_circle/2-bolt_d/2-.4,0])
                circle(d=bolt_d);
            translate([-bolt_d-epsilon,endstop_circle/2-bolt_d/2-.4,0])
                circle(d=bolt_d);
            translate([endstop_circle/2-bolt_d*2-epsilon+.1,bolt_d/2+epsilon,0])
                circle(d=bolt_d);
            translate([-endstop_circle/2+bolt_d*2+epsilon-.1,bolt_d/2+epsilon,0])
                circle(d=bolt_d);
            translate([bolt_d+epsilon,endstop_circle/2-bolt_d*2-.4,0])
                circle(d=bolt_d);
            translate([-bolt_d-epsilon,endstop_circle/2-bolt_d*2-.4,0])
                circle(d=bolt_d);
        }
    }
}
module endstop_horiz_type2(){
    translate([27,0,0])
        cylinder(h=face_z+epsilon,d=bolt_d);
    translate([12,0,0])
        cylinder(h=face_z+epsilon,d=bolt_d);
    translate([-27,0,0])
        cylinder(h=face_z+epsilon,d=bolt_d);
    translate([-12,0,0])
        cylinder(h=face_z+epsilon,d=bolt_d);
}
module endstop_mount(){
    union(){
        difference(){
            cube([30,15,5.7]);
            translate([5.31,12,5.7/2])
                rotate([90,0,0])
                    cylinder(h=12+epsilon,d=arduino_bolt_d);
            translate([5.31+18.49,12,5.7/2])
                rotate([90,0,0])
                    cylinder(h=12+epsilon,d=arduino_bolt_d);
        } 
       translate([7,12+7,0])
            rotate([0,0,-90]) 
                bolt_hole(0);
        translate([22,12+7,0])
            rotate([0,0,-90]) 
                bolt_hole(0);
    } 
}

module turret_x(){
    difference(){
        cube([turr_x,turr_y,face_z]);
        translate([turr_x/2,turr_y/2,-epsilon/2])
            cylinder(h=face_z+epsilon,d=bolt_d);
        translate([turr_x/2,turr_y/2-16+1.2,-epsilon/2])
            cylinder(h=face_z+epsilon,d=1.8);
        translate([turr_x/2,turr_y/2+16-1.2,-epsilon/2])
            cylinder(h=face_z+epsilon,d=1.8);
        translate([turr_x/2+8-1.2,turr_y/2,-epsilon/2])
            cylinder(h=face_z+epsilon,d=1.8);
        translate([turr_x/2-8+1.2,turr_y/2,-epsilon/2])
            cylinder(h=face_z+epsilon,d=1.8);
    }
    difference(){
        translate([turr_x-face_z,12,face_z]){
            difference(){
                cube([face_z,turr_y-12,turr_y-7]);
                //translate([-epsilon/2,20,30])
                    // rotate([0,90,0])
                        //cylinder(h=face_z+epsilon,d=bolt_d);
        
            }
        }
        translate([-epsilon+2*turr_x-turr_x/3,-turr_y/2-2.5+12,66.5])
            rotate([0,90,90])
                servo_hole();
    }
}
module turret_z(){
        difference(){
            translate([0,0,20])
                rotate([-90,0,0])
                    cylinder(h=21,d=12);
            translate([0,-epsilon/2,20])
                rotate([-90,0,0])
                    cylinder(h=21+epsilon,d=8);
            translate([0,21/2-8,20])
                rotate([90,0,90])
                    cylinder(h=21+epsilon,d=1.8);
            translate([0,21/2+8,20])
                rotate([90,0,90])
                    cylinder(h=21+epsilon,d=1.8);
    }
}
module servo_hole(){
    translate([30,25,30]){
        difference(){
            cube([13,face_z+epsilon*2,23]);
            //translate([13/2,face_z+epsilon*2+epsilon/2,23-5])
                //rotate([90,0,0])
                    //cylinder(h=face_z+epsilon*3, d=bolt_d);
        }
        translate([13/2,face_z+epsilon*2,-2.5])
            rotate([90,0,0])
                cylinder(h=face_z+epsilon*2, d=1.8);
        translate([13/2,face_z+epsilon*2,23+2.5])
            rotate([90,0,0])
                cylinder(h=face_z+epsilon*2, d=1.8);
    }
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
    translate([0,face_y+epsilon,0]){
        top_plate();
    }
    translate([0,0,face_z+epsilon]){
        side_plate();
    }
    mirror([1,0,0]){
        translate([-face_x,0,face_z]){
            side_plate();
        }   
    }
    rotate([-90,0,0])
        translate([face_x/2-turr_x/2,-30-22.5-5,face_y+face_z*2+epsilon*2])
            turret_x();
    rotate([-90,0,0])
        translate([face_x/2,-turr_y/2-30+11/2,face_y+2*face_z+turr_y/2-epsilon*3-10])
            turret_z();
    //rotate([-90,0,0])
       // translate([face_x/2,-35,0])
            //cylinder(h=100,d=bolt_d);
   // rotate([-90,0,0])
    //    translate([0,-30,face_y+face_z+epsilon*3])
    //        endstop_mount();
   // rotate([-90,0,0])
   //     mirror([1,0,0])
    //        translate([-face_x,-30,face_y+face_z+epsilon*3])
    //            endstop_mount();
}










