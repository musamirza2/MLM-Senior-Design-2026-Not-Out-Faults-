cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{IDF_PATH})
    message(FATAL_ERROR "IDF_PATH is not set. Run the ESP-IDF export script before building, for example: . $HOME/esp/esp-idf/export.sh")
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(fault_detection_firmware)
