#pragma once
#include <irrlicht.h>
namespace Reglas { class Tablero; }

namespace d2spike {

// Builds the IrrlichtMt scene from a real Reglas::Tablero. Fully implemented
// in Task 3. Declared here so main.cpp and CMake compile from Task 2 onward.
struct SceneRefs {
    irr::scene::IAnimatedMeshSceneNode* piece0 = nullptr; // player 0's piece node
    irr::scene::IAnimatedMeshSceneNode* piece1 = nullptr; // player 1's piece node
};

// Maps grid cell (x,y) to a world position. Tunable in Task 3.
irr::core::vector3df cellToWorld(int x, int y);

} // namespace d2spike
