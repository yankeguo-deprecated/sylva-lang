PROJECT(sylva)
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

SET(SYLVA_CLI_SRCS

    cli/cli.c)

SET(SYLVA_LIB_SRCS

    lib/lexer.c
    lib/token.c
    lib/string.c
    lib/parser.c)

SET(SYLVA_RT_SRCS

    runtime/runtime.c
    runtime/object.c
    runtime/value.c
    runtime/args.c
    runtime/func.c
    runtime/member.c
    ../foundation/foundation.c)

# libsylva-runtime (static/shared)
ADD_LIBRARY(sylva-runtime-static STATIC ${SYLVA_RT_SRCS})
ADD_LIBRARY(sylva-runtime-shared SHARED ${SYLVA_RT_SRCS})

SET_TARGET_PROPERTIES(sylva-runtime-static PROPERTIES OUTPUT_NAME "sylva-runtime")
SET_TARGET_PROPERTIES(sylva-runtime-static PROPERTIES C_STANDARD 99)

SET_TARGET_PROPERTIES(sylva-runtime-shared PROPERTIES OUTPUT_NAME "sylva-runtime")
SET_TARGET_PROPERTIES(sylva-runtime-shared PROPERTIES C_STANDARD 99)

TARGET_LINK_LIBRARIES(sylva-runtime-static m)
TARGET_LINK_LIBRARIES(sylva-runtime-shared m)

# libsylva (static/shared)
ADD_LIBRARY(sylva-static STATIC ${SYLVA_LIB_SRCS})
ADD_LIBRARY(sylva-shared SHARED ${SYLVA_LIB_SRCS})

SET_TARGET_PROPERTIES(sylva-static PROPERTIES OUTPUT_NAME "sylva")
SET_TARGET_PROPERTIES(sylva-shared PROPERTIES OUTPUT_NAME "sylva")
SET_TARGET_PROPERTIES(sylva-static PROPERTIES C_STANDARD 99)
SET_TARGET_PROPERTIES(sylva-shared PROPERTIES C_STANDARD 99)

# sylva (executable)
ADD_EXECUTABLE(sylva-cli ${SYLVA_CLI_SRCS})
SET_TARGET_PROPERTIES(sylva-cli PROPERTIES OUTPUT_NAME "sylva")
SET_TARGET_PROPERTIES(sylva-cli PROPERTIES C_STANDARD 99)

TARGET_LINK_LIBRARIES(sylva-cli sylva-static)
