#!/usr/bin/env python
#-*- coding:utf-8 -*-
import Reglas

class MiAgente (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, Tablero, id):
        self.tab = Tablero
        self.id = id
        
        if(self.tab.getMetas(self.id)[0].getPosicion()[1] == 8):
            self.direccion = Reglas.Direccion.NORTE
        elif self.tab.getMetas(self.id)[0].getPosicion()[1] == 0:
            self.direccion = Reglas.Direccion.SUR
        else:
            print "WTF"
            self.direccion = Reglas.Direccion.SUR
        
        print "Soy jugador ", self.id, "en ", self.tab.getCelda(self.id), "con direccion: ", self.direccion
        print "Mis metas son:", self.tab.getMetas(self.id)
        for meta in self.tab.getMetas(self.id):
            print meta
        print "Y mi Celda actual está: ", "Libre" if self.tab.getCelda(self.id).estaLibre() else "ocupada"
        
        print "Estoy iniciado"
        
        
    def siguienteJugada(self):
        print self.id, ": Preparo mi jugada"
        
        j = Reglas.Jugada()
        j.setTipoDeJugada(Reglas.TipoDeJugada.MOVIMIENTO)
        
        celdaActual = self.tab.getCelda(self.id)
        
        print "EL hijo de ", celdaActual, "En direccion ", self.direccion, "es", celdaActual.getHijo(self.direccion)
        print "EL hijo de ", celdaActual, "En direccion ", Reglas.Direccion.ESTE, "es", celdaActual.getHijo(Reglas.Direccion.ESTE)
        print "EL hijo de ", celdaActual, "En direccion ", Reglas.Direccion.OESTE, "es", celdaActual.getHijo(Reglas.Direccion.OESTE)
        print "EL hijo de ", celdaActual, "En direccion ", Reglas.Direccion.SUR, "es", celdaActual.getHijo(Reglas.Direccion.SUR)
        if(celdaActual.getHijo(self.direccion).estaLibre()):
            j.setPosicion( celdaActual.getHijo(self.direccion).getPosicion() )
        else:
            j.setPosicion( celdaActual.getHijo(self.direccion).getHijo(self.direccion).getPosicion() )
        
        print self.id, ": Hice mi Jugada"
        return jugada
    
    def terminar(self):
        print "He terminado", self.id
