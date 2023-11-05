// SunTracker

use <sensor.scad>
use <case_bottom.scad>

*translate([0,0,.01]) sensor();
*case_bottom();

// blank used for the design
module case_top_blank()
{
    offset = [-2,-2,1.5];
    pt = case_bottom_box();
    //translate(offset) cube(pt);
    translate(offset) hull() {
        case_bottom_rounded([2,2,0], h=1);
        case_bottom_rounded([2,pt[1]-2,0], h=1);
        case_bottom_rounded([pt[0]-2,pt[1]-2,0], h=1);
        case_bottom_rounded([pt[0]-2,2,0], h=1);
    }
}

// cylinder used with rounded_cuve
module rounding(pt, h, r=2)
{
    translate(pt) 
    cylinder(r=r, h=h, $fn=36);
}

// rounded cube
module rounded_cube(size, r=2)
{
    pt = size;
    //translate(offset) cube(pt);
    hull() {
        rounding([r,r,0], h=pt[2]);
        rounding([r,pt[1]-r,0], h=pt[2]);
        rounding([pt[0]-r,pt[1]-r,0], h=pt[2]);
        rounding([pt[0]-r,r,0], h=pt[2]);
    }
    //cube(size);
}

// the size of the window
function case_top_window_size() =
    pcb_hole_pt(11,10) - 
    pcb_hole_pt(2,2) + 
    [2.54, 2.54, 2.54];

// the position of the window
function case_top_window_pos() =
    pcb_hole_pt(1,2)-
    [2.54, 2.54/2, 2.54/2];

// window in the blank
module case_top_cutout(h=4)
{
    dxy = case_top_window_size();
    pti = [dxy[0], dxy[1], h];
    translate(case_top_window_pos()) 
    rounded_cube(pti);
}

// the ouside sensor border
module case_top_hull(h=8)
{
    bo = [2,2,0];
    dxy = case_top_window_size() + bo;
    pti = [dxy[0], dxy[1], h];
    pt = case_top_window_pos()-bo/2+[0,0,2.54];
    translate(pt)
    rounded_cube(pti);
}

// addional wall inside the senser border
module case_top_hull_blinds(h=8)
{
    dxy = case_top_window_size()+[2,2,0];
    pth = [dxy[0], 1, h+0.5];
    ptv = [1, dxy[1]/2, h+0.5];
    translate(case_top_window_pos()+[0, dxy[1]/2-0.5, 2.54]-[1,1,0.5])
    cube(pth);
    translate(case_top_window_pos()+[dxy[0]/2-0.5, 0, 2.54]-[1,1,0.5])
    cube(ptv);
}

// height of the blinds needed
function shadow_height() = 3*2.54/2/tan(10);

// sensor border and the blinds
module case_top_shadow() 
{
    echo(shadow_height());
    h = shadow_height();
    hh = h;
    union() {
        difference() {
            case_top_hull(h=hh);
            case_top_cutout(h=hh+4);
        }
        case_top_hull_blinds(h=h);
    }
}

// connector cutout in the blank
module case_top_connector($fn=36)
{
    box = sensor_connector_box()+[2,-2.54,0];
    pt = sensor_connector_pos();
    hull() {
        translate(pt+[0,-box[1]/2,0])
            cylinder(r=box[0]/2,h=2);
        translate(pt+[0, box[1]/2,0])
            cylinder(r=box[0]/2,h=2);
    }
}

// single mount holes
module case_top_hole(pt,dm,$fn=36)
{
    translate(pt-[0,0,-1.5])
        cylinder(d=dm,h=4,center=true);
}

// mount holes for the circuit board
module case_top_holes($fn=36)
{
    pts = sensor_mounts();
    dms = sensor_mount_dms();
    for (i = [0:2])
        case_top_hole(pts[i], dms[i]);
}

// the top case of the sensor
module case_top()
{
    union() {
        difference() {
            case_top_blank();
            case_top_cutout();
            case_top_connector();
            case_top_holes();
        }
        case_top_shadow();
    }
     
}

color("khaki") case_top();
