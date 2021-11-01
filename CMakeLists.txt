add_subdirectory(core)
add_subdirectory(3d)
add_subdirectory(qml)

set(qtuser qtuser_core qtuser_3d qtuser_qml PARENT_SCOPE)
__set_folder_targets(qtuser TARGET ${qtuser})


