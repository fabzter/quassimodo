find_package(pybind11 CONFIG REQUIRED)

# Single shared domain lib — single source of RTTI for Cell/Move/Player
# (mirrors libReglas in the real game).
add_library(pybindspike_domain SHARED
    domain.cpp
)
target_include_directories(pybindspike_domain PUBLIC .)
set_target_properties(pybindspike_domain PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Extension module: separate .so, imported at runtime by the embedder.
# Built as a MODULE; on APPLE, -undefined dynamic_lookup so Python C-API
# resolves from the host process at load time.
pybind11_add_module(PybindSpike MODULE bindings.cpp)
set_target_properties(PybindSpike PROPERTIES
    OUTPUT_NAME "PybindSpike"
    PREFIX ""
    SUFFIX ".so"
)
target_link_libraries(PybindSpike PRIVATE pybindspike_domain)
if(APPLE)
    target_link_options(PybindSpike PRIVATE -Wl,-undefined,dynamic_lookup)
endif()
