cmake_minimum_required(VERSION 3.5)
project(python_plugin)

# Find source files
find_source_files(ALL_SOURCE_FILES)
find_source_files(ALL_SOURCE_FILES "${REPOSITORY_DIR}/runtime/sdk")

# Define static versus dynamic
if( NOT BUILD_SHARED_LIBS )
	add_compile_options(-DSTATIC_LINKING)
elseif( PLATFORM_WINDOWS )
	include_directories(${PROJECT_SOURCE_DIR})
	configure_file("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.rc.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.rc")
	set(RESOURCE_FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.rc")
	source_group("Resources" FILES ${RESOURCE_FILES})
	list(APPEND ALL_SOURCE_FILES ${RESOURCE_FILES})
endif()

# Create target and set compile/link options
add_compile_options(-DPLUGIN_NAMESPACE=${PROJECT_NAME})
find_package(PythonInterp)
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${REPOSITORY_DIR}/runtime/sdk)
add_library(${PROJECT_NAME} ${ALL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

# Set target properties
set_system_properties(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "${ENGINE_PLUGINS_FOLDER_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${ENGINE_PLUGIN_SUFFIX}_$<LOWER_CASE:$<CONFIG>>)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${ENGINE_PLUGINS_INSTALL_DIR}")
if( BUILD_SHARED_LIBS AND PLATFORM_IOS )
	set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${ENGINE_IOS_CODE_SIGN_IDENTITY}")
endif()

# Add files to package manifest
add_package_manifest_files(${PROJECT_NAME} $<TARGET_FILE:${PROJECT_NAME}>)