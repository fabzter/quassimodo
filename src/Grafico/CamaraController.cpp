#include "CamaraController.hpp"

namespace Grafico {

CamaraController::CamaraController(scene::ICameraSceneNode* cam, core::vector3df centro, f32 radio)
    : cam(cam), centro(centro), radio(radio) {
    if (!cam) return;
    // 3/4 view: elevated and pulled back along -Z, looking at the board centre.
    // A flatter (~29deg) elevation than a steep top-down look, so the pawns'
    // vertical jump arc reads clearly while the whole board still frames
    // (tuned via vision — D2.2a M1).
    core::vector3df pos = centro + core::vector3df(0.0f, radio * 1.0f, -radio * 1.8f);
    cam->setPosition(pos);
    cam->setTarget(centro);
}

CamaraController::~CamaraController() = default;

void CamaraController::update(u32 /*dtMs*/) {
    // D2.2a: static framing — nothing to update. Seam for D2.2b interactive controls.
}

} // namespace Grafico
