#!/usr/bin/env python
#-*- coding:utf-8 -*-
import Reglas
import random

class AgenteCamina2 (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
        
    def siguienteJugada(self):
        print self.id, ": Preparo mi jugada"
        
        j = random.choice(ayudante.getMovimientosPosibles(self.id))
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "He terminado", self.id
