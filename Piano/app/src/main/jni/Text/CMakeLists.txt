cmake_minimum_required(VERSION 3.18.1)


add_library(kstextlib STATIC Text.cpp)

include_directories(${FREETYPE}/include)
add_library(ft2 STATIC IMPORTED)
set_target_properties(ft2 PROPERTIES IMPORTED_LOCATION ${FREETYPE}/${CMAKE_ANDROID_ARCH_ABI}/libft.a)

target_link_libraries(kstextlib ft2)
