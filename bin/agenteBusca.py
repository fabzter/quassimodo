#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
from sys import maxint
sys.path.append("../lib")
import random
import math
from heapq import heappush, heappop
from Reglas import astar, TipoDeJugada, minimax
import Reglas

MIN = False
MAX = True

CELLS = 81

def printInfo(tab):
    print "Soy un tablero esperando tiro por el Jugador {0}, mi valor es {1}.".format(tab.idJugador, tab.val)
    print "Soy de tipo", "MAX" if tab.tipo is MAX else "MIN"
    print "Este soy yo\n", tab
    try:
        print "Mis hijos son:"
        for hijo in tab.hijos: printInfo(hijo)
    except AttributeError: pass

def evaluate(tab):
    res = (CELLS - len(astar(tab, tab.idJugador) ) ) \
            - (CELLS - len(astar(tab, tab.idEnemigo) ) )  # si el path es menor, es mejor calificacion
    return res
    
def minmax(currentTab, currentDepth = 0, maxDepth = 4, 
            tipoJug = TipoDeJugada.MOVIMIENTO):
    
    ayudanteCurrent = Reglas.AyudanteDeAgente(currentTab)
    currentTab.idEnemigo = 0 if currentTab.idJugador == 1 else 1
    
    #si llegamos al final de la recursion o hay un ganador, evaluamos el tab.
    if currentDepth >= maxDepth or ayudanteCurrent.hayGanador():
        currentTab.val = evaluate(currentTab)
        return
    
    #si no, generamos a sus hijos.
    jugadas = []
    if tipoJug == TipoDeJugada.MOVIMIENTO:
        jugadas.extend( ayudanteCurrent.getMovimientosPosibles(currentTab.idJugador) )
    else:
        posEnemigo = tablero.getCelda(currentTab.idEnemigo).getPosicion()
        jugadas.extend( ( hijo for hijo in ayudanteCurrent.getBarrerasPosibles(currentTab.idJugador)\
                            if hijo.getPosicion()[1] in \
                            xrange(posEnemigo[1] -2, posEnemigo[1] +2 ) ) )
    ayudanteCurrent = None #no necesitamos mas al ayudante
    
    currentTab.hijos = []
    while len(jugadas) > 0:
        jugada = jugadas.pop( random.randint(0, len(jugadas) - 1 ) )
        tabHijo = Reglas.Tablero(currentTab)
        if jugada.getTipoDeJugada() == Reglas.TipoDeJugada.MOVIMIENTO:
            tabHijo.moverJugador(currentTab.idJugador, jugada.getPosicion() )
        else:
            tabHijo.setBarrera(currentTab.idJugador, Reglas.Barrera(jugada) )
        
        #revisamos que el hijo no sea igual al padre, o a sus hermanos :/
        if tabHijo == currentTab or tabHijo in currentTab.hijos:
            tabHijo = None
            continue
        
        tabHijo.idJugador = currentTab.idEnemigo
        tabHijo.tipo = not currentTab.tipo
        tabHijo.jugada = jugada
        minmax(tabHijo, currentDepth + 1, 4, TipoDeJugada.MOVIMIENTO)
        
        currentTab.hijos.append(tabHijo)
        print "Aun quedan {0} jugadas por revisar.".format(len(jugadas))
    
    #propagamos hacia arriba los valores de los hijos
    valsHijos = (tab.val for tab in currentTab.hijos)
    if currentTab.tipo is MIN:
        currentTab.val = min( valsHijos )
    else:
        currentTab.val = max( valsHijos )
        
    #printInfo(currentTab)
    
    #limpiamos?
    if currentDepth > 1:
        del currentTab.hijos[:]

class AgenteInteligente (Reglas.Agente):
    def __init__(self):
        Reglas.Agente.__init__(self)
        
    def iniciar(self, id):
        self.tab = tablero
        self.id = id
        self.ayudante = ayudante
        
    def siguienteJugada(self):
        print "Jugador {0} prepara jugada.".format(self.id)
        tablero.tipo = MAX
        tablero.idJugador = self.id
        
        #if len(ayudante.getBarrerasPosibles(self.id)) != 0:
        #    return minimax(tablero, self.id, 0, 1, TipoDeJugada.BARRERA)
        #else:
        #    return minimax(tablero, self.id, 0, 2, TipoDeJugada.MOVIMIENTO)
        
        idEnemigo = 0 if self.id == 1 else 0
        
        path_yo = astar(self.tab, self.id)
        path_enemigo = (self.tab, idEnemigo)
        
        if len(path_enemigo) < len(path_yo) and \
                                len(ayudante.getBarrerasPosibles(self.id)) != 0:
            return minimax(self.tab, self.id, 0, 4, TipoDeJugada.BARRERA)
        else:
            return Reglas.Jugada( Reglas.astar(tablero, self.id)[1] )            
        

    
    def terminar(self):
        print "Jugador {0} ha terminado.".format( self.id )

