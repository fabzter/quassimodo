#include "Animador.hpp"
#include <algorithm>

namespace Grafico {

Animacion::~Animacion() = default;

void Animador::agregar(Animacion* a) {
    if (a) activas.emplace_back(a);
}

void Animador::update(u32 dtMs) {
    for (auto& a : activas) a->update(dtMs);
    // reap finished
    activas.erase(
        std::remove_if(activas.begin(), activas.end(),
            [](const std::unique_ptr<Animacion>& a){ return a->terminada(); }),
        activas.end());
}

bool Animador::hayBloqueante() const {
    for (const auto& a : activas)
        if (a->bloqueante() && !a->terminada()) return true;
    return false;
}

bool Animador::vacio() const { return activas.empty(); }
void Animador::limpiar() { activas.clear(); }

} // namespace Grafico
