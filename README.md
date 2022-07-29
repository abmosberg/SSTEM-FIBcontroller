# SSTEM-FIBcontroller
This is the public repository with all source files needed to reproduce the SuperSTEM FIBcontroller as presented at M&M2022.


## Notes on SuperSTEM FIBcontroller 1.0 and 1.1
The difference between revisions 1.0 and 1.1 is that in 1.0 KEY11, the bottom left button, does not work as it is connected to a pin that also connects to an LED, and thus the internal pullup behavior does not work well.
Version 1.1 attempts to fix this by moving KEY11 to D1 instead of D17.

This repository contains 3 folders, with different parts of the project:

### Arduino
Arduino sketch required to program the controller. See the Arduino help pages for instructions on how to flash. The Arduino Micro can be connected directly to a PC and flashed over the micro-USB cable.

Note, this code is still for v1.0, and is very much a rough first attempt, that can easily be refined.

### Enclosure
3D files, both Autodesk Inventor part files (configurable) and exported .stl files that can be read by any common slicer for 3D printing. For the prototype, .stl's were sliced in Ultimaker Cura and printed on an Ender 3 without supports.

### PCB
The subfolders FIBcontroller_v1.0 and v1.1 contain archived KiCad 6.0 projects. These should contain everything needed to iterate on the design, but for simple fabrication the accompanying gerber zip folders can be uploaded to any of the popular PCB manufacturing houses (e.g. PCBway, JLCPCB). v1.0 fabricated successfully to become the prototype, but no guarantees are given for v1.1
(it should only be a small change, so fairly safe)

