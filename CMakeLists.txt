# Z80 - CMakeLists.txt
#     ______  ______ ______
#    /\___  \/\  __ \\  __ \
#    \/__/  /\_\  __ \\ \/\ \
#       /\_____\\_____\\_____\
# Zilog \/_____//_____//_____/ CPU Emulator - Linux Kernel Module
# Copyright (C) 2022 Manuel Sainz de Baranda y Goñi.
# Released under the terms of the GNU General Public License v3.

cmake_minimum_required(VERSION 3.11)

if(	CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND
	NOT CMAKE_BUILD_TYPE				   AND
	NOT CMAKE_CONFIGURATION_TYPES
)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(Z80-LKM
	LANGUAGES C
	DESCRIPTION "Z80 emulator for Linux")

set(LINUX_KERNEL_HEADERS_DIR
"" CACHE STRING
"Directory of the Linux kernel headers.")

option(Z80_LKM_STABLE
"Download the latest stable versions of Z80 and Zeta rather than clone their \
Git repositories."
NO)

if(LINUX_KERNEL_HEADERS_DIR STREQUAL "")
	execute_process(COMMAND uname -r
		OUTPUT_VARIABLE _uname_r
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	find_path(
		_kernel_headers_dir
		"include/linux/user.h"
		PATHS	"/usr/src/linux-headers-${_uname_r}"
			"/usr/src/linux-${_uname_r}")

	if(NOT _kernel_headers_dir)
		message(FATAL_ERROR "Unable to locate the kernel headers.")
	else()
		set(LINUX_KERNEL_HEADERS_DIR "${_kernel_headers_dir}")

		message(STATUS
			"Found Linux kernel headers: ${LINUX_KERNEL_HEADERS_DIR} (found version \"${_uname_r}\")")
	endif()

	unset(_uname_r)
endif()

include(FetchContent)

if(${PROJECT_NAME}_STABLE)
	FetchContent_Declare(Zeta URL https://zeta.st/download/Zeta-latest.tar.xz)
	FetchContent_Declare(Z80  URL https://zxe.io/software/Z80/download/Z80-latest.tar.xz)
else()
	FetchContent_Declare(Zeta GIT_REPOSITORY https://github.com/redcode/Zeta.git)
	FetchContent_Declare(Z80  GIT_REPOSITORY https://github.com/redcode/Z80.git )
endif()

FetchContent_MakeAvailable(Zeta)

set(Z80_OBJECT_LIBS                 YES CACHE BOOL "")
set(Z80_WITH_EXECUTE                YES CACHE BOOL "")
set(Z80_WITH_FULL_IM0               YES CACHE BOOL "")
set(Z80_WITH_Q                      YES CACHE BOOL "")
set(Z80_WITH_ZILOG_NMOS_LD_A_IR_BUG YES CACHE BOOL "")

FetchContent_MakeAvailable(Z80)

add_custom_command(
	OUTPUT	"${CMAKE_CURRENT_BINARY_DIR}/Z80.o_shipped"
		"${CMAKE_CURRENT_BINARY_DIR}/Z80-LKM.c"
	COMMAND cp "$<TARGET_OBJECTS:Z80>" "${CMAKE_CURRENT_BINARY_DIR}/Z80.o_shipped" &&
		touch "${CMAKE_CURRENT_BINARY_DIR}/.Z80.o.cmd" &&
		cp "${CMAKE_CURRENT_SOURCE_DIR}/sources/Z80-LKM.c" "${CMAKE_CURRENT_BINARY_DIR}/Z80-LKM.c"
	DEPENDS Z80)

add_custom_target(copy_files DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Z80.o_shipped")
set(Z80_LKM_SOURCES "Z80-LKM.o Z80.o")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/support/Kbuild.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Kbuild"
	@ONLY)

set(_kbuild_command ${CMAKE_MAKE_PROGRAM} -C ${LINUX_KERNEL_HEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
	OUTPUT z80.ko
	COMMAND ${_kbuild_command} modules
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS copy_files "${CMAKE_CURRENT_BINARY_DIR}/Kbuild")

add_custom_target(module ALL DEPENDS z80.ko)
add_custom_target(module_clean COMMAND ${_kbuild_command} clean)
add_custom_target(mrproper COMMAND ${_kbuild_command} clean && ${CMAKE_MAKE_PROGRAM} clean)

# CMakeLists.txt EOF
