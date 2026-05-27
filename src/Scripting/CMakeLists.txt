file(GLOB SCRIPTING_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Scripting SHARED ${SCRIPTING_SRC})
target_include_directories(Scripting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(Boost REQUIRED COMPONENTS python filesystem system)

target_link_libraries(Scripting PUBLIC
  Reglas
  Boost::python Boost::filesystem Boost::system
  Python3::Python)
