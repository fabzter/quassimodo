#include <irrlicht.h>
#include <Opciones/ManejadorOpciones.hpp>
#include <Grafico/Skin.hpp>
#include "PartidaGrafica.hpp"
#include <iostream>
#include <string>
#include <list>
#include <Grafico/CamaraController.hpp>
#include <cstdlib>

using namespace irr;

// IrrlichtMt 1.9.0mt15 has no dynamic lights; with Lighting=true + ambient-only
// the textured meshes render as flat black silhouettes. The board/cells/pawns DO
// carry textures (Tablero/Celda/Pieza call setTexture). Forcing every material
// unlit makes the GL2 Solid path emit texture*vertexcolor directly, so the
// textures become visible. Recurse over the whole built scene graph.
static void makeUnlit(scene::ISceneNode* node) {
    if (!node) return;
    node->forEachMaterial([](video::SMaterial& m) { m.Lighting = false; });
    const std::list<scene::ISceneNode*>& kids = node->getChildren();
    for (std::list<scene::ISceneNode*>::const_iterator it = kids.begin();
         it != kids.end(); ++it) {
        makeUnlit(*it);
    }
}

int main(int argc, char* argv[]) {
    std::string agent1 = (argc > 1) ? argv[1] : "bin/agenteCamina.py";
    std::string agent2 = (argc > 2) ? argv[2] : "bin/agenteCamina2.py";

    // ManejadorOpciones expects agent paths via "-a a1 a2"; any bare positional
    // argument trips its unrecognized-positional guard and forces help+exit(0).
    // The driver consumes the agent paths from argv itself (above), so feed the
    // options manager only the program name — it still loads conf/opciones.conf
    // (the model/texture/font paths Skin needs) from defaults.
    char* optionsArgv[] = { argv[0], nullptr };
    Opciones::ManejadorOpciones opciones(1, optionsArgv);

    IrrlichtDevice* device = createDevice(
        video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
        16, false, false, false, nullptr);
    if (!device) { std::cerr << "createDevice failed\n"; return 1; }
    device->setWindowCaption(L"Quassimodo");
    video::IVideoDriver*  drv  = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* env  = device->getGUIEnvironment();

    smgr->setAmbientLight(video::SColorf(1.0f, 1.0f, 1.0f));  // full ambient (no dynamic lights)

    Grafico::Skin* skin = new Grafico::Skin(smgr, env, device->getFileSystem(), opciones);
    PartidaGrafica* partida = new PartidaGrafica(smgr, skin, env, /*velAnim*/ 1);

    try {
        partida->SetJugadores(agent1, agent2);
        partida->iniciarPartida();
    } catch (std::exception& e) {
        std::cerr << "Error iniciando la partida: " << e.what() << "\n";
        device->drop(); return 2;
    }

    makeUnlit(smgr->getRootSceneNode());

    // Eager: run the whole match logically up front, recording the event stream.
    // (A crashing agent — e.g. the Minimax SIGSEGV — would throw here, before any
    // animation; the clean Camina pair completes. Catch so we fail gracefully.)
    try {
        partida->produceAll();
    } catch (std::exception& e) {
        std::cerr << "Error produciendo la partida: " << e.what() << "\n";
        device->drop(); return 3;
    }

    // Static 3/4 framed camera behind the D2.2b seam.
    core::vector3df centro = partida->getCentro();
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    Grafico::CamaraController camara(cam, centro, /*radio*/ 320.0f);  // ~board half-extent; tune

    // Optional screenshots (env-guarded), captured at wall-clock offsets chosen to
    // land MID-MOVE (each move animates over ~350ms), so a pawn is caught airborne
    // regardless of frame rate — the proof that moves animate rather than teleport.
    const bool shots = std::getenv("QM_D22_SHOT") != nullptr;
    const u32 t0 = device->getTimer()->getTime();
    u32 prev = t0;
    int shotIdx = 0;
    const u32 shotTimes[] = {175, 360, 530, 900};
    const char* shotPaths[] = {"build/d22-f1.png","build/d22-f2.png","build/d22-f3.png","build/d22-f4.png"};
    bool announced = false;
    auto shot = [&](const char* p){ video::IImage* i = drv->createScreenShot(); if (i) { drv->writeImageToFile(i, p); i->drop(); } };

    while (device->run()) {
        u32 now = device->getTimer()->getTime();
        u32 dt  = now - prev; prev = now;

        partida->update(dt);    // replay: start the next event when idle + tick animations by real dt
        camara.update(dt);

        drv->beginScene(true, true, video::SColor(255, 40, 40, 70));
        smgr->drawAll();
        env->drawAll();
        drv->endScene();

        if (shots && shotIdx < 4 && (now - t0) >= shotTimes[shotIdx]) {
            shot(shotPaths[shotIdx]); ++shotIdx;
        }
        if (partida->terminadoVisual() && !announced) {
            std::cout << "Hay un ganador! Jugador " << partida->getJugadorGanador() << std::endl;
            announced = true;
            if (shots) shot("build/d22-win.png");
        }
    }

    delete partida;
    delete skin;
    device->drop();
    return 0;
}
