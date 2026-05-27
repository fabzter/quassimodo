#include <irrlicht.h>
#include <iostream>
using namespace irr;

int main() {
    IrrlichtDevice* dev = createDevice(
        video::EDT_OPENGL, core::dimension2d<u32>(640, 480),
        16, false, false, false, nullptr);
    if (!dev) { std::cerr << "createDevice failed\n"; return 1; }

    video::IVideoDriver* drv = dev->getVideoDriver();
    int frames = 0;
    while (dev->run() && frames < 120) {   // ~2s then exit (so it terminates)
        drv->beginScene(true, true, video::SColor(255, 30, 30, 60));
        drv->endScene();
        ++frames;
    }
    dev->drop();
    std::cout << "ENGINE WINDOW OK (" << frames << " frames)\n";
    return 0;
}
