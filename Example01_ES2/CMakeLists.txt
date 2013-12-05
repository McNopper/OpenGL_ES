#
# Example01_ES2 CMake file
#
# (c) Norbert Nopper
# 

cmake_minimum_required(VERSION 2.6)

project(Example01_ES2)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Linux

	IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
		# Raspberry Pi
		# ToDo Add i.MX6
		
		add_definitions(-DGLUS_ES2)
		
		include_directories(/opt/vc/include /opt/vc/include/interface/vcos/pthreads /opt/vc/include/interface/vmcs_host/linux ${Example01_ES2_SOURCE_DIR}/../GLUS/src)		

		link_directories(/opt/vc/lib ${Example01_ES2_SOURCE_DIR}/../GLUS)
		
	ENDIF()
	
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# Windows
	
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
	# Desktop OpenGL ES 2.0

	add_definitions(-DGLUS_ES2)
	
	include_directories(C:\\Imagination\\PowerVR\\GraphicsSDK\\SDK_3.1\\Builds\\Include ${Example01_ES2_SOURCE_DIR}/../GLUS/src)	
	
	link_directories(C:\\Imagination\\PowerVR\\GraphicsSDK\\SDK_3.1\\Builds\\Windows\\x86_32\\Lib ${Example01_ES2_SOURCE_DIR}/../GLUS)
			
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Example01_ES2_SOURCE_DIR}/../Binaries)

# Source files
file(GLOB C_FILES ${Example01_ES2_SOURCE_DIR}/src/*.c)

# Header files
file(GLOB_RECURSE H_FILES ${Example01_ES2_SOURCE_DIR}/src/*.h)

add_executable(Example01_ES2 ${C_FILES} ${H_FILES})
	
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Linux

	IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
		# Raspberry Pi
		# ToDo Add i.MX6
		
		target_link_libraries(Example01_ES2 SDL GLUS EGL GLESv2 bcm_host vcos vchiq_arm pthread rt m)
		
	ENDIF()
	
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# Windows

	target_link_libraries(Example01_ES2 GLUS libEGL libGLESv2)

	message("Executable is deployed either to Binaries/Release or Binaries/Debug.")
	message("Copy the executable to the Binaries folder.")
	message("CMAKE_RUNTIME_OUTPUT_DIRECTORY is set to Binaries, but Release/Debug is appended.")
				
ENDIF()
