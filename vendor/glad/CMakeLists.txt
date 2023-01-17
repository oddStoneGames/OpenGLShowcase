add_library(glad glad.c include/glad.h include/khrplatform.h)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})