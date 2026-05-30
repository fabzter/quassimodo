find_package(IrrlichtMt CONFIG REQUIRED)

add_executable(d2_render
    main.cpp
    scene_builder.cpp
)
target_include_directories(d2_render PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src        # for <Reglas/...>
)
# Reuse the existing shared Reglas lib (single RTTI source) + the engine.
target_link_libraries(d2_render PRIVATE Reglas IrrlichtMt::IrrlichtMt)
if(APPLE)
    target_link_libraries(d2_render PRIVATE
        "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
