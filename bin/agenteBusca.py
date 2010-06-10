#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
from sys import maxint
sys.path.append("../lib")
import random
import math
from heapq import heappush, heappop
from Reglas import astar
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
    idEnemigo = 0 if tab.idJugador == 1 else 1
    res = (CELLS - len(astar(tab, tab.idJugador) ) ) \
            - (0.5)*(CELLS - len(astar(tab, idEnemigo) ) )  # si el path es menor, es mejor calificacion
    return res
    
def minmax(currentTab, currentDepth = 0, maxDepth = 3):
    
    ayudanteCurrent = Reglas.AyudanteDeAgente(currentTab)
    
    #si llegamos al final de la recursion o hay un ganador, evaluamos el tab.
    if currentDepth >= maxDepth or ayudanteCurrent.hayGanador():
        currentTab.val = evaluate(currentTab)
        return
    
    #si no, generamos a sus hijos.
    jugadas = []
    jugadas.extend( ayudanteCurrent.getMovimientosPosibles(currentTab.idJugador) )
    #jugadas.extend( ayudanteCurrent.getBarrerasPosibles(currentTab.idJugador) )
    ayudanteCurrent = None
    currentTab.hijos = []
    
    while len(jugadas) > 0:
        jugada = jugadas.pop( random.randint(0, len(jugadas) - 1 ) )
        tabHijo = Reglas.Tablero(currentTab)
        if jugada.getTipoDeJugada() == Reglas.TipoDeJugada.MOVIMIENTO:
            tabHijo.moverJugador(currentTab.idJugador, jugada.getPosicion() )
        else:
            tabHijo.setBarrera(currentTab.idJugador, Reglas.Barrera(jugada) )
        
        #revisamos que el hijo no sea igual al padre
        if tabHijo == currentTab or tabHijo in currentTab.hijos:
            tabHijo = None
            continue
        
        tabHijo.idJugador = 0 if currentTab.idJugador == 1 else 1
        tabHijo.tipo = not currentTab.tipo
        tabHijo.jugada = jugada
        minmax(tabHijo, currentDepth + 1)
        
        currentTab.hijos.append(tabHijo)
    
    #propagamos hacia arriba los valores de los hijos
    valsHijos = (tab.val for tab in currentTab.hijos)
    currentTab.val = max( valsHijos ) if currentTab.tipo is MAX \
                    else min( valsHijos )
    
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
        
        minmax(tablero)
        for hijo in tablero.hijos:
            if hijo.val == tablero.val:
                return hijo.jugada
    
    def terminar(self):
        print "Jugador {0} ha terminado.".format( self.id )

