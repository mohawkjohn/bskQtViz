# Specify the version used
cmake_minimum_required(VERSION 3.0.2)

# Set project name - based on current directory
# Should give you the option to change in the CMake gui
get_filename_component(dirName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXE_NAME ${dirName} CACHE STRING "Name of executable to be created.")
project(${EXE_NAME})

# Specify the available build types for multi-configuration generators
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE INTERNAL "Configuration types")

# Specify the directory that simulation code is located in
# Should give you the option to change in the CMake gui
# set(SIMULATION_DIR ${CMAKE_SOURCE_DIR}/../Simulation/ CACHE PATH "Directory containing simulation source code")

# Put all the runtime targets in one folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

# Add in QT
# Note: This is a windows path, but you will be able to change this in the cmake gui!
if(WIN32)
    set (CMAKE_PREFIX_PATH "C:/Qt5.5.1/5.5/msvc2013/lib/cmake" CACHE PATH "qt cmake prefix path")
elseif(APPLE)
    set (CMAKE_PREFIX_PATH "~/Qt5.5.1/5.5/clang_64/lib/cmake" CACHE PATH "qt cmake prefix path")
endif()

# Widgets finds its own dependencies (QtGui and QtCore)
find_package(Qt5Widgets CONFIG REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# As moc files are generated in the binary dir, tell CMake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up macro for adding sources
macro(add_sources)
    file(RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach(_src ${ARGN})
        if(_relPath)
            list(APPEND SRCS "${_relPath}/${_src}")
        else()
            list(APPEND SRCS "${_src}")
        endif()
    endforeach()
    if(_relPath)
        # propagate SRCS to parent directory
        set(SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

# Add source files in root directory
add_sources(main.cpp
    adcssimdatamanager.cpp
    adcssimdatamanager.h
    camera.cpp
    camera.h
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    renderer.cpp
    renderer.h
    resources.qrc
    scenewidget.cpp
    scenewidget.h
    simdatamanager.cpp
    simdatamanager.h
    simobject.cpp
    simobject.h
    visualizationMacros.h
    defaultStyle.qss
    SpacecraftSimDefinitions.h
    SpacecraftSimDefinitions.cpp
    spacecraftDefinitions.h
    jsonParser.cpp
    jsonParser.hpp
)

# Add subdirectories
add_subdirectory(dialogs)
add_subdirectory(displays)
add_subdirectory(doc)
add_subdirectory(external/qdarkstyle)
add_subdirectory(geometries)
add_subdirectory(shaders)
add_subdirectory(utilities)
add_subdirectory(communication)
add_subdirectory(communication/tcp)
add_subdirectory(communication/udp)

# Setup include directories
include_directories(dialogs)
include_directories(displays)
include_directories(external/assimp_3_1_1/include)
include_directories(external/boost_1_61_0)
include_directories(external/cspice/include)
include_directories(external/qdarkstyle)
include_directories(geometries)
include_directories(utilities)
include_directories(communication)
include_directories(communication/tcp)
include_directories(communication/udp)
include_directories(external/WMM)
include_directories(external)



# Generate Visual Studio file filters
foreach(item ${SRCS})
    get_filename_component(dir ${item} PATH)
    list(APPEND dirs ${dir})
    list(APPEND ${dir}_FILES ${item})
endforeach()
list(REMOVE_DUPLICATES dirs)
foreach(item ${dirs})
    source_group(${item} FILES ${${item}_FILES})
endforeach()
source_group("" FILES ${_FILES})

# Create list of libraries depending on system
if(WIN32)
    if(MSVC_VERSION EQUAL 1800) # VS2013
        set(library_dependencies
            ${Qt5Widgets_LIBRARIES}
            ${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/assimp.lib
            ${CMAKE_SOURCE_DIR}/external/cspice/lib/cspice-vc120-mt.lib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_filesystem-vc120-mt-1_58.lib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_system-vc120-mt-1_58.lib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_serialization-vc120-mt-gd-1_58.lib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_thread-vc120-mt-1_58.lib
        )
    else()
        message(FATAL_ERROR "Only Visual Studio 2013 is currently supported")
    endif()
elseif(APPLE)
    set(library_dependencies
            ${Qt5Widgets_LIBRARIES}
            ${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.3.1.1.dylib
            ${CMAKE_SOURCE_DIR}/external/cspice/lib/cspice.a
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_filesystem.dylib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_system.dylib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_serialization.dylib
            ${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_thread.dylib
    )
else()
    message(FATAL_ERROR "Only WIN32 and APPLE systems are currently supported")
endif()

# Add compiler flags 
if(MSVC)
    add_definitions(/D BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
    add_definitions(/D BOOST_MPL_LIMIT_LIST_SIZE=50)
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)
    add_definitions(/D _WIN32_WINNT=0x0501) # Targets Windows xp
    add_definitions(/D _WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(/D BOOST_ALL_NO_LIB)
    add_definitions(/D WIN32_LEAN_AND_MEAN)
#    add_definitions(/W3)
#    add_definitions(/wd"4913")
    foreach(flag 
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUGwd
        CMAKE_CXX_FLAGS_RELEASE)
        set("${flag}" "${${flag}} /EHsc ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    endforeach()
elseif(APPLE)
    add_definitions(-D BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
    add_definitions(-D BOOST_MPL_LIMIT_LIST_SIZE=50)
	add_definitions(-D QT_DEBUG_PLUGINS=1)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif()

# Generate executable
if(WIN32)
    add_executable(${EXE_NAME} WIN32 ${SRCS})
else()
    add_executable(${EXE_NAME} ${SRCS})
endif()

# Set up library dependencies
target_link_libraries(${EXE_NAME} debug ${library_dependencies})
target_link_libraries(${EXE_NAME} optimized ${library_dependencies})

# Add the dynamic libraries to the build folder
if(WIN32)
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        DEPENDS ${EXE_NAME}
        # Debug version
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Guid.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Cored.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/icuin53.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/icuuc53.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/assimp.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_filesystem-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_system-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_serialization-vc120-mt-gd-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_thread-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_chrono-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_date_time-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/cspice/lib/cspice-vc120-mt.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/cspice.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/images" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/images"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/models"
        # Release version
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Gui.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/Qt5Core.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/icuin53.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${Qt5Core_DIR}/../../../bin/icuuc53.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/assimp.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_filesystem-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_system-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_serialization-vc120-mt-gd-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_thread-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_chrono-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/boost_date_time-vc120-mt-1_58.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/cspice/lib/cspice-vc120-mt.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/cspice.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/images" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/images"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/models"
    )
elseif(APPLE)
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        DEPENDS ${EXE_NAME}
        # Debug version
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.3.1.1.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.3.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_filesystem.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_system.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_serialization.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_thread.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/images" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/images"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/models"
        # Release version
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.3.1.1.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.3.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/assimp_3_1_1/lib/libassimp.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_filesystem.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_system.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_serialization.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/boost_1_61_0/lib/libboost_thread.dylib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/images" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/images"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/models" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/models"
    )
endif()
