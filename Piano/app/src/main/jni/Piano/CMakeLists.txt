cmake_minimum_required(VERSION 3.18.1)

project(Piano)

add_subdirectory(../Music ./music)
add_subdirectory(../Midi ./midi)
add_library(kspiano STATIC PianoSynthNode.cpp PianoState.cpp )
target_link_libraries(kspiano music midi)