# Console AI-vs-AI match runner (recovered from the old 'Ejecutable' project,
# deleted at 3de1f4b, renamed to 'consola'). Depends ONLY on Reglas + Scripting
# — no Grafico. A secondary executable target alongside the graphical Aplicacion.
add_executable(consola main.cpp Partida.cpp)
target_include_directories(consola PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(consola PRIVATE Reglas Scripting)
# Locate sibling dylibs (libReglas/libScripting) at runtime.
set_target_properties(consola PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>;$<TARGET_FILE_DIR:Scripting>")
