cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Make the external SSD1306 component available to this project
list(APPEND EXTRA_COMPONENT_DIRS 
    ${CMAKE_SOURCE_DIR}/lib/esp-idf-ssd1306-master/components
    )

project(BEATKLOK)
