#!/usr/bin/env python
#-*- coding:utf-8 -*-
import Reglas

class AgenteCamina (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        
        if(self.tab.getMetas(self.id)[0].getPosicion()[1] == 8):
            self.direccion = Reglas.Direccion.NORTE
        elif self.tab.getMetas(self.id)[0].getPosicion()[1] == 0:
            self.direccion = Reglas.Direccion.SUR
        else:
            print "WTF"
            self.direccion = Reglas.Direccion.SUR
        
        print "Soy jugador ", self.id
        
        print "Estoy iniciado"
        
        
    def siguienteJugada(self):
        print self.id, ": Preparo mi jugada"
        
        j = Reglas.Jugada()
        j.setTipoDeJugada(Reglas.TipoDeJugada.MOVIMIENTO)
        
        celdaActual = self.tab.getCelda(self.id)


        if(celdaActual.getHijo(self.direccion).estaLibre()):
            j.setPosicion( celdaActual.getHijo(self.direccion).getPosicion() )
        else:
            j.setPosicion( celdaActual.getHijo(self.direccion).getHijo(self.direccion).getPosicion() )
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "He terminado", self.id
