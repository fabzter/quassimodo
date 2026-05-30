#include "scene_builder.hpp"
#include <Reglas/Tablero.hpp>
#include <Reglas/Celda.hpp>
#include <vector>

using namespace irr;

namespace d2spike {

// World Y of the board's TOP surface after it's recentred on the origin.
// Tablero.obj is ~63 tall (Y in [-0.2, 63.1]); recentring shifts its centre
// (Y≈31.5) to the origin, so the top surface lands at +halfHeight ≈ +31.5.
// Cells (Y in [0,10]) sit ON this plane; pieces are lifted above it. Tunable.
static const float kBoardTopY = 31.5f;

core::vector3df cellToWorld(int x, int y) {
    // Centred grid so cell (4,4) sits over the world origin — pairs with the
    // board being recentred on the origin in buildScene (Task 2 finding:
    // the board's model origin is a far corner). board ~650 deep / 9 ≈ 72.
    // Y = board top surface so a cell/piece placed here rests ON the board,
    // not buried inside it (the board straddles Y=0 after recentring).
    const float spacing = 68.0f;   // tune visually (board half-extent ~325 vs 4*68=272)
    return core::vector3df((x - 4) * spacing, kBoardTopY, (y - 4) * spacing);
}

static scene::IAnimatedMeshSceneNode* addMesh(
        IrrlichtDevice* dev, const char* path, const core::vector3df& pos,
        video::SColor color) {
    scene::ISceneManager* smgr = dev->getSceneManager();
    io::IReadFile* rf = dev->getFileSystem()->createAndOpenFile(path);
    scene::IAnimatedMesh* mesh = rf ? smgr->getMesh(rf) : nullptr;
    if (rf) rf->drop();
    if (!mesh) return nullptr;
    scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
    if (node) {
        // IrrlichtMt removed setMaterialFlag/EMF_* (Task 2 findings) and ALSO
        // removed dynamic lights (no addLightSceneNode/ILightSceneNode; only
        // setAmbientLight survives — Minetest does its own lighting). With no
        // light, lit materials go flat-ambient and every mesh — all white in
        // the .mtl (Kd ~1,1,1) — renders the same gray, indistinguishable.
        // So: disable lighting and force a distinct flat colour per mesh type
        // via EmissiveColor (unlit colour) so the scene reads. Mutated directly
        // via the fork's forEachMaterial helper.
        // With Lighting=false the fixed-function path modulates by vertex
        // colour/texture and ignores Diffuse/Emissive — every mesh stayed white.
        // Instead enable lighting with EmissiveColor set: output = emissive +
        // ambient*ambientColor, and main.cpp sets a near-black ambient, so each
        // mesh shows its own flat EmissiveColor regardless of the white .mtl.
        node->forEachMaterial([color](video::SMaterial& mat) {
            mat.Lighting = true;
            mat.EmissiveColor = color;
            mat.AmbientColor = color;
            mat.DiffuseColor = color;
        });
        node->setPosition(pos);
    }
    return node;
}

void placePieceAtCell(scene::IAnimatedMeshSceneNode* node, int x, int y) {
    if (node) node->setPosition(cellToWorld(x, y) + core::vector3df(0, 10, 0));
}

SceneRefs buildScene(IrrlichtDevice* dev, const Reglas::Tablero& t) {
    SceneRefs refs;

    // Distinct flat colours per mesh type (see addMesh: every .mtl is white,
    // and the fork has no dynamic lights, so colour is the only depth cue).
    const video::SColor kBoard  (255,  90,  70,  45);  // dark wood brown
    const video::SColor kCell   (255, 170, 150, 110);  // light tan tile
    const video::SColor kPiece0 (255, 220,  60,  60);  // red pawn  (player 0)
    const video::SColor kPiece1 (255,  60, 120, 230);  // blue pawn (player 1)
    const video::SColor kBarrier(255,  40, 200,  90);  // green barrier

    // Board — recentre on the world origin (its model origin is a far corner,
    // Task 2 finding) so the centred cell grid lands on it.
    scene::IAnimatedMeshSceneNode* boardNode =
        addMesh(dev, "conf/skin_default/Tablero.obj", core::vector3df(0, 0, 0), kBoard);
    if (boardNode) {
        core::aabbox3df bb = boardNode->getBoundingBox();   // local-space bbox
        boardNode->setPosition(-bb.getCenter());            // board centre -> world origin
    }

    // A Celda node at every grid position (9x9).
    for (int y = 0; y < Reglas::Tablero::size_y; ++y) {
        for (int x = 0; x < Reglas::Tablero::size_x; ++x) {
            addMesh(dev, "conf/skin_default/Celda.obj", cellToWorld(x, y), kCell);
        }
    }

    // Players at their current cells (derived from real Tablero state).
    std::vector<int> p0 = t.getCelda(0).getPosicion(); // {x,y}
    std::vector<int> p1 = t.getCelda(1).getPosicion();
    refs.piece0 = addMesh(dev, "conf/skin_default/Mono.obj",
                          cellToWorld(p0.at(0), p0.at(1)) + core::vector3df(0, 10, 0), kPiece0);
    refs.piece1 = addMesh(dev, "conf/skin_default/MonoBlanco.obj",
                          cellToWorld(p1.at(0), p1.at(1)) + core::vector3df(0, 10, 0), kPiece1);

    // One barrier between cells (4,3) and (4,4) — purely to prove the barrier
    // mesh renders; placed at the midpoint, lifted slightly.
    core::vector3df a = cellToWorld(4, 3);
    core::vector3df b = cellToWorld(4, 4);
    addMesh(dev, "conf/skin_default/Barrera.obj",
            (a + b) * 0.5f + core::vector3df(0, 5, 0), kBarrier);

    return refs;
}

} // namespace d2spike
