# Hardware

Hardware workspace for the fault-detection microcontroller, sensor interface, wiring, and validation notes.

## Layout

- `firmware/esp-idf/` - ESP-IDF firmware project area for the microcontroller.
- `schematics/` - circuit schematics and exported PDFs.
- `pcb/` - PCB design files and manufacturing exports.
- `datasheets/` - component datasheets used for design decisions.
- `wiring/` - pin maps, wiring tables, and breadboard diagrams.
- `test_logs/` - bring-up notes, measurements, and hardware validation records.

## Initial Hardware Goals

- Define the microcontroller board and toolchain.
- Document sensor inputs and expected signal ranges.
- Map microcontroller pins to hardware signals.
- Keep firmware source tracked while excluding generated build outputs.

## Firmware Notes

The repo is prepared for an ESP-IDF firmware project under `firmware/esp-idf/`. Commit firmware source, configuration defaults, and project documentation. Do not commit generated build artifacts.
