#!/usr/bin/env python
#-*- coding:utf-8 -*-
import Reglas

class AgenteBarreras (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.idEnemigo = 1 if self.id == 0 else 0
        
        if(self.tab.getMetas(self.id)[0].getPosicion()[1] == 8):
            self.direccion = Reglas.Direccion.NORTE
        elif self.tab.getMetas(self.id)[0].getPosicion()[1] == 0:
            self.direccion = Reglas.Direccion.SUR
        else:
            print "WTF"
            self.direccion = Reglas.Direccion.SUR
            
        self.direccionEnemigo = Reglas.Direccion.NORTE \
        if self.direccion == Reglas.Direccion.SUR else Reglas.Direccion.SUR
        
        print "Soy jugador ", self.id
        
        print "Estoy iniciado"
        
        
    def siguienteJugada(self):
        print self.id, ": Preparo mi jugada"
        
        j = Reglas.Jugada()
        j.setTipoDeJugada(Reglas.TipoDeJugada.BARRERA)
        
        celdaActual = self.tab.getCelda(self.id)
        celdaEnemigo = self.tab.getCelda(self.idEnemigo)
        
        j.setPosicion( celdaEnemigo.getPosicion()[0], celdaEnemigo.getHijo(self.direccionEnemigo).getPosicion()[1] )
        j.setDireccion( Reglas.Direccion.ESTE )
        
        print self.id, ": Hice mi Jugada"
        return j
    
    def terminar(self):
        print "He terminado", self.id
