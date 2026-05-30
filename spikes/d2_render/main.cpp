#include <irrlicht.h>
#include "scene_builder.hpp"
#include <iostream>

using namespace irr;

int main() {
    IrrlichtDevice* dev = createDevice(
        video::EDT_OPENGL, core::dimension2d<u32>(800, 600),
        16, false, false, false, nullptr);
    if (!dev) { std::cerr << "createDevice failed\n"; return 1; }

    video::IVideoDriver* drv  = dev->getVideoDriver();
    scene::ISceneManager* smgr = dev->getSceneManager();

    // NOTE: the fork's getMesh takes io::IReadFile* (no string overload).
    io::IReadFile* rf = dev->getFileSystem()->createAndOpenFile("conf/skin_default/Tablero.obj");
    scene::IAnimatedMesh* board = rf ? smgr->getMesh(rf) : nullptr;
    if (rf) rf->drop();
    if (!board) { std::cerr << "board mesh load failed\n"; dev->drop(); return 2; }
    scene::IAnimatedMeshSceneNode* boardNode = smgr->addAnimatedMeshSceneNode(board);
    // IrrlichtMt 1.9.0mt15 dropped setMaterialFlag/EMF_LIGHTING (fixed-function
    // material flags). Set the SMaterial::Lighting bool directly on every material
    // via the fork's forEachMaterial helper.
    boardNode->forEachMaterial([](video::SMaterial& mat) { mat.Lighting = false; });

    // Frame the camera from the board mesh's actual bounding box so the whole
    // board is visible regardless of where its model origin sits.
    const core::aabbox3df bb = board->getBoundingBox();
    const core::vector3df c = bb.getCenter();
    const core::vector3df ext = bb.getExtent();
    const float reach = core::max_(ext.X, ext.Z);
    std::cerr << "board bbox center=(" << c.X << "," << c.Y << "," << c.Z
              << ") extent=(" << ext.X << "," << ext.Y << "," << ext.Z << ")\n";

    // Overhead, slightly tilted view that frames the whole board.
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    cam->setPosition(core::vector3df(c.X, c.Y + reach * 1.4f, c.Z - reach * 0.8f));
    cam->setTarget(c);

    int frames = 0;
    while (dev->run() && frames < 60) {
        drv->beginScene(true, true, video::SColor(255, 40, 40, 70));
        smgr->drawAll();
        drv->endScene();
        ++frames;
    }

    video::IImage* img = drv->createScreenShot();
    bool wrote = img && drv->writeImageToFile(img, "build/spikes/d2_render/board.png");
    if (img) img->drop();
    dev->drop();

    if (!wrote) { std::cerr << "screenshot write failed\n"; return 3; }
    std::cout << "D2 SCAFFOLD OK\n";
    return 0;
}
