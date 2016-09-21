project(sylva)
cmake_minimum_required(VERSION 3.6)

set(SYLVA_CLI_SRCS

    sylva-cli/cli.c)

set(SYLVA_LIB_SRCS

    sylva/lexer.c
    sylva/token.c
    sylva/string.c sylva/parser.c)

set(SYLVA_RT_SRCS

    sylva-runtime/runtime.c)

# libsylva-runtime
add_library(sylva-runtime-static STATIC ${SYLVA_RT_SRCS})
add_library(sylva-runtime-shared SHARED ${SYLVA_RT_SRCS})
set_target_properties(sylva-runtime-static PROPERTIES OUTPUT_NAME "sylva-runtime")
set_target_properties(sylva-runtime-shared PROPERTIES OUTPUT_NAME "sylva-runtime")

# libsylva
add_library(sylva-static STATIC ${SYLVA_LIB_SRCS})
add_library(sylva-shared SHARED ${SYLVA_LIB_SRCS})

set_target_properties(sylva-static PROPERTIES OUTPUT_NAME "sylva")
set_target_properties(sylva-shared PROPERTIES OUTPUT_NAME "sylva")

target_link_libraries(sylva-static sylva-runtime-static)
target_link_libraries(sylva-shared sylva-runtime-static)

# sylva-cli
add_executable(sylva-cli ${SYLVA_CLI_SRCS})
target_link_libraries(sylva-cli sylva-static)
