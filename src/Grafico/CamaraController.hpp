#pragma once
#include <irrlicht.h>

namespace Grafico {
using namespace irr;

// Owns the framing of the active camera. D2.2a: a static 3/4 perspective framed
// on the board centre. D2.2b will subclass/extend with orbit/pan/zoom on input
// (the seam: override update() / add onDrag/onScroll).
class CamaraController {
public:
    // centro = board centre, radio = half-extent used to back the camera off.
    CamaraController(scene::ICameraSceneNode* cam, core::vector3df centro, f32 radio);
    virtual ~CamaraController();
    virtual void update(u32 dtMs);   // D2.2a: holds the framed pose (no-op motion)
protected:
    scene::ICameraSceneNode* cam;
    core::vector3df centro;
    f32 radio;
};

} // namespace Grafico
