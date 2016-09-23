PROJECT(sylva)
CMAKE_MINIMUM_REQUIRED(VERSION 3.6)

SET(SYLVA_CLI_SRCS

    sylva-cli/cli.c)

SET(SYLVA_LIB_SRCS

    sylva/lexer.c
    sylva/token.c
    sylva/string.c
    sylva/parser.c)

SET(SYLVA_RT_SRCS

    sylva-runtime/runtime.c)

# libsylva-runtime
ADD_LIBRARY(sylva-runtime-static STATIC ${SYLVA_RT_SRCS})
ADD_LIBRARY(sylva-runtime-shared SHARED ${SYLVA_RT_SRCS})
SET_TARGET_PROPERTIES(sylva-runtime-static PROPERTIES OUTPUT_NAME "sylva-runtime")
SET_TARGET_PROPERTIES(sylva-runtime-shared PROPERTIES OUTPUT_NAME "sylva-runtime")

# libsylva
ADD_LIBRARY(sylva-static STATIC ${SYLVA_LIB_SRCS})
ADD_LIBRARY(sylva-shared SHARED ${SYLVA_LIB_SRCS})

SET_TARGET_PROPERTIES(sylva-static PROPERTIES OUTPUT_NAME "sylva")
SET_TARGET_PROPERTIES(sylva-shared PROPERTIES OUTPUT_NAME "sylva")

TARGET_LINK_LIBRARIES(sylva-static sylva-runtime-static)
TARGET_LINK_LIBRARIES(sylva-shared sylva-runtime-static)

# sylva-cli
ADD_EXECUTABLE(sylva-cli ${SYLVA_CLI_SRCS})
TARGET_LINK_LIBRARIES(sylva-cli sylva-static)
