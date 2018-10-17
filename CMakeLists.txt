cmake_minimum_required(VERSION 3.9)
project(b8_texat_convertWaveform)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

#Find ROOT
find_package(ROOT REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})

# Find FFTW3
find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDES})
link_directories(${FFTW_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(SCRIPTS
    )

foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_script}
        COPYONLY
    )
endforeach()

add_executable(b8_texat_convertWaveform
        Convert.C WaveletNew.cpp)

target_link_libraries(b8_texat_convertWaveform ${ROOT_LIBRARIES} ${FFTW_LIBRARIES})
