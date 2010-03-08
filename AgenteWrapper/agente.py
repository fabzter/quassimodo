import Reglas

class MiAgente (Reglas.Agente):

    def __init__(self):
        pass
        
    def iniciar(self, Tablero, id):
        self.tab = Tablero
        self.id = id
        print "Estoy iniciado"
        
    def siguienteJugada(self):
        print "Estoy por enviar mi jugada"
        return None
    
    def terminar(self):
        print "He terminado"
        self.tab = None
        raise Exception

def get():
    return MiAgente()
