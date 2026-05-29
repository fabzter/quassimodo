// Shared opaque-type declarations for the Reglas pybind11 module.
//
// PYBIND11_MAKE_OPAQUE adds a type_caster specialization that disables
// automatic STL conversion for the named type. pybind11's documentation is
// explicit: this specialization MUST be visible in EVERY translation unit
// that references the type across the binding boundary — otherwise different
// TUs see different type_caster specializations and the binding silently
// uses the wrong conversion path (auto-stl vs opaque-class), producing
// hard-to-diagnose runtime crashes.
//
// D1' (the initial pybind11 migration) put MAKE_OPAQUE only in main.cpp.
// The result: AyudanteDeAgente.cpp returning std::vector<Celda*> from
// __astar__ triggered the auto-stl path (because that TU didn't see the
// opaque specialization), which segfaulted on the canonical
// Camina-vs-MiniMax2 / Camina-vs-astar match. This header is the fix:
// include it from every TU that references any of the named containers,
// BEFORE any other pybind11 header.
//
// Every cpp under src/AgenteWrapper/ should include this first.
#pragma once

#include <pybind11/pybind11.h>
#include <vector>

#include <Reglas/Celda.hpp>
#include <Reglas/Jugada.hpp>

PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Celda>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Celda*>);
PYBIND11_MAKE_OPAQUE(std::vector<Reglas::Jugada>);
