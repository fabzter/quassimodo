# pyembed Boost.Python spike — DISABLED (boost_python removed in D1').
# The D1.5 pybind11 spike and D1' migration proved the replacement.
# Uncomment and re-add boostPython to flake.nix if needed for reference.
# find_package(Boost REQUIRED COMPONENTS python)
# add_library(widget SHARED pyembed/widget.cpp)
# ...
# add_executable(pyembed_spike pyembed/main.cpp)
# ...

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
