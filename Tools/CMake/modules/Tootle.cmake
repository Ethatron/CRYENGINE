if(WIN32 OR WIN64)
	add_library(TOOTLE SHARED IMPORTED GLOBAL)
	set_target_properties(TOOTLE PROPERTIES
		INTERFACE_COMPILE_DEFINITIONS "USE_TOOTLE"
		INTERFACE_INCLUDE_DIRECTORIES "${SDK_DIR}/AMD/Tootle-2.2.150/include"
	)
endif()

if(WIN64)
	set_target_properties(TOOTLE PROPERTIES IMPORTED_LOCATION "${SDK_DIR}/AMD/Tootle-2.2.150/bin/TootleDLL_2k8_64.dll")
	set_target_properties(TOOTLE PROPERTIES IMPORTED_IMPLIB "${SDK_DIR}/AMD/Tootle-2.2.150/lib/TootleDLL_2k8_64.lib")#
	set_target_properties(TOOTLE PROPERTIES INTERFACE_LINK_LIBRARIES "${SDK_DIR}/AMD/Tootle-2.2.150/lib/TootleDLL_2k8_64.lib")

	deploy_runtime_files("${SDK_DIR}/AMD/Tootle-2.2.150/bin/TootleDLL_2k8_64.dll")
elseif(WIN32)
	set_target_properties(TOOTLE PROPERTIES IMPORTED_LOCATION "${SDK_DIR}/TOOTLE/lib/win_x86/Tootle_2k8.dll")
	set_target_properties(TOOTLE PROPERTIES IMPORTED_IMPLIB "${SDK_DIR}/TOOTLE/lib/win_x86/TootleDLL_2k8.lib")
	set_target_properties(TOOTLE PROPERTIES INTERFACE_LINK_LIBRARIES "${SDK_DIR}/TOOTLE/lib/win_x86/TootleDLL_2k8.lib")

	deploy_runtime_files("${SDK_DIR}/TOOTLE/lib/win_x86/Tootle_2k8.dll")
endif()
