#pragma once
#include <irrlicht.h>
#include <vector>

// One recorded match event. Carries its own world coordinates so the
// presentation can replay it regardless of how far the Reglas model has advanced.
struct EventoJugada {
    enum Tipo { MOVIMIENTO, BARRERA } tipo;
    int jugador = 0;
    irr::core::vector3df desde;   // MOVIMIENTO: pawn start world pos
    irr::core::vector3df hasta;   // MOVIMIENTO: pawn end; BARRERA: final slot
    int numCeldas = 1;            // MOVIMIENTO: cells jumped (jump height hint)
    int barreraIdx = -1;          // BARRERA: index into PartidaGrafica::Barreras
};
