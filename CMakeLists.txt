cmake_minimum_required(VERSION 3.25)
project(quassimodo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QM_BUILD_SPIKES "Build the de-risking spikes" ON)

if(QM_BUILD_SPIKES)
  add_subdirectory(spikes)
endif()

option(QM_BUILD_GAME "Build the real game modules" ON)
if(QM_BUILD_GAME)
  add_subdirectory(src)
endif()
