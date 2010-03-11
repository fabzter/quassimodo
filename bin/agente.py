#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys; sys.path.append('../lib')

import Reglas

class MiAgente (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, Tablero, id):
        self.tab = Tablero
        self.id = id
        print "Estoy iniciado"
        
    def siguienteJugada(self):
        print self.id, ": Preparo mi jugada"
        jugada = Reglas.Jugada()
        jugada.setPosicion(9, 8)
        jugada.setTipoDeJugada(Reglas.TipoDeJugada.MOVIMIENTO)

        return jugada
    
    def terminar(self):
        print "He terminado", self.id
