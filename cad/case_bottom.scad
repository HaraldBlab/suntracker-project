// SunTracker

use <sensor.scad>

*translate([0,0,.1]) sensor();

// overaall size of the bottom case
function case_bottom_box() = sensor_box()+[4,4,5.5];

// one cylinder for the rounded box used for design
module case_bottom_rounded(pt, h)
{
    translate(pt) 
    cylinder(r=2, h=h, $fn=36);
}

// blank used for the design
module case_bottom_blank()
{
    offset = [-2,-2,-5.5];
    pt = case_bottom_box();
    //translate(offset) cube(pt);
    translate(offset) hull() {
        case_bottom_rounded([2,2,0], h=pt[2]);
        case_bottom_rounded([2,pt[1]-2,0], h=pt[2]);
        case_bottom_rounded([pt[0]-2,pt[1]-2,0], h=pt[2]);
        case_bottom_rounded([pt[0]-2,2,0], h=pt[2]);
    }
}

// cutout of the sensor circuit board
module case_bottom_cutout()
{
    border = [0.5, 0.5, 0.5];
    translate(-border/2) 
    cube(sensor_box()+ border);
}

// mount holes for the circuit board for 4mm screws
module case_bottom_holes($fn=36)
{
    color("white")
    for (pt = sensor_mounts())
        translate(pt-[0,0,1.5])
    cylinder(d=mount_screw_dm(),h=4,center=true);
}

// pocket for the wires on back if the cicuit board
module case_bottom_pocket()
{
    pt0 = pcb_hole_pt(1,1);
    dxy = sensor_box() - pt0; 
    pti = [dxy[0], dxy[1], 4];
    ptm = [dxy[0]/6, 1.5*2.54, 4];
    translate([0,0,-3]) 
    translate(pt0-[2.54, 1.27, 0]) 
    color("white") 
    difference()
    {
        cube(pti);
        translate([dxy[0]/6*5,0,0]) cube(ptm);
    }
}

// all in one
module case_bottom() 
{
    difference() {
        case_bottom_blank();
        case_bottom_cutout();
        case_bottom_holes();
        case_bottom_pocket();
    }
    
}

case_bottom();
