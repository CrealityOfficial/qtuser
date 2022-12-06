add_subdirectory(core)

set(build_libs qtuser_core)

if(TARGET Qt5::3DRender)
	message(STATUS "qtuser setup qtuser_3d qtuser_qml")
	add_subdirectory(3d)

	if(TARGET Qt5::Quick)
		add_subdirectory(quick)
		list(APPEND build_libs qtuser_quick)
	endif()	
	
	add_subdirectory(qml)
	list(APPEND build_libs qtuser_3d qtuser_qml)
endif()

message(STATUS "qtuser --> [${build_libs}]")
__set_folder_targets(qtuser TARGET ${build_libs})

set(qtuser ${build_libs} PARENT_SCOPE)


