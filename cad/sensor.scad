// SunTracker Sensor
// Platine [mm]
plate = [ 39.4, 30.0, 1.5];

function pcb_col_border() =
    plate[1] - 9 * 2.54 - 1.0/2.0;
    
function pcb_hole_pt(col, row) =
    [3+2.54+1, pcb_col_border()/2, 1.5/2] + 
    [(col-1)*2.54, (row-1)*2.54, 0];

module pcb_hole(col, row)
{
    translate(pcb_hole_pt(col, row))
        cylinder(d=1, h= 2*1.5, center=true, $fn=24);
}
module pcb_holes()
{
    for (col = [1:14])
        for (row = [1:10])
            pcb_hole(col, row);
}

function mount_screw_dm() = 2;
module mount_hole(pt, dm)
{
    translate(pt) 
        cylinder(d=dm, h=2*1.5, center=true, $fn=36);
}

function mount_hole_centers() = [
    // 2.5 mm 
    [1+2.5/2, 1+2.5/2, 1.5/2],
    // 2.5 mm 
    [1+2.5/2, plate[1]-(1+2.5/2), 1.5/2],
    // 2.0 mm 
    pcb_hole_pt(12,1)];

function mount_hole_dms() = [2.5, 2.5, 2];

module mount_holes()
{
    pts = mount_hole_centers();
    dms = mount_hole_dms();
    for (i = [0:2])
        mount_hole(pts[i], dms[i]);
}
module pcb() 
{
    difference() {
    color ("green") cube(plate);
    pcb_holes();
    mount_holes();
    }
    
}

*pcb();

module ldr(pt)
{
    color("red") 
    translate(pt) translate([0,0,1.5]) {
        hull() {
        translate([-2.54/2, 0, 0])
        cylinder(d=2.54, h=1, center=true,$fn=36);
        translate([2.54/2, 0, 0])
        cylinder(d=2.54, h=1, center=true, $fn=36);
        }
    }
}

module ldr_top() { ldr(pcb_hole_pt(5,9)); }
module ldr_left() { ldr(pcb_hole_pt(2,4)); }
module ldr_right() { ldr(pcb_hole_pt(8,4)); }

module ldrs()
{
    ldr_top();
    ldr_left();
    ldr_right();
}

*ldrs();

function sensor_connector_box() =
    [2.54, 5*2.54, 8];

function sensor_connector_pos() =
    pcb_hole_pt(12,7);

// the 5 pin connector
module connector(pt)
{
    box = sensor_connector_box();
    color("black")
    translate(pt) translate([0,0,box[2]/2])
    cube(box, true);
}

*connector(sensor_connector_pos);

module sensor()
{
    pcb();
    ldrs();
    connector(pcb_hole_pt(12,7));
}

sensor();

function sensor_box() = plate;
function sensor_mounts() = mount_hole_centers();
function sensor_mount_dms() = mount_hole_dms();

