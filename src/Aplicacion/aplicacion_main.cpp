#include <irrlicht.h>
#include <Opciones/ManejadorOpciones.hpp>
#include <Grafico/Skin.hpp>
#include "PartidaGrafica.hpp"
#include <iostream>
#include <string>
#include <list>

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

    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    core::vector3df centro = partida->getCentro();
    cam->setTarget(centro);
    cam->setPosition(centro + core::vector3df(0, 1000, -750));  // tune via the vision check

    // Optional screenshots, guarded by env var, for visual verification: a few
    // frames across the match (so movement is observable) + the winning state.
    const bool shots = std::getenv("QM_D21_SHOT") != nullptr;
    long frame = 0;
    bool wonShot = false;
    bool announced = false;
    auto shot = [&](const char* path) {
        video::IImage* img = drv->createScreenShot();
        if (img) { drv->writeImageToFile(img, path); img->drop(); }
    };
    while (device->run()) {
        if (partida->estaEnCurso() && partida->animacionesEnd()) {
            try { partida->siguienteJugada(); }
            catch (std::exception& e) { std::cerr << "Error en jugada: " << e.what() << "\n"; break; }
        }
        drv->beginScene(true, true, video::SColor(255, 40, 40, 70));
        smgr->drawAll();
        env->drawAll();
        drv->endScene();

        if (shots) {
            if (frame == 0)      shot("build/d21-start.png");
            else if (frame == 4) shot("build/d21-mid1.png");
            else if (frame == 9) shot("build/d21-mid2.png");
        }
        if (partida->hayGanador() && !announced) {
            std::cout << "Hay un ganador! Jugador " << partida->getJugadorGanador() << std::endl;
            announced = true;
            if (shots && !wonShot) { shot("build/d21-win.png"); wonShot = true; }
        }
        ++frame;
    }

    delete partida;
    delete skin;
    device->drop();
    return 0;
}
