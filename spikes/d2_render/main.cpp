#include <irrlicht.h>
#include <Reglas/Tablero.hpp>
#include <Reglas/Jugador.hpp>
#include "scene_builder.hpp"
#include <iostream>
#include <vector>

using namespace irr;

int main() {
    // --- Real game state (same construction as src/Consola/main.cpp) ---
    Reglas::Tablero t;
    std::vector<Reglas::Jugador*> jugadores;
    jugadores.push_back(new Reglas::Jugador(0, nullptr));
    jugadores.push_back(new Reglas::Jugador(1, nullptr));
    t.setJugadores(jugadores);   // p0 -> (4,0), p1 -> (4,8)

    // --- Engine ---
    IrrlichtDevice* dev = createDevice(
        video::EDT_OPENGL, core::dimension2d<u32>(800, 600),
        16, false, false, false, nullptr);
    if (!dev) { std::cerr << "createDevice failed\n"; return 1; }
    video::IVideoDriver* drv   = dev->getVideoDriver();
    scene::ISceneManager* smgr = dev->getSceneManager();

    d2spike::SceneRefs refs = d2spike::buildScene(dev, t);
    if (!refs.piece0 || !refs.piece1) {
        std::cerr << "scene build failed (a piece mesh did not load)\n";
        dev->drop(); return 2;
    }

    // NOTE: this IrrlichtMt fork has no dynamic lights (addLightSceneNode /
    // ILightSceneNode were removed; only setAmbientLight survives). The meshes
    // are lit but coloured via per-material EmissiveColor (scene_builder.cpp),
    // with a near-black ambient so each mesh shows its own flat colour. Depth
    // reads from camera angle + occlusion, not shading.
    smgr->setAmbientLight(video::SColorf(0.0f, 0.0f, 0.0f));

    // Oblique overhead view so the upright pieces stand out against the flat
    // board and the two ends are both visible. Tunable.
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    cam->setPosition(core::vector3df(0, 480, -560));  // lower/back -> oblique; tune visually
    cam->setTarget(core::vector3df(0, 0, 0));

    auto renderN = [&](int n) {
        for (int i = 0; i < n && dev->run(); ++i) {
            drv->beginScene(true, true, video::SColor(255, 40, 40, 70));
            smgr->drawAll();
            drv->endScene();
        }
    };

    // --- BEFORE: initial position ---
    renderN(60);
    video::IImage* before = drv->createScreenShot();
    bool wroteBefore = before &&
        drv->writeImageToFile(before, "build/spikes/d2_render/spike-before.png");
    if (before) before->drop();

    // --- Apply one REAL move: player 0 from (4,0) to (4,1) ---
    t.moverJugador(0, 4, 1);
    std::vector<int> p0 = t.getCelda(0).getPosicion();   // expect {4,1}
    d2spike::placePieceAtCell(refs.piece0, p0.at(0), p0.at(1));

    // --- AFTER: post-move position ---
    renderN(60);
    video::IImage* after = drv->createScreenShot();
    bool wroteAfter = after &&
        drv->writeImageToFile(after, "build/spikes/d2_render/spike-after.png");
    if (after) after->drop();

    dev->drop();

    const bool movedToExpected = (p0.size() == 2 && p0.at(0) == 4 && p0.at(1) == 1);
    if (wroteBefore && wroteAfter && movedToExpected) {
        std::cout << "D2 SPIKE OK\n";
        return 0;
    }
    std::cerr << "D2 SPIKE FAIL"
              << " wroteBefore=" << wroteBefore
              << " wroteAfter="  << wroteAfter
              << " p0=(" << (p0.size()>0?p0[0]:-1) << ","
                         << (p0.size()>1?p0[1]:-1) << ")\n";
    return 4;
}
