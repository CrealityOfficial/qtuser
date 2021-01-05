cmake_minimum_required(VERSION 3.1)

project(qtuser)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigureAllModules) 

__enable_qt5()
add_subdirectory(core)
add_subdirectory(3d)
add_subdirectory(qml)

set(qtuser qtuser_core qtuser_3d qtuser_qml)


