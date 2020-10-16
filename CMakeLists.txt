cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigureAllModules) 

project(qtuser)

__enable_qt5()
add_subdirectory(core)
add_subdirectory(3d)
add_subdirectory(qml)

set(qtuser qtuser_core qtuser_3d qtuser_qml)

__configure_all()


