add_subdirectory(../core/ core)
add_subdirectory(../3d/ 3d)
add_subdirectory(../qml/ qml)

set_target_properties(qtuser_core PROPERTIES FOLDER qtuser)
set_target_properties(qtuser_3d PROPERTIES FOLDER qtuser)
set_target_properties(qtuser_qml PROPERTIES FOLDER qtuser)