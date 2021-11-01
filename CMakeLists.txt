add_subdirectory(core)
add_subdirectory(3d)
add_subdirectory(qml)

set_target_properties(qtuser_core PROPERTIES FOLDER qtuser)
set_target_properties(qtuser_3d PROPERTIES FOLDER qtuser)
set_target_properties(qtuser_qml PROPERTIES FOLDER qtuser)

set(qtuser qtuser_core qtuser_3d qtuser_qml)


