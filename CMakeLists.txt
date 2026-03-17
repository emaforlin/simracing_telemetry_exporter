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

set_target_properties(acc_shmem PROPERTIES LINKER_LANGUAGE CXX)

# Link against Windows system libraries needed for shared memory APIs
# (OpenFileMappingW, MapViewOfFile, CloseHandle, UnmapViewOfFile)
target_link_libraries(telemetry_exporter PRIVATE kernel32)
target_link_libraries(telemetry_exporter PRIVATE shmem_reader)
target_link_libraries(telemetry_exporter PRIVATE acc_shmem)