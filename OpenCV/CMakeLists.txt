cmake_minimum_required(VERSION 3.0.0)
project(opencv_example_project VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)


add_executable(opencv_example example.cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
find_path(MOSQUITTOPP_INCLUDE_DIR mosquittopp.h)
find_library(MOSQUITTOPP_LIBRARY mosquittopp)
find_library(MOSQUITTO_LIBRARY mosquitto)
if(MOSQUITTOPP_INCLUDE_DIR AND MOSQUITTOPP_LIBRARY AND MOSQUITTO_LIBRARY)
  include_directories(${MOSQUITTOPP_INCLUDE_DIR})
  add_definitions(-DWITH_WBHOMABRIDGE)
  SET(HAVE_MOSQUITOPP TRUE)
else()
  message(STATUS "==== working mosquitopp not found, support for WBHomaBridge disabled")
endif()
include(CPack)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX__STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_example ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${MOSQUITTOPP_LIBRARY} ${MOSQUITTO_LIBRARY})










# cmake needs this line
#cmake_minimum_required(VERSION 2.8)

# Define project name
project(opencv_example_project)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
#find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
#message(STATUS "OpenCV library status:")
#message(STATUS "    version: ${OpenCV_VERSION}")
#message(STATUS "    libraries: ${OpenCV_LIBS}")
#message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

#if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
 # include_directories(${OpenCV_INCLUDE_DIRS})
#endif()

# Declare the executable target built from your sources
#add_executable(opencv_example example.cpp)

# Link your application with OpenCV libraries
#target_link_libraries(opencv_example PRIVATE ${OpenCV_LIBS})
