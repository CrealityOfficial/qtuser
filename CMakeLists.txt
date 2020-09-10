set(qtuser_folder qtuser)

add_subdirectory(core)
add_subdirectory(3d)
add_subdirectory(qml)

__set_folder_targets(${qtuser_folder} TARGET ${QTUSER_LIBS})


