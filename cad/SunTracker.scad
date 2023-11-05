// SunTracker

use <sensor.scad>
use <case_bottom.scad>
use <case_top.scad>

translate([0,0,.01]) sensor();
case_bottom();
case_top();

