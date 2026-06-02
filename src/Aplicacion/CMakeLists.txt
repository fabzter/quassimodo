find_package(IrrlichtMt CONFIG REQUIRED)

# D2.1 lean graphical match driver. EXCLUDES main.cpp/Aplicacion.cpp/
# ManejadorJuego.cpp/EventReceiver.cpp/PartidaConsola.cpp (menu-centric app,
# deferred to D2.2). Reuses PartidaGrafica (graphical match logic) + Partida.
add_executable(aplicacion
    aplicacion_main.cpp
    PartidaGrafica.cpp
    Partida.cpp
)
target_include_directories(aplicacion PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(aplicacion PRIVATE
    Grafico Reglas Scripting Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
    target_link_libraries(aplicacion PRIVATE
        "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
set_target_properties(aplicacion PROPERTIES
    BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>;$<TARGET_FILE_DIR:Grafico>;$<TARGET_FILE_DIR:Scripting>;$<TARGET_FILE_DIR:Opciones>")
