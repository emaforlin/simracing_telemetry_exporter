cmake_minimum_required(VERSION 3.24)

project(telemetry_exporter CXX)

# This project uses Windows shared memory APIs and only targets Windows
if(NOT WIN32)
    message(FATAL_ERROR "telemetry_exporter is a Windows-only project.")
endif()

add_executable(telemetry_exporter
    main.cpp
)

target_compile_features(telemetry_exporter PRIVATE cxx_std_20)

set_target_properties(telemetry_exporter PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_library(shmem_reader
    shmem_reader.cpp
)

add_library(acc_shmem
    acc_shmem.h
)

add_subdirectory(serial_telemetry_protocol)

add_subdirectory(serial)

add_subdirectory(telemetry_packer)

# telemetry_packer needs acc_shmem types and serial_telemetry_protocol (CRC, packet types)
target_include_directories(acc_shmem PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(serial_telemetry_protocol PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(serial PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(telemetry_packer PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(telemetry_packer PUBLIC ${CMAKE_SOURCE_DIR}/serial_telemetry_protocol)
target_include_directories(telemetry_exporter PRIVATE ${CMAKE_SOURCE_DIR}/serial)
target_include_directories(telemetry_exporter PRIVATE ${CMAKE_SOURCE_DIR}/serial_telemetry_protocol)
target_include_directories(telemetry_exporter PRIVATE ${CMAKE_SOURCE_DIR}/telemetry_packer)

set_target_properties(acc_shmem PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(serial_telemetry_protocol PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(serial PROPERTIES LINKER_LANGUAGE CXX)

# Link against Windows system libraries needed for shared memory APIs
# (OpenFileMappingW, MapViewOfFile, CloseHandle, UnmapViewOfFile)
target_link_libraries(telemetry_exporter PRIVATE kernel32)
target_link_libraries(telemetry_exporter PRIVATE shmem_reader)
target_link_libraries(telemetry_exporter PRIVATE acc_shmem)
target_link_libraries(telemetry_exporter PRIVATE serial_telemetry_protocol)
target_link_libraries(telemetry_exporter PRIVATE serial)
target_link_libraries(telemetry_exporter PRIVATE telemetry_packer)