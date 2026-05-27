#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
sys.path.append("../lib")
import Reglas
import random

class AgenteBarreras (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
        
        print("Estoy iniciado")


    def siguienteJugada(self):
        if len(ayudante.getBarrerasPosibles(self.id)) != 0:
            j = random.choice(ayudante.getBarrerasPosibles(self.id))
        else:
            j = random.choice(ayudante.getMovimientosPosibles(self.id))

        print(self.id, ": Hice mi Jugada")
        return j

    def terminar(self):
        print("He terminado", self.id)
