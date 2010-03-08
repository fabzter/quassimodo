#!/usr/bin/env python
from Reglas import *

class AgenteHijo(Agente):
    def __init__(self):
        print "Construido!"
        
    def siguienteJugada(self):
        return "siguiente jugada puf!"
        
    def iniciar(self, t, id):
        print "tablero recibido"
        print t.size_x, t.size_y, t.tam_barrera
        try:
            t.getCelda(0)
        except Excepcion , e:
            print e
        print "Hay ", len(t.getBarrerasColocadas()), "barreras colocadas."
        
    def terminar(self):
        print "terminado!"

if __name__ == "__main__":
    a = AgenteHijo()
    
    a.iniciar(Tablero(), 0)
    
    a.terminar()
    
    pieza1 = Pieza()
    v = pieza1.getPosicion()
    for p in v:
        print p
        
    pieza2 = Pieza()
    v = pieza2.getPosicion()
    for p in v:
        print p
    
    print pieza1 == pieza1, pieza2 == pieza1
    
    celda1 = Celda()
    celda2 = Celda()
    
    print celda1 == celda2
    
    print celda1
    
    #prueba de enums?
    print TipoDeJugada.MOVIMIENTO, Direccion.NORTE
    
    #pruebas de familiaridad
    print issubclass(Celda, Pieza), isinstance(celda1, Pieza)
    
    #pruebas de Barrera
    b1 = Barrera()
    b2 = Barrera()
    b3 = Barrera()
    b4 = Barrera()
    
    try:
        b1.colocar( (4, 6, 7), Direccion.NORTE )
    except ParametrosMalos, e:
        print e
        b1.colocar( (4, 6), Direccion.NORTE )
    
    for i in b1.getPosicion():
        print i
    b2.colocar( (0, 0), Direccion.NORTE )
    b3.colocar( (4, 6), Direccion.ESTE )
    b4.colocar( (4, 6), Direccion.NORTE )
    
    print b1 == b2, b1 == b3, b1 == b4, b1 is b4
