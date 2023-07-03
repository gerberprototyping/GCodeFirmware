# GCodeFirmware

A modular and adaptable firmware for CNC routers.

This project was designed by Andrew Gerber and Erik Neilson for the course
ECE 3710 Microcontroller Hardware and Software taught by Dr. Jonathan Phillips
at Utah State University fall semester 2021.

The full report can be found at
https://github.com/gerberprototyping/GCodeFirmware/wiki

A video presentation is available at
https://www.youtube.com/watch?v=GPX9U4xJiTs&t=1s

<br>

## G-Code Commands

| Command | Description | Usage | Arguments |
| --- | --- | --- | --- |
| **`G0`** & **`G1`** | Linear movement | `G0 [X<n>] [Y<n>] [Z<n>] [F<n>]` | XYZ values and feedrate
| **`G28`** | Home | `G28 [X] [Y] [Z]` | Select which axis to home *(default: all)* |
| **`G90`** | Absolute positioning mode *(default)* | `G90` | *no arguments* |
| **`G91`** | Relative positioning mode | `G91` | *no arguments* |
| **`G92`** | Set workspace origin | `G92 [X<n>] [Y<n>] [Z<n>]` | Machine coordinate for workspace origin *(default: current location)* |
| **`G92.1`** | Use native machine coordinates | `G92.1` | *no arguments* |
| **`M111`** | Set debug level | `M111 D<n>` | `D0` : echo off; `D1` : echo on *(default)* |

## ToDo

- [ ] Z homing 
- [ ] System stalls when using cnc-link
- [ ] Z probing
- [ ] `G92` wait to change origin until previous moves are complete
- [ ] Check `G28` and `G82` with no arguments
- cnc-link
  - [ ] terminal mode
  - [ ] abort command

| Command | Description |
| --- | --- |
| **`G4`** | Dwell |
| **`G29`** | Bed leveling (3-Point) |
| **`G30`** | Z-Probe at current XY location |
| **`G53`** | Move in machine coordinates |
| **`M0`** & **`M1`** | Stop and wait for user |
| **`M3`** & **`M4`** | Tool on |
| **`M5`** | Tool off |
| **`M6`** | Tool change |
| **`M10`** | Vacuum on |
| **`M11`** | Vacuum off |
| **`M17`** | Enable steppers |
| **`M18`** & **`M84`** | Disable steppers |
| **`M31`** | Report print time |
| **`M75`** | Start job timer |
| **`M76`** | Pause job timer |
| **`M77`** | Stop job timer |
| **`M78`** | Job stats |
| **`M80`** | Power on (ATX)
| **`M81`** | Power off (ATX)
| **`M112`** | Emergency Stop |
| **`M114`** | Get current position |
| **`M117`** | Display message |
| **`M119`** | End stop states |
| **`M211`** | Report software end stops |
| **`M300`** | Play tone |
| **`M355`** | Light On/Off |
| **`M400`** | Wait for current moves to finish |
| **`M410`** | Quick stop |
| **`M420`** | Get/set bed leveling |

<br><br>

This implementation is for the Nucleo-L476RG.
