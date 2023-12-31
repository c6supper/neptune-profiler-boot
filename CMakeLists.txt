# ----------------------------------------------------------------
# Copyright (c) Coding Nerd
# Licensed under the Apache License, Version 2.0
# See LICENSE in the project root for license information.
# ----------------------------------------------------------------
# INSTALL default: off
option(INSTALL "Install neptune binaries and files" OFF)
option(DOC "Generate and update the doc files" OFF)
option(OPTION_CLANG_TIDY "Analyze code with clang-tidy." ON)
option(OPTION_CPPCHECK "Analyze code with cppcheck." OFF)
option(OPTION_TARGET "Build Target" ON)

if(OPTION_CPPCHECK)
  cmake_minimum_required(VERSION 3.10)
elseif(OPTION_CLANG_TIDY)
  cmake_minimum_required(VERSION 3.6)
else()
  cmake_minimum_required(VERSION 3.5.1)
endif()

project(neptune-profiler-boot)

# Target name
set(target boot_perf)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(OPTION_TARGET)
  set(TARGET_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/${CMAKE_SYSTEM_PROCESSOR})

  file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  )

  message(DEBUG "CMAKE_CURRENT_LIST_DIR=${CMAKE_CURRENT_LIST_DIR}")
  message(DEBUG "SRC_FILES=${SRC_FILES}")
  message(STATUS "${SRC_FILES}")

  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/inc")

  add_executable(${target}
    ${SRC_FILES}
  )

  if(OPTION_CLANG_TIDY)
    include(ClangTidy)

    # ------------------------------------------------------------------------------
    # Code analysis
    # ------------------------------------------------------------------------------
    tidy_target(${target})
  endif(OPTION_CLANG_TIDY)

  install(TARGETS ${target} DESTINATION ${TARGET_DIRECTORY}/bin)
endif(OPTION_TARGET)

if(DOC)
  set(DOC_FILE "${CMAKE_CURRENT_LIST_DIR}/doc/neptune-profiler-boot-doc.md")
  add_custom_command(
    OUTPUT ${DOC_FILE}
    COMMAND ${PROTOC}
    ARGS --doc_out=markdown,neptune-profiler-boot-doc.md:${CMAKE_CURRENT_LIST_DIR}/doc
    -I "${CMAKE_CURRENT_LIST_DIR}/boot/"
    -I "${CMAKE_CURRENT_LIST_DIR}/service/boot/"
    "${CMAKE_CURRENT_LIST_DIR}/boot/*.boot"
    "${CMAKE_CURRENT_LIST_DIR}/boot/service/*.boot"
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/boot/*.boot" "${CMAKE_CURRENT_LIST_DIR}/boot/service/*.boot")

  add_custom_target(doc ALL DEPENDS ${DOC_FILE})
endif(DOC)
