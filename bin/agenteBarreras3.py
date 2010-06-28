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
        self.num_jugada = 0
        print "Estoy iniciado"
        
        
    def siguienteJugada(self):
        j = Reglas.Jugada()
        j.setTipoDeJugada(Reglas.TipoDeJugada.BARRERA)
        
        if self.num_jugada == 0: 
            j.setPosicion( (1, 3) )
            j.setDireccion( Reglas.Direccion.NORTE )
        elif self.num_jugada == 1: 
            j.setPosicion( (0, 5) )
            j.setDireccion( Reglas.Direccion.ESTE )
        elif self.num_jugada == 2: 
            j.setPosicion( (8, 3) )
            j.setDireccion( Reglas.Direccion.NORTE )
        num_jugada += 1
        
        if self.num_jugada>3:
            if len(ayudante.getBarrerasPosibles(self.id)) != 0:
                j = random.choice(ayudante.getBarrerasPosibles(self.id))
            else:
                j = random.choice(ayudante.getMovimientosPosibles(self.id))
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "He terminado", self.id
