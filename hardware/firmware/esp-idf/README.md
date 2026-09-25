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

This project is being edited from WSL/Linux. If ESP-IDF was installed only on Windows, WSL will not see `idf.py` or the Linux toolchain. Install ESP-IDF inside WSL or open/build the project from the Windows ESP-IDF terminal instead.

WSL/Linux setup example:

```bash
mkdir -p $HOME/esp
cd $HOME/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3
. ./export.sh
```

Typical commands from this folder:

```bash
idf.py set-target esp32s3
idf.py fullclean
idf.py build
idf.py flash monitor
```

Run these commands from `hardware/firmware/esp-idf/`. If CMake reports that `$ENV{IDF_PATH}/tools/cmake/project.cmake` cannot be found, the ESP-IDF environment is not active in that terminal or editor session.

If `main.c` cannot find `led.h`, confirm you opened or built from this `esp-idf/` folder. The LED files currently live directly in `main/`, and `main/CMakeLists.txt` builds both `main.c` and `led.c`.

For local testing, this repo also includes a helper script that tries common ESP-IDF install paths before running `idf.py`:

```bash
./tools/idf-build.sh build
./tools/idf-build.sh flash monitor
```
