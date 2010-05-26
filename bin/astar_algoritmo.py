#!/usr/bin/env python
#-*- coding:utf-8 -*-
"""
    Este archivo contiene el algoritmo A* adaptado para nuestro juego de 
    Quoridor.
"""

import sys
from sys import maxint
import math
from heapq import heappush, heappop
import agente_astar

#constantes para el a*
F, H, NUM, G, POS, OPEN, VALID, PARENT = xrange(8)

#funciones de apoyo al astar:
def get_neighbors(tableroActual):
    """
        Recibe el tablero modificado, y regresa los movimientos posibles 
        (vecinos) del jugador en turno.
    """
    ayudanteTemp = agente_astar.Reglas.AyudanteDeAgente(tableroActual)
    vecinos = []
    
    # para cada movimiento posible para el jugador al que le toca tirar
    for jugada in ayudanteTemp.getMovimientosPosibles(tableroActual.idJugador):
        # creamos un tablero con dicho movimiento realizado
        tab = agente_astar.Reglas.Tablero(tableroActual)
        tab.moverJugador( tableroActual.idJugador, 
            jugada.getPosicion()[0], jugada.getPosicion()[1] )
        
        tab.jugada = jugada # agregamos la jugada que nos llevo a dicho estado
        tab.idJugador = tableroActual.idJugador # ...
        
        vecinos.append(tab)
    
    return vecinos

def is_goal(tableroActual):
    """
        Regresa True si el Jugador al que le toca tirar esta en su meta. False 
        de no ser asi.
    """
    return tableroActual.getCelda(tableroActual.idJugador) in \
            tableroActual.getMetas(tableroActual.idJugador)
            
def manhattan(a, b):
    """
        Regresa la distancia manhattan entre dos puntos. a y b son colecciones 
        de dos enteros. (x, y).
    """
    return abs( (b[0] - a[0]) * 1.2 ) + abs(b[1] - a[1])
    
def costo(tabA, tabB):
    """
        Regresa el costo para el jugador que le toca tirar, de moverse de su 
        posicion en tabA a su posicion en tabB. Ambos son tableros modificados.
    """
    pos_1 = tabA.getCelda(tabA.idJugador).getPosicion()
    pos_2 = tabB.getCelda(tabB.idJugador).getPosicion()
    
    return manhattan(pos_1, pos_2)

def estimate(tableroActual):
    """
        Regresa el costo estimado para el jugador que le toca tirar, de ir de su
        posicion en tableroActual a su meta.
    """
    pos_1 = tableroActual.getCelda(tableroActual.idJugador).getPosicion()
    
    #ok... escogemos la maxima distancia entre la posicion actual del jugador al
    #que le toco tirar y todas sus metas
    return max( manhattan(pos_1, celda_2.getPosicion()) for celda_2 in \
                                tableroActual.getMetas(tableroActual.idJugador))
    

def astar(start_pos, neighbors = get_neighbors, goal = is_goal, start_g = 0, 
            cost = costo, heuristic = estimate, limit=maxint, debug=None):

    """
    Encuentra el camino mas corto (posiblemente) a la meta.

Argumentos:

start_pos - La posicion de inicio.
neighbors(pos) - Una función que regresa todos los vecinos de la posicion dada.
goal(pos) - funcion que regresa True si pos es la meta, False de lo contrario.
start_g - El costo de inicio.
cost(a, b) - Una funcion que regresa el costo de moverse de una posicion a otra.
heuristic(pos) - Una funcion que regresa un estimado del costo total de alcanzar
la meta desde pos.
limit - El numero máximo de posiciones a buscar.
debug(nodes) - Esta funcion se llama con un diccionario de todos los nodos.

Regresa el mejor camino encontrado sin contar la posicion inicial.
"""

    # Crea el nodo de inicio
    nums = iter(xrange(maxint))
    start_h = heuristic(start_pos)
    start = [start_g + start_h, start_h, nums.next(), start_g, start_pos, True,
             True, None]

    # Track all nodes seen so far.
    nodes = {start_pos: start}

    # Mantenemos un heap de los nodos
    heap = [start]

    # Track the best path found so far.
    best = start

    while heap:

        # Sacamos el siguiente nodo del heap
        current = heappop(heap)
        current[OPEN] = False

        # alcanzamos la meta?
        if goal(current[POS]):
            best = current
            break

        # Visitamos a los vecinos del nodo actual
        for neighbor_pos in neighbors(current[POS]):
            neighbor_g = current[G] + cost(current[POS], neighbor_pos)
            neighbor = nodes.get(neighbor_pos)
            if neighbor is None:

                # Limitamos la busqueda.
                if len(nodes) >= limit:
                    continue

                # Encontramos un nuevo nodo
                neighbor_h = heuristic(neighbor_pos)
                neighbor = [neighbor_g + neighbor_h, neighbor_h, nums.next(),
                            neighbor_g, neighbor_pos, True, True, current[POS]]
                nodes[neighbor_pos] = neighbor
                heappush(heap, neighbor)
                if neighbor_h < best[H]:

                    # Nos aproximamos a la meta
                    best = neighbor

            elif neighbor_g < neighbor[G]:

                # Hemos encontrado el mejor camino al vecino.
                if neighbor[OPEN]:

                    # El vecino ya esta abierto. Encontrarlo y actualizarlo
                    # seria una operacion de complejidad linear.
                    # En lugar de eso lo marcamos como invalido y hacemos una 
                    # copia actualizada de él.

                    neighbor[VALID] = False
                    nodes[neighbor_pos] = neighbor = neighbor[:]
                    neighbor[F] = neighbor_g + neighbor[H]
                    neighbor[NUM] = nums.next()
                    neighbor[G] = neighbor_g
                    neighbor[VALID] = True
                    neighbor[PARENT] = current[POS]
                    heappush(heap, neighbor)

                else:

                    # Re-abrir al vecino.
                    neighbor[F] = neighbor_g + neighbor[H]
                    neighbor[G] = neighbor_g
                    neighbor[PARENT] = current[POS]
                    neighbor[OPEN] = True
                    heappush(heap, neighbor)

        # Botamos los nodos invalidos al inicio del heap
        while heap and not heap[0][VALID]:
            heappop(heap)

    if debug is not None:
        # Pasamos el diccionario de nodos al llamador.
        debug(nodes)

    # Regresamos el mejor path como una lista.
    path = []
    current = best
    while current[PARENT] is not None:
        path.append(current[POS])
        current = nodes[current[PARENT]]
    path.reverse()
    return path

