#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
sys.path.append("../lib")
import random
import Reglas

class AgenteAStar (Reglas.Agente):
    def __init__(self):
        Reglas.Agente.__init__(self)
    
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
    
    def siguienteJugada(self):
        tablero.idJugador = self.id
        
        #if len(ayudante.getBarrerasPosibles(self.id)) != 0:
        #    j = random.choice(ayudante.getBarrerasPosibles(self.id))
        #else:
        #    j = Reglas.Jugada( Reglas.astar(tablero, self.id)[1] )
        try : j = Reglas.Jugada( Reglas.astar(tablero, self.id)[1] )
        except IndexError: j = random.choice(ayudante.getMovimientosPosibles(self.id) )
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "Jugador {0} ha terminado.".format( self.id )
