# Fault Detection Fall 2026

Fault detection analysis and modeling workspace for transmission-line event data.

## Repository Layout

- `notebooks/` - analysis and model-development notebooks.
- `data/processed/` - processed CSV datasets used by the notebooks.
- `models/` - saved model weights used by the project.
- `figures/` - exported plots and visual results.

## Data Policy

Raw CEV/event exercise files are intentionally not tracked. Exercise CSV exports are also ignored except for the processed Exercise 4 and Exercise 5 CSVs used by the project.

Ignored local inputs include:

- `Event Files Exercises 1_7/`
- `*.cev` and `*.CEV`
- `Exercise*.csv` outside the allowed processed files in `data/processed/`
- local virtual environments and editor settings

## Notes

Run notebooks from the `notebooks/` directory or keep their relative paths unchanged. The notebooks reference processed data through `../data/processed/` and saved models through `../models/`.
