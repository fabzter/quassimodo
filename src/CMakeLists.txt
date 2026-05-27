# D1 builds the non-graphical game. Grafico + the graphical Partida path stay
# excluded while QM_CONSOLE_ONLY is ON (flipped OFF in Phase D2).
option(QM_CONSOLE_ONLY "Build console-only (exclude Grafico/PartidaGrafica)" ON)

add_subdirectory(Reglas)
add_subdirectory(Opciones)
add_subdirectory(Scripting)
