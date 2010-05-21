#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
import random
import Reglas

MIN = 0
MAX = 1

sys.setrecursionlimit(10000)

def busqueda_recursiva(tableroActual):
    idEnemigo = 1 if tableroActual.idJugador == 0 else 0
    ayudanteTemp = Reglas.AyudanteDeAgente(tableroActual)
    #ponemos en una sola lista todas las jugadas
    jugadas = []
    jugadas.extend(ayudanteTemp.getMovimientosPosibles(tableroActual.idJugador))
    jugadas.extend( ayudanteTemp.getBarrerasPosibles(tableroActual.idJugador))
    
    #mientras no hay ganador
    while ( tableroActual.getCelda(0) not in tableroActual.getMetas(0) ) and \
           ( tableroActual.getCelda(1) not in tableroActual.getMetas(1) ) :
        #mientras haya hijos...
        while len(jugadas) > 0:
            #creamos un hijo
            t = Reglas.Tablero(tableroActual)
            j = jugadas.pop( random.randint(0, len(jugadas) -1 ) )
            
            if j.getTipoDeJugada() == Reglas.TipoDeJugada.MOVIMIENTO:
                t.moverJugador( tableroActual.idJugador, 
                j.getPosicion()[0], j.getPosicion()[1] )
            else:
                b = Reglas.Barrera(j)
                try: t.setBarrera(tableroActual.idJugador, b)
                except Reglas.JugadorSinBarreras:
                    print "Jugador:", tableroActual.idJugador
                    raise Exception, "Jugador {0} sin barreras".format(tableroActual.idJugador)
            print "Tablero de generacion"
            tableroActual.print_tab()
                
            t.idJugador = idEnemigo
            t.tipo = MIN if tableroActual.tipo == MAX else MAX
            busqueda_recursiva(t)
    print "Tablero ganador"
    tableroActual.print_tab()

class AgenteInteligente (Reglas.Agente):

    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
        
    def siguienteJugada(self):
        tablero.tipo = MAX
        tablero.idJugador = self.id
        busqueda_recursiva(tablero)
    
    def terminar(self):
        print "He terminado", self.id

