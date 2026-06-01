find_package(IrrlichtMt CONFIG REQUIRED)

# D2.1 subset — EXCLUDES Menu/GUI/ManejadorGUI (menu, deferred to D2.2) and
# JumpAnimator/tokayoCamera/IAnimatorCameraTokayo (animator subsystem removed
# from the IrrlichtMt fork, deferred to D2.2).
add_library(Grafico SHARED
    Tablero.cpp Celda.cpp Pieza.cpp Jugador.cpp Barrera.cpp
    Skin.cpp SkinLoader.cpp CImageGUISkin.cpp CGUIProgressBar.cpp
    CConfigReader.cpp CConfigMap.cpp Antorcha.cpp Video.cpp Exepciones.cpp
)
target_include_directories(Grafico PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Grafico PUBLIC Reglas Opciones IrrlichtMt::IrrlichtMt)
if(APPLE)
    target_link_libraries(Grafico PRIVATE
        "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif()
