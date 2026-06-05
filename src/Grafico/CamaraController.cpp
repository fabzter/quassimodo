#include "CamaraController.hpp"

namespace Grafico {

CamaraController::CamaraController(scene::ICameraSceneNode* cam, core::vector3df centro, f32 radio)
    : cam(cam), centro(centro), radio(radio) {
    if (!cam) return;
    // 3/4 view: elevated and pulled back along -Z, looking at the board centre.
    core::vector3df pos = centro + core::vector3df(0.0f, radio * 1.4f, -radio * 1.6f);
    cam->setPosition(pos);
    cam->setTarget(centro);
}

CamaraController::~CamaraController() = default;

void CamaraController::update(u32 /*dtMs*/) {
    // D2.2a: static framing — nothing to update. Seam for D2.2b interactive controls.
}

} // namespace Grafico
