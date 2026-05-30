#pragma once
#include <irrlicht.h>

namespace Reglas { class Tablero; }

namespace d2spike {

struct SceneRefs {
    irr::scene::IAnimatedMeshSceneNode* piece0 = nullptr; // player 0's piece node
    irr::scene::IAnimatedMeshSceneNode* piece1 = nullptr; // player 1's piece node
};

// Maps grid cell (x,y) to a world position. Centred grid (cell (4,4) at origin).
irr::core::vector3df cellToWorld(int x, int y);

// Builds the scene tree from a REAL Reglas::Tablero:
//   - the board mesh (recentred on the world origin)
//   - a Celda node at every (x,y) in the 9x9 grid
//   - player 0's piece (Mono.obj) and player 1's piece (MonoBlanco.obj) at
//     their current cells (from t.getCelda(0)/t.getCelda(1))
//   - one Barrera node between two adjacent cells
// Returns the piece node refs so the caller (Task 4) can move them.
SceneRefs buildScene(irr::IrrlichtDevice* dev, const Reglas::Tablero& t);

// Re-position a piece node to the world coords of grid cell (x,y).
void placePieceAtCell(irr::scene::IAnimatedMeshSceneNode* node, int x, int y);

} // namespace d2spike
