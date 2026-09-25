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

Activate the ESP-IDF environment before configuring or building. Use the path where ESP-IDF is installed on your machine:

```bash
. $HOME/esp/esp-idf/export.sh
```

Typical commands from this folder:

```bash
idf.py set-target esp32s3
idf.py fullclean
idf.py build
idf.py flash monitor
```

Run these commands from `hardware/firmware/esp-idf/`. If CMake reports that `$ENV{IDF_PATH}/tools/cmake/project.cmake` cannot be found, the ESP-IDF environment is not active in that terminal or editor session.

If `main.c` cannot find `led.h`, confirm you opened or built from this `esp-idf/` folder and that `main/CMakeLists.txt` includes the `fault_indicators` component. The LED header lives at `components/fault_indicators/include/led.h`.
