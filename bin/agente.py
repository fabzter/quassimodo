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
        print "Estoy por enviar mi jugada"
        return None
    
    def terminar(self):
        print "He terminado", self.id
