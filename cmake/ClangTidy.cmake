# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if(OPTION_CLANG_TIDY)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy-16" "clang-tidy-17"
    DOC "Path to clang-tidy executable")

  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "unable to locate clang-tidy")

    macro(set_tidy_target_properties TARGET_NAME)
    endmacro()

  else()
    set(RUN_CLANG_TIDY_BIN_ARGS
      "${CLANG_TIDY_EXE}"
      "--config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy"
      "-p=${CMAKE_CURRENT_BINARY_DIR}")

    macro(tidy_target target_name)
      set_target_properties(
        ${target_name} PROPERTIES
        CXX_CLANG_TIDY "${RUN_CLANG_TIDY_BIN_ARGS}")
    endmacro()
  endif()

else()
  macro(tidy_target target_name)
  endmacro()
endif()
