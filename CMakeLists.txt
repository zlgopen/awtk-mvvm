cmake_minimum_required(VERSION 3.16)

project(awtk-mvvm
  VERSION 1.0.0
  LANGUAGES C CXX
  DESCRIPTION "AWTK MVVM library"
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

option(AWTK_MVVM_WITH_JERRYSCRIPT "Build with JerryScript (requires 3rd/jerryscript CMake support; not implemented)" OFF)
option(AWTK_MVVM_WITH_IOTJS "Build with IoT.js (not implemented in CMake)" OFF)

if(AWTK_MVVM_WITH_JERRYSCRIPT OR AWTK_MVVM_WITH_IOTJS)
  message(FATAL_ERROR "AWTK_MVVM_WITH_JERRYSCRIPT / AWTK_MVVM_WITH_IOTJS are not supported in this CMake build yet. Turn them OFF.")
endif()

include(GNUInstallDirs)

find_package(awtk REQUIRED CONFIG)

get_filename_component(_awtk_install_prefix "${awtk_DIR}/../../.." ABSOLUTE)

set(_mvvm_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm")

file(GLOB _mvvm_base_src CONFIGURE_DEPENDS "${_mvvm_src_dir}/base/*.c")
file(GLOB _mvvm_hardware_src CONFIGURE_DEPENDS "${_mvvm_src_dir}/hardware/*.c")
file(GLOB _mvvm_awtk_src CONFIGURE_DEPENDS "${_mvvm_src_dir}/awtk/*.c")
file(GLOB _mvvm_view_models_src CONFIGURE_DEPENDS "${_mvvm_src_dir}/view_models/*.c")
file(GLOB _mvvm_cpp_src CONFIGURE_DEPENDS "${_mvvm_src_dir}/cpp/*.cpp")
set(_mvvm_root_src "${_mvvm_src_dir}/mvvm.c")

set(AWTK_MVVM_SOURCES
  ${_mvvm_base_src}
  ${_mvvm_hardware_src}
  ${_mvvm_awtk_src}
  ${_mvvm_view_models_src}
  ${_mvvm_cpp_src}
  ${_mvvm_root_src}
)

add_library(mvvm ${AWTK_MVVM_SOURCES})

add_library(awtk_mvvm::mvvm ALIAS mvvm)

set_target_properties(mvvm PROPERTIES
  OUTPUT_NAME mvvm
  EXPORT_NAME mvvm
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(mvvm PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(mvvm PRIVATE
  "${_awtk_install_prefix}/include/awtk/ext_widgets"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm/hardware"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm/awtk"
)

target_link_libraries(mvvm PUBLIC awtk::awtk)

# Windows DLL: mvvm.def lists JerryScript/JS exports for IDL consumers. When
# JerryScript is not built (CMake default; see AWTK_MVVM_WITH_JERRYSCRIPT),
# those symbols are absent — SConstruct strips the same lines via
# dll_def_processor(). Apply equivalent filtering so the link succeeds.
set(_mvvm_def_for_link "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm.def")
if(WIN32 AND BUILD_SHARED_LIBS AND NOT AWTK_MVVM_WITH_JERRYSCRIPT AND NOT AWTK_MVVM_WITH_IOTJS)
  set(_mvvm_def_src "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm.def")
  set(_mvvm_def_filtered "${CMAKE_CURRENT_BINARY_DIR}/mvvm-link.def")
  file(READ "${_mvvm_def_src}" _mvvm_def_content)
  string(REPLACE "\r\n" "\n" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+jerry[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+[^\n \t]*_jerryscript_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+jsvalue[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+js_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+jsobj_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+object_js_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+object_is_object_js_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  string(REGEX REPLACE "[ \t]+mvvm_iotjs_[^\n]*\n" "" _mvvm_def_content "${_mvvm_def_content}")
  file(WRITE "${_mvvm_def_filtered}" "${_mvvm_def_content}")
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_mvvm_def_src}")
  set(_mvvm_def_for_link "${_mvvm_def_filtered}")
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
  target_compile_definitions(mvvm PRIVATE MVVM_DLL_EXPORT)
  if(MSVC)
    target_link_options(mvvm PRIVATE "/DEF:${_mvvm_def_for_link}")
  else()
    target_link_options(mvvm PRIVATE "${_mvvm_def_for_link}")
  endif()
endif()

if(APPLE)
  set_target_properties(mvvm PROPERTIES INSTALL_RPATH "@loader_path;@loader_path/../lib")
elseif(UNIX)
  set_target_properties(mvvm PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib")
endif()

include(CMakePackageConfigHelpers)

set(_awtk_mvvm_cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/awtk-mvvm")

install(
  TARGETS mvvm
  EXPORT awtk-mvvmTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/mvvm/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mvvm
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp"
)

install(
  EXPORT awtk-mvvmTargets
  FILE awtk-mvvmTargets.cmake
  NAMESPACE awtk_mvvm::
  DESTINATION ${_awtk_mvvm_cmake_dir}
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/awtk-mvvmConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/awtk-mvvmConfig.cmake"
  INSTALL_DESTINATION ${_awtk_mvvm_cmake_dir}
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/awtk-mvvmConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/awtk-mvvmConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/awtk-mvvmConfigVersion.cmake"
  DESTINATION ${_awtk_mvvm_cmake_dir}
)
