# -*- coding: utf-8 -*-
"""
D1' verification fixture — NOT a real agent.

Intentionally raises a RuntimeError from siguienteJugada so the consola
runner's exception-handling path can be exercised in CI / smoke tests.
Expected outcome: consola prints "Error en un Script (Python exception):"
or "Error en un Script:" (depending on whether the wrapper's translation
layer catches it first) and exits with a non-zero status. It MUST NOT
SIGABRT through libc++abi.
"""

import Reglas


class BrokenAgent(Reglas.Agente):
    def __init__(self):
        Reglas.Agente.__init__(self)

    def iniciar(self, id):
        # Don't raise here — let the runtime get all the way to siguienteJugada
        # so we exercise the inner loop catch, not the agent-loading path.
        self.id = id

    def siguienteJugada(self):
        raise RuntimeError(
            "agente_test_broken: intentional failure to exercise the "
            "Python-exception path in consola/main.cpp")

    def terminar(self):
        pass
