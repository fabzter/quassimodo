#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
from sys import maxint
sys.path.append("../lib")
import random
import math
from heapq import heappush, heappop
import Reglas
from astar_algoritmo import astar

MIN = 0
MAX = 1

sys.setrecursionlimit(20000)

def busqueda_recursiva(tableroActual):
    """
        Funcion que realiza la busqueda recursiva sobre el tablero de juego. 
        Recibe un tablero de Juego iniciado, con unos miembros agregados:
        idJugador: el id del Jugador al que le toca tirar
        tableroActual.idCaller: el id del Jugador que inicio al principio la 
        busqueda.
        tableroActual.tipo: si el tableroActual es MIN o MAX.
    """
    
    idEnemigo = 1 if tableroActual.idJugador == 0 else 0
    ayudanteTemp = Reglas.AyudanteDeAgente(tableroActual)
    
    #ponemos en una sola lista todas las jugadas
    jugadas = []
    jugadas.extend(ayudanteTemp.getMovimientosPosibles(tableroActual.idJugador))
    jugadas.extend( ayudanteTemp.getBarrerasPosibles(tableroActual.idJugador))
    
    #creamos la lista que contendra a los hijos
    tableroActual.hijos = []
    
    #mientras no hay ganador, y haya hijos.
    while ( tableroActual.getCelda(0) not in tableroActual.getMetas(0) ) and \
           ( tableroActual.getCelda(1) not in tableroActual.getMetas(1) ) and \
           len(jugadas) > 0:

        #creamos un hijo
        t = Reglas.Tablero(tableroActual)
        j = jugadas.pop( random.randint(0, len(jugadas) -1 ) )
        
        #si el hijo es movimiento
        if j.getTipoDeJugada() == Reglas.TipoDeJugada.MOVIMIENTO:
            t.moverJugador( tableroActual.idJugador, 
            j.getPosicion()[0], j.getPosicion()[1] )
        #si es de barrera
        else:
            b = Reglas.Barrera(j)
            try: t.setBarrera(tableroActual.idJugador, b)
            except Reglas.JugadorSinBarreras:
                print "Jugador:", tableroActual.idJugador
                raise Exception, "Jugador {0} sin barreras".format(tableroActual.idJugador)
        
        #le ponemos el id del jugador al que le toca tirar sobre ese hijo
        t.idJugador = idEnemigo
        #le ponemos si es un tablero tipo min o tipo max
        t.tipo = MIN if tableroActual.tipo == MAX else MAX
        #le ponemos la jugada que nos llevo a ese tablero
        t.jugada = j
        #ponemos al jugador que hizo la llamada inicial
        t.idCaller = tableroActual.idCaller
        
        #agregamos el hijo a la lista de hijos del tablero actual
        tableroActual.hijos.append(t)
        
        #si t es ganado para idCaller, lo regresamos, sino, recurrimos
        if t.getCelda(t.idCaller) in t.getMetas(t.idCaller):
            return t
        else:
            try: busqueda_recursiva(t)
            except RuntimeError as error:
                if error.message.find("recursion") != -1:
                    return t
                raise error

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
        tablero.idCaller = tablero.idJugador = self.id
        
        return busqueda_recursiva(tablero)
    
    def terminar(self):
        print "Jugador {0} ha terminado.".format( self.id )

