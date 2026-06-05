#pragma once
#include <irrlicht.h>
#include <vector>
#include <memory>

namespace Grafico {
using namespace irr;

// Base class for any time-based animation. Advanced by real elapsed dt each
// frame. (Replaces the IrrlichtMt-deleted ISceneNodeAnimator subsystem with a
// general, reusable system — D2.2a.)
class Animacion {
public:
    virtual ~Animacion();
    virtual void update(u32 dtMs) = 0;   // advance by real elapsed time
    virtual bool terminada() const = 0;  // finished?
    virtual bool bloqueante() const { return true; } // gates the next move? (ambient=false)
};

// Owns active animations, ticks them, reaps finished ones.
class Animador {
public:
    void agregar(Animacion* a);   // takes ownership
    void update(u32 dtMs);        // tick all; erase finished
    bool hayBloqueante() const;   // any blocking animation still running?
    bool vacio() const;           // no animations at all
    void limpiar();
private:
    std::vector<std::unique_ptr<Animacion>> activas;
};

} // namespace Grafico
