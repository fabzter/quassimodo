#include "scene_builder.hpp"

namespace d2spike {

irr::core::vector3df cellToWorld(int x, int y) {
    const float spacing = 55.0f;   // ~cell width (Task 1: Celda extent ~50); tune visually
    return irr::core::vector3df(x * spacing, 0.0f, y * spacing);
}

} // namespace d2spike
