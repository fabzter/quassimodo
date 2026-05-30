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

    int frames = 0;
    while (dev->run() && frames < 60) {
        drv->beginScene(true, true, video::SColor(255, 40, 40, 70));
        smgr->drawAll();
        drv->endScene();
        ++frames;
    }

    video::IImage* img = drv->createScreenShot();
    bool wrote = img && drv->writeImageToFile(img, "build/spikes/d2_render/spike-before.png");
    if (img) img->drop();
    dev->drop();

    if (!wrote) { std::cerr << "screenshot write failed\n"; return 3; }
    std::cout << "D2 SCENE OK\n";
    return 0;
}
