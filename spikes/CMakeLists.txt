find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(Boost REQUIRED COMPONENTS python)

# pyembed Boost.Python spike — reference artifact (Phase A-C).
# Single shared domain lib (mirrors libReglas): ONE RTTI source for Widget.
add_library(widget SHARED pyembed/widget.cpp)
target_include_directories(widget PUBLIC pyembed)

# Spike.so = Boost.Python extension (mirrors Reglas.so).
add_library(Spike MODULE pyembed/module.cpp)
set_target_properties(Spike PROPERTIES PREFIX "" SUFFIX ".so")
target_link_libraries(Spike PRIVATE widget Boost::python Python3::Module)
if(APPLE)
  target_link_options(Spike PRIVATE -Wl,-undefined,dynamic_lookup)
endif()

# Embedder (mirrors consola).
add_executable(pyembed_spike pyembed/main.cpp)
target_link_libraries(pyembed_spike PRIVATE widget Boost::python Python3::Python)
add_dependencies(pyembed_spike Spike)

# Engine spike: IrrlichtMt provided by the flake's irrlicht-fork derivation.
find_package(IrrlichtMt CONFIG REQUIRED)
add_executable(engine_spike engine/main.cpp)
target_link_libraries(engine_spike PRIVATE IrrlichtMt::IrrlichtMt)
if(APPLE)
  target_link_libraries(engine_spike PRIVATE
    "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()

# pybind11 round-trip spike (D1.5 — analog of pyembed but with pybind11).
add_subdirectory(pybind)
