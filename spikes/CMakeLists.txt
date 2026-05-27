add_executable(hello hello.cpp)

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(Boost REQUIRED COMPONENTS python)

# Single shared domain lib (mirrors libReglas): ONE RTTI source for Widget,
# linked by BOTH the module and the embedder.
add_library(widget SHARED pyembed/widget.cpp)
target_include_directories(widget PUBLIC pyembed)

# Spike.so = the Boost.Python extension, a SEPARATE module (mirrors Reglas.so).
add_library(Spike MODULE pyembed/module.cpp)
set_target_properties(Spike PROPERTIES PREFIX "" SUFFIX ".so")
target_link_libraries(Spike PRIVATE widget Boost::python Python3::Module)
if(APPLE)
  target_link_options(Spike PRIVATE -Wl,-undefined,dynamic_lookup)
endif()

# Embedder = aplicacion/libScripting stand-in. Links the SAME shared boost_python
# and the SAME libwidget; does NOT link the module — it imports Spike.so at runtime.
add_executable(pyembed_spike pyembed/main.cpp)
target_link_libraries(pyembed_spike PRIVATE widget Boost::python Python3::Python)
add_dependencies(pyembed_spike Spike)
