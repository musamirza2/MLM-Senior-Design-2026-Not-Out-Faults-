# ESP-IDF Firmware

Place the ESP-IDF microcontroller firmware project here.

Recommended tracked files once the firmware starts:

- `CMakeLists.txt`
- `main/`
- `components/`
- `sdkconfig.defaults`
- `partitions.csv`
- firmware-specific `README.md`

Generated files such as `build/`, `managed_components/`, `.bin`, `.elf`, and `.map` outputs are ignored by the root `.gitignore`.

Typical commands from this folder after an ESP-IDF project is created:

```bash
idf.py set-target esp32
idf.py build
idf.py flash monitor
```
