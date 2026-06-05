#pragma once
#include "Animador.hpp"
#include "Pieza.hpp"

namespace Grafico {
using namespace irr;

// Pure jump-arc position: linear in XZ (eased), parabolic in Y.
// t in [0,1]; height = peak rise above the straight line.
core::vector3df saltoPos(const core::vector3df& desde, const core::vector3df& hasta,
                         f32 t, f32 height);

// Pawn jump: moves a Pieza desde->hasta over durMs with a parabolic hop.
class SaltoAnim : public Animacion {
public:
    SaltoAnim(Pieza* pieza, core::vector3df desde, core::vector3df hasta,
              u32 durMs, f32 height);
    void update(u32 dtMs) override;
    bool terminada() const override;
private:
    Pieza* pieza; core::vector3df desde, hasta; u32 durMs, elapsed; f32 height;
};

// Barrier slide-in: linear (eased) lerp desde->hasta; makes the node visible on first tick.
class DeslizarAnim : public Animacion {
public:
    DeslizarAnim(Pieza* pieza, core::vector3df desde, core::vector3df hasta, u32 durMs);
    void update(u32 dtMs) override;
    bool terminada() const override;
private:
    Pieza* pieza; core::vector3df desde, hasta; u32 durMs, elapsed; bool shown;
};

} // namespace Grafico
