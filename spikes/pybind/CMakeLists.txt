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
