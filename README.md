___
Juego de Quoridor en el que compiten 2 inteligencias artificiales.

# Quassimodo
Quassimodo es un juego de Quoridor multiplataforma. Está escrito en C++, haciendo uso del Irrlicht Engine y de Boost::Python.

A diferencia de los juegos tradicionales de mesa para computadora, el jugador no interactúa directamente con la pieza que controla, sino que el jugador escribe un agente inteligente (bueno la inteligencia no es un requerimiento ;) en Python, que competirá ya sea con otro agente inteligente escrito por el usuario, o por alguno de los provistos por default en el juego.

Se le proporciona al usuario una API para desarrollar sus agentes, sobre la cual se pueden generar las jugadas que desea realizar el jugador. La API tiene métodos de alto y bajo nivel, que permiten distintos niveles de control sobre las acciones que se desean tomar. Al programar el agente, se deben tomar en cuenta las reglas del juego, pues si un agente las rompe, perderá instantáneamente.
___

## Ejemplo
Un agente inteligente trivial es:

``` 
import Reglas #ésto es totalmente opcional 
import random

#todo agente debe heredar de Reglas.Agente
class AgenteTrivial (Reglas.Agente):

#Este constructor es forzoso!
def __init__(self):
    Reglas.Agente.__init__(self)#inclusive esta linea

def iniciar(self, id):
    self.tab = tablero
    self.id = id
    self.ayudante = ayudante

def siguienteJugada(self):   
    if len(ayudante.getBarrerasPosibles(self.id)) != 0:
        j = random.choice(ayudante.getBarrerasPosibles(self.id))
    else:
        j = random.choice(ayudante.getMovimientosPosibles(self.id))

    return j

def terminar(self):
    self.tab = None
    self.id = None
    self.ayudante = None
```

### Explicación
A pesar de su sencillez este agente muestra todos los básicos para crear uno.

En primer lugar hacemos: import Reglas import random Importar el namespace Reglas por el momento es opcional, pues es expuesto por default al agente sin que éste requiera importarlo, pero también es considerado buena práctica pues no deja implícito este hecho. El namespace Reglas contiene todas las clases necesarias para revisar las reglas, y manipular el tablero del juego de forma segura. (es decir, que el tablero del juego resulte inafectado por las pruebas hechas en él.)

Luego empezamos con la declaración de nuestro agente: class AgenteTrivial (Reglas.Agente): def __init__(self): Reglas.Agente.__init__(self) Aqui presentamos dos partes obligatorias de un agente: 1. Aunque no importa qué nombre se le ponga a nuestra clase de agente, éste debe heredar de la clase Reglas.Agente. 1. Se debe crear un constructor que al menos tenga lo que aqui presentamos. Esto se debe a que se necesita inicializar la parte Reglas.Agente de nuestra clase para "asegurar" que se cumple con la interfaz de dicha clase abstracta.

Luego sobrecargamos el método iniciar: def iniciar(self, id): self.tab = tablero self.id = id self.ayudante = ayudante Este método es llamado antes de comenzar una partida, y puede ser usado por los agentes para inicializar recursos. En este caso, guardamos referencias a los objetos expuestos por default al agente. Estos objetos se exponen desde el principio al agente y están disponibles durante toda la duración de la partida.

Luego sobrecargamos el método siguienteJugada: ``` def siguienteJugada(self): if len(ayudante.getBarrerasPosibles(self.id)) != 0: j = random.choice(ayudante.getBarrerasPosibles(self.id)) else: j = random.choice(ayudante.getMovimientosPosibles(self.id))

    return j
``` Éste método regresa una jugada. Es llamado cada vez que le toca realizar una jugada al agente. En este caso usamos la interfaz de alto nivel, pues los métodos ayudante.getBarrerasPosibles y ayudante.getMovimientosPosibles nos regresan jugadas posibles ya armadas, y no tenemos que armarla a mano. Este agente también revisa que no se intente elegir una jugada de una lista vacía.

El último método por sobrecargar es terminar: def terminar(self): self.tab = None self.id = None self.ayudante = None Este método es llamado una vez que termina la partida (ya sea por violación de reglas o por que hay un ganador). Puede ser usado por el agente para liberar recursos usados. En este caso quitamos las referencias que habíamos guardado para que el recolector de basura sea libre de recogerlas de ser necesario.

Por supuesto esta explicación es bastante resumida, y el agente es bastante trivial. Para una explicación más a detalle de las clases en el namespace Reglas, y los métodos de alto y bajo nivel para la creación de jugadas, favor de revisar el readme del proyecto ó la documentación de doxygen de las clases involucradas.
