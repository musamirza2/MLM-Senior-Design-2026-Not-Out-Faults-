# Fault Detection Fall 2026

A prototype that uses machine learning to detect faults in a simulated three-phase transmission line. MATLAB/Simulink generates healthy and fault events, Jupyter notebooks turn those signals into features and train a model, and the ESP32-S3 firmware provides the embedded side of the system.

![alt text](faultdetection_diagram2.png)


## System Overview
![alt text](faultdetection_diagram3.png)

## From simulation to the ESP32-S3


Simulate the line. Generate three-phase voltage and current waveforms for healthy operation and fault events.

Build the dataset. Extract features from the waveforms and store them as processed CSV files.

Train and evaluate. Use the notebooks to develop and evaluate the fault detection model. Saved model artifacts are in models/.

Explore the embedded implementation. Use the hardware guide to find the ESP32-S3 firmware, schematics, and wiring notes.


## Repository Layout

- `notebooks/` - analysis and model-development notebooks.
- `data/processed/` - processed CSV datasets used by the notebooks.
- `models/` - saved model weights used by the project.
- `figures/` - exported plots and visual results.
- `hardware/` - microcontroller firmware, schematics, wiring notes, datasheets, and hardware test logs.

## Getting started
Open the notebooks with notebooks/ as the working directory. They load datasets from ../data/processed/ and saved models from ../models/, so keep those relative paths intact.

For the ESP32-S3 setup, the firmware is under hardware/firmware/esp-idf/.


## Data and scope

The datasets come from simulated transmission-line events. Model evaluation in this repository reflects those simulation conditions. The prototype is intended for advisory fault detection and does not issue relay trip commands.
