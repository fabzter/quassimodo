#include "Animaciones.hpp"
#include <algorithm>

namespace Grafico {

static f32 clamp01(f32 t) { return t < 0 ? 0 : (t > 1 ? 1 : t); }
// smoothstep ease in/out
static f32 ease(f32 t) { t = clamp01(t); return t * t * (3.0f - 2.0f * t); }

core::vector3df saltoPos(const core::vector3df& desde, const core::vector3df& hasta,
                         f32 t, f32 height) {
    t = clamp01(t);
    f32 e = ease(t);
    core::vector3df p = desde + (hasta - desde) * e;   // eased horizontal lerp
    p.Y += height * 4.0f * t * (1.0f - t);             // parabolic hop (peak at t=0.5)
    return p;
}

SaltoAnim::SaltoAnim(Pieza* pieza, core::vector3df desde, core::vector3df hasta,
                     u32 durMs, f32 height)
    : pieza(pieza), desde(desde), hasta(hasta),
      durMs(durMs == 0 ? 1 : durMs), elapsed(0), height(height) {}

void SaltoAnim::update(u32 dtMs) {
    elapsed += dtMs;
    f32 t = (f32)elapsed / (f32)durMs;
    if (t >= 1.0f) { pieza->setPosicionSuave(hasta); return; }
    pieza->setPosicionSuave(saltoPos(desde, hasta, t, height));
}
bool SaltoAnim::terminada() const { return elapsed >= durMs; }

DeslizarAnim::DeslizarAnim(Pieza* pieza, core::vector3df desde, core::vector3df hasta, u32 durMs)
    : pieza(pieza), desde(desde), hasta(hasta),
      durMs(durMs == 0 ? 1 : durMs), elapsed(0), shown(false) {}

void DeslizarAnim::update(u32 dtMs) {
    if (!shown) { if (pieza->getNodo()) pieza->getNodo()->setVisible(true); shown = true; }
    elapsed += dtMs;
    f32 t = (f32)elapsed / (f32)durMs;
    if (t >= 1.0f) { pieza->setPosicionSuave(hasta); return; }
    f32 e = ease(t);
    pieza->setPosicionSuave(desde + (hasta - desde) * e);
}
bool DeslizarAnim::terminada() const { return elapsed >= durMs; }

} // namespace Grafico
