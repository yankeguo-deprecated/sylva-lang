PROJECT(sylva)
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

SET(SYLVA_RUNTIME_TEST_SRCS

    runtime_test.c
    )

ADD_EXECUTABLE(sylva-runtime-test ${SYLVA_RUNTIME_TEST_SRCS})
SET_TARGET_PROPERTIES(sylva-runtime-test PROPERTIES C_STANDARD 99)

TARGET_LINK_LIBRARIES(sylva-runtime-test sylva-runtime-static)

ADD_TEST(runtime-test-run1 sylva-runtime-test)
