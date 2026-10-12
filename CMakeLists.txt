cmake_minimum_required(VERSION 3.14)

project (OpenGL_ESExamples VERSION 1.0.0)

# Build policy + static analysis. Registered ONLY when this is the top-level
# project: GLUS is vendored through FetchContent, and a second `cppcheck`
# custom target would make CMake fail that build outright. None of this is
# required to build the examples - with the tools absent and every option at
# its default the build is unchanged.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
	include(cppcheck)
	include(warnings)
endif()

include(FetchContent)

# macOS is not supported: GLUS has no macOS ES window backend, and the ES
# providers are ANGLE (Windows) and the system EGL/GLES stack (Linux).
if(APPLE)
	message(STATUS "OpenGL_ES: macOS is not supported (no ES window backend in GLUS) - nothing to build.")
	return()
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Binaries)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# Force single output directory (no Debug/Release subdirectories)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/Binaries)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
endforeach()

# OpenGL ES provider - the same optional ANGLE handling as McNopper/EGL:
#   Windows: Google ANGLE, provided through the vcpkg manifest (vcpkg.json).
#   Linux:   the system EGL/GLES stack (Mesa on the desktop).
# When none is present, the examples are skipped with a status message and
# the configure still succeeds.
set(GLES_PROVIDER "")
if(WIN32)
	find_package(unofficial-angle CONFIG QUIET)
	if(unofficial-angle_FOUND)
		message(STATUS "OpenGL_ES: ANGLE found - building the examples against it")
		set(GLES_PROVIDER "ANGLE")
		set(ES_LIBRARIES unofficial::angle::libEGL unofficial::angle::libGLESv2)
		# The unofficial-angle targets carry no include usage requirements, so
		# their header root is located explicitly: manifest installs live in the
		# build tree; a system-wide vcpkg install is found by the default search.
		find_path(ES_INCLUDE_ROOT EGL/egl.h
			PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
			NO_DEFAULT_PATH)
		if(NOT ES_INCLUDE_ROOT)
			find_path(ES_INCLUDE_ROOT EGL/egl.h)
		endif()
	else()
		message(STATUS "OpenGL_ES: ANGLE not found - the examples need it on Windows. "
			"Run 'vcpkg install' or configure with -DCMAKE_TOOLCHAIN_FILE=<vcpkg.cmake> to enable them.")
	endif()
else()
	find_library(EGL_LIBRARY EGL)
	find_library(GLESV2_LIBRARY GLESv2)
	find_path(ES_INCLUDE_ROOT EGL/egl.h)
	if(EGL_LIBRARY AND GLESV2_LIBRARY AND ES_INCLUDE_ROOT)
		message(STATUS "OpenGL_ES: building the examples against the system EGL/GLES")
		set(GLES_PROVIDER "SYSTEM")
		set(ES_LIBRARIES ${EGL_LIBRARY} ${GLESV2_LIBRARY})
	else()
		message(STATUS "OpenGL_ES: system EGL/GLES not found - skipping the examples "
			"(install the Mesa EGL/GLES development packages to enable them).")
	endif()
endif()

if(GLES_PROVIDER STREQUAL "")
	return()
endif()

# GLES3 headers are optional: ES 2.0-only platforms (for example the Vivante
# GPU of the STM32MP1) build the *_ES2 examples only. The examples use
# OpenGL ES 3.0 at most, so the GLUS "ES" flavor is selected whenever GLES3
# headers exist and ES 2.0 otherwise.
if(EXISTS "${ES_INCLUDE_ROOT}/GLES3/gl3.h")
	set(HAVE_GLES3 TRUE)
	set(OpenGL "ES" CACHE STRING "GLUS flavor" FORCE)
else()
	set(HAVE_GLES3 FALSE)
	message(STATUS "OpenGL_ES: GLES3 headers not found - building the ES 2.0 examples only")
	set(OpenGL "ES2" CACHE STRING "GLUS flavor" FORCE)
endif()

# GLUS: prefer the sibling local checkout (multi-repo development layout), and
# fall back to the published release when it is not present.
if(EXISTS "${CMAKE_SOURCE_DIR}/../GLUS/CMakeLists.txt")
    FetchContent_Declare(
        glus
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/../GLUS
    )
else()
    FetchContent_Declare(
        glus
        GIT_REPOSITORY https://github.com/McNopper/GLUS.git
        GIT_TAG        v1.1.6
    )
endif()
FetchContent_MakeAvailable(glus)

# GLUS compiles its EGL window against the ES provider's headers and resolves
# the EGL/GLES entry points through the same libraries as the examples.
# The platform libraries resolve the native window and timer symbols inside
# GLUS itself, so they are PUBLIC dependencies too: the linker needs them
# after the static GLUS library, which is exactly how CMake orders them.
# The fbdev windowing (SoC=fbdev) takes its native window from the platform
# EGL and its input from evdev, so it needs no X11 at all.
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(SoC MATCHES "fbdev")
        set(GLUS_PLATFORM_LIBRARIES dl pthread m)
    elseif(SoC MATCHES "Wayland")
        # wl_* and wl_egl_window_* inside GLUS resolve through wayland-client
        # and wayland-egl.
        set(GLUS_PLATFORM_LIBRARIES wayland-client wayland-egl dl pthread m)
    else()
        set(GLUS_PLATFORM_LIBRARIES X11 Xrandr dl pthread m)
    endif()
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GLUS_PLATFORM_LIBRARIES winmm)
ENDIF()

target_include_directories(GLUS PUBLIC ${ES_INCLUDE_ROOT})
target_link_libraries(GLUS PUBLIC ${ES_LIBRARIES} ${GLUS_PLATFORM_LIBRARIES})

if(GLES_PROVIDER STREQUAL "ANGLE")
	# Copy the ANGLE runtime DLLs next to the executables, so they run as built.
	add_custom_target(angle_runtime ALL
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:unofficial::angle::libEGL>
			$<TARGET_FILE:unofficial::angle::libGLESv2>
			${CMAKE_SOURCE_DIR}/Binaries)
endif()

file(GLOB EXAMPLES_LIST LIST_DIRECTORIES true RELATIVE ${CMAKE_SOURCE_DIR} "Example*")
foreach (EXAMPLE ${EXAMPLES_LIST})
	if(NOT HAVE_GLES3 AND NOT EXAMPLE MATCHES "_ES2$")
		message(STATUS "Skip (needs GLES3): " ${EXAMPLE})
		continue()
	endif()
	message(STATUS "Add Subdirectory: " ${EXAMPLE})
	add_subdirectory(${EXAMPLE})
endforeach()

# Warnings are applied per example target and only here - AFTER the dependency
# setup above - so GLUS and ANGLE keep building exactly as before, and every
# warning that appears is ours. At the default settings this loop adds no
# flags at all.
if(COMMAND project_enable_warnings)
	foreach(_ex IN LISTS EXAMPLES_LIST)
		get_filename_component(_ex_target ${_ex} NAME)
		if(TARGET ${_ex_target})
			project_enable_warnings(${_ex_target})
		endif()
	endforeach()
endif()
