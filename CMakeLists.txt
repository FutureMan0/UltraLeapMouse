cmake_minimum_required(VERSION 3.29)
project(UltraLeapMouse_Test)

# C++-Standard festlegen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Pfad zum Leap Motion SDK festlegen
# Geben Sie hier den Pfad zu Ihrem LeapC SDK an
set(LEAP_SDK_PATH "C:/Program Files/Ultraleap/LeapSDK")

# Fügen Sie das LeapC Include-Verzeichnis hinzu
include_directories(${LEAP_SDK_PATH}/include)

# Quellcode-Dateien und LeapC-Beispieldateien hinzufügen
add_executable(UltraLeapMouse_Test main.cpp)

# LeapC-Bibliothek hinzufügen und verlinken
if(WIN32)
    set(LEAP_LIB ${LEAP_SDK_PATH}/lib/x64/LeapC.lib)
elseif(APPLE)
    set(LEAP_LIB ${LEAP_SDK_PATH}/lib/libLeapC.dylib)
else()
    set(LEAP_LIB ${LEAP_SDK_PATH}/lib/libLeapC.so)
endif()

# LeapC-Bibliothek zum Projekt linken
target_link_libraries(UltraLeapMouse_Test ${LEAP_LIB})

# Zusätzliche Einstellungen für Windows
if(WIN32)
    # Setzt den Pfad zur DLL für die Ausführung
    set_target_properties(UltraLeapMouse_Test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

# Post-Build-Befehl, um LeapC.dll bei Windows-Builds zu kopieren
if(WIN32)
    add_custom_command(TARGET UltraLeapMouse_Test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${LEAP_SDK_PATH}/lib/x64/LeapC.dll"
            "$<TARGET_FILE_DIR:UltraLeapMouse_Test>"
    )
endif()
