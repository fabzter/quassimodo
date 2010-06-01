#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
sys.path.append("../lib")
import random
import Reglas
import astar_algoritmo

class AgenteAStar (Reglas.Agente):
    def __init__(self):
        Reglas.Agente.__init__(self)
    
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
    
    def siguienteJugada(self):
        tablero.idJugador = self.id
        
        if len(ayudante.getBarrerasPosibles(self.id)) != 0:
            j = random.choice(ayudante.getBarrerasPosibles(self.id))
        else:
            #j = astar_algoritmo.astar(self.tab).jugada
            j = ayudante.astar(self.id, 200).jugada
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "Jugador {0} ha terminado.".format( self.id )
