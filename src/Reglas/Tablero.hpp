/**
 */
#ifndef _TABLERO_HPP
#define	_TABLERO_HPP
#include <vector>
#include <list>
#include <algorithm>
#include <stdexcept>
#include "Excepciones.hpp"
#include "Celda.hpp"
#include "Jugador.hpp"
#include "Barrera.hpp"
#include "Grafo.hpp"

namespace Reglas{
class Grafo;
class Jugador;
class AyudanteDeAgente;
/**
 * La clase Tablero es un conjunto de Celdas. Esta clase nos sirve como interface
 * para poder conocer el estado de las Celdas, y de sus lados, asi como para
 * modificar cada Celda (o insertar una nueva!). Nos indica dónde está colocado
 * cada Jugador.
 */
class Tablero {
friend class Grafo;
friend class Juez;
friend class AyudanteDeAgente;
public:
    /**
     *El constructor por default. Nos entrega un Tablero con una lista vacia de
     * barreras_colocadas vacia, y sin ningún Jugador colocado.
     * También inicializa el Grafo, es decir, conecta a las Celdas con sus
     * hijas, dejando efectivamente sus lados libres.
     */
    Tablero();

    /**
     * Constructor copia para Tablero. Crea un Tablero con el mismo estado del
     * que se va a copiar. Los Jugadores mantienen el mismo agente.
     * @param orig una referencia al Tablero que se va a copiar.
     * @sa Tablero::copiar
     */
    Tablero(const Tablero& orig);

    /**
     * Otro tipo de constructor copia para Tablero. Hace lo mismo que el
     * constructor copia habitual, pero recibe un apuntador a Tablero en lugar
     * de una referencia.
     * @param orig un apuntador al Tablero que se va a copiar.
     * @sa Tablero::copiar
     */
    Tablero(const Tablero* orig);

    virtual ~Tablero();

    /**
     *Recibe apuntadores a los Jugadores ya inicializados. Para Colocarlos en el
     * Tablero. Sobreescribe su id, para que se ajuste a su posicion en el
     * vector.
     * @todo: id debería ser un nombre, indendiente de su posición en el vector.
     * Obviamente se tiene que llamar antes de poder mover Jugadores o de pedir
     * un Jugador al Tablero.
     * @param jugadores es una vector de apuntadores a Jugador, con los Jugadores
     * ya construidos para empezar la Jugada.
     */
    void setJugadores(const std::vector< Jugador* > &jugadores);

    /**
     * Coloca una Barrera sobre el Tablero, por parte del Jugador j. Bloquea los
     * lados de las Celdas correspondientes.
     *  Actualiza las Barreras desponibles de dicho Jugador, y la lista de
     * Barreras colocadas.
     * @param j es una referencia al Jugador que coloca la Barrera.
     * @param b es una referencia a la Barrera que se va a colocar.
     */
    void setBarrera(Jugador &j, const Barrera &b);

    /**
     * Coloca una Barrera sobre el Tablero, por parte del Jugador con
     * identificador idJugador. Actualiza las Barreras desponibles de dicho
     * Jugador, y la lista de Barreras colocadas.
     * @param idJugador es un entero que representa el id del Jugador que coloca
     * la Barrera.
     * @param b es una referencia a la Barrera que se va a colocar.
     */
    void setBarrera(int idJugador, const Barrera &b);

    /**
     * Quita una Barrera del Tablero. Se la regresa al Jugador.
     * @param j es una referencia al Jugador al que se le regresa la Barrera
     * @param b la Barrera que se va a quitar.
     */
    void quitarBarrera(Jugador &j, Barrera &b);

    /**
     * Quita una Barrera del Tablero. Se la regresa al Jugador.
     * @param idJugador El id del Jugador al que se le regresa la Barrera
     * @param b la Barrera que se va a quitar.
     */
    void quitarBarrera(int idJugador, const Barrera &b);

    /**
     *Mueve un jugador de su Celda actual a Celda nueva. Actualiza la posición
     * del jugador, y libera la celda actual.
     * @param jugador es el identificador del Jugador que se va a mover.
     * @param celda es una referencia a la Celda a la que se quiere mover al
     * Jugador.
     * @throws JugadorNoColocado si el Jugador solicitado no está aún colocado
     * en el Tablero.
     */
    void moverJugador(int jugador, Celda& celdaNueva);

    /**
     * Mueve un Jugador de su Celda actual a la Celda en la posición pos.
     *  Actualiza la posición del jugador, y libera la celda actual.
     * @param jugador es el identificador del Jugador que se va a mover.
     * @param pos es una referencia a un vector de enteros (x, y) que representa
     * la posición de la Celda a la que se quiere mover al Jugador.
     * @throws ParametrosMalos si posicion no tiene dos enteros.
     * @throws JugadorNoColocado si el jugador solicitado no está aún colocado
     * en el Tablero.
     */
    void moverJugador(int jugador, const std::vector<int> &pos);

    /**
     * Mueve un Jugador de su Celda actual a la Celda en la posición dada por
     * (x, y). Actualiza la posición del jugador, y libera la celda actual.
     * @param jugador es el identificador del Jugador que se va a mover.
     * @param x es un entero que representa la posición horizontal de la Celda a
     * la cual se quiere mover al Jugador.
     * @param y es un entero que representa la posición horizontal de la Celda a
     * la cual se quiere mover al Jugador.
     */
    void moverJugador(int jugador, int x, int y);

    /**
     *Nos indica si la Celda especificada está dentro del Tablero. (?)
     * @param celda es una referencia a la Celda que se está buscando.
     * @return true si Celda está dentro del Tablero, false en otro caso.
     */
    bool tieneCelda(const Celda& celda) const;

    /**
     * Nos indica la posición del Jugador cuyo id se proporcionó.
     * @param idJugador un entero que contiene el id del Jugador cuya posición
     * se quiere conocer.
     * @return un vector de dos enteros (x, y) que nos indica la posición del
     * Jugador.
     * @throws JugadorNoColocado si se pide la posición de un Jugador que no ha
     * sido colocado aún en el Tablero.
     */
    const std::vector<int> &getPosicion(int idJugador) const;

    /**
     * Nos regresa a todas las Barreras que estén ya colocadas sobre el Tablero.
     * @return una referencia a una lista de Barreras ya colocadas.
     */
    const std::list<Barrera>& getBarrerasColocadas() const;

    /**
     * Regresa el vector que contiene a todas las metas que le corresponden al
     * Jugador dado.
     * @param idJugador el id del Jugador del que se desea saber sus metas.
     * @return Una referencia a el vector que contiene a las metas del jugador
     * especificado.
     * @hrows JugadorNoColocado si el idJugador no corresponde con la cantidad de
     * Jugadores en el Tablero.
     * @todo: solo sirve para cuando hay dos Jugadores. No más. Ni menos :P.
     * Arreglarlo para más.
     */
    const std::vector<Reglas::Celda>& getMetas(int idJugador) const;

    /**
     * Nos regresa la Celda en la posición especificada por el vector pos.
     * @param pos un vector de dos enteros (x, y).
     * @return Una referencia a la Celda que se solicitó.
     * @throws ParametrosMalos si pos no es un vector con dos enteros.
     * @throws PiezaFueraDelTablero si se da una posición que no existe dentro
     * del Tablero.
     */
    const Celda &getCelda(const std::vector<int> &pos) const;

    /**
     * Nos regresa la Celda en la que se encuentra el Jugador con id idJugador.
     * @param idJugador es un entero que representa el id del Jugador cuya celda
     * actual se quiere conocer.
     * @return Una referencia a la Celda que se solicitó.
     * @throws ParametrosMalos si pos no es un vector con dos enteros.
     * @throws PiezaFueraDelTablero si se da una posición que no existe dentro
     * del Tablero.
     */
    const Celda &getCelda(int idJugador) const;

    /**
     * Nos regresa la Celda en la posición especificada.
     * @param x un entero que indica la posición horizontal.
     * @param y un entero que indica la posición vertical.
     * @return Una referencia a la Celda que se solicitó.
     * @throws ParametrosMalos si pos no es un vector con dos enteros.
     * @throws PiezaFueraDelTablero si se da una posición que no existe dentro
     * del Tablero.
     */
    const Celda &getCelda(int x, int y) const;

    /**
     *Regresa el Jugador con indentificador id.
     * @param id un entero que nos dice el identificador que Jugador que se pide.
     * @return una referencia a el Jugador solicitado.
     */
    const Jugador &getJugador(int id) const;

    /**
     * Determina si el Jugador con el id dado, está colocado en el Tablero.
     * @param id el id del Jugador que se está buscando.
     * @return true si el Jugador con id está en el Tablero, false de no ser así.
     */
    bool tieneJugador(int id) const;

    /**
     * El operador == sobrecargado. Compara si dos Tableros tienen el mismo
     * estado. (los jugadores en la misma posición, y las mismas barreras
     * colocadas)
     * @param otro una referencia a un Tablero.
     * @return true si los tableros tienen el mismo estado, false de lo contrario.
     */
    bool operator==(const Tablero &otro) const;

    /**
     * El operador != sobrecargado. Compara si dos Tableros tienen el mismo
     * estado. (los jugadores en la misma posición, y las mismas barreras
     * colocadas)
     * @param otro una referencia a un Tablero.
     * @return false si los tableros tienen el mismo estado, true de lo contrario.
     */
    bool operator!=(const Tablero &otro) const;
    

    /**
     * Estos dos miembros nos indican el numero de Celdas que tiene el Tablero,
     * tanto horizontal como verticalmente.
     */
    static const int size_x = 9, size_y = 9;

    /**
     * Un entero que nos indica cuantas Celdas ocupa una barrera.
     */
    static const int tam_barrera = 2;

    /**
     * Ésta propiedad nos indica el número de Jugadores que mantendrá el Tablero
     * a la vez.
     */
    static const int num_jugadores = 2;
private:
    /**
     * Esta miembro es la matriz que contiene a todas las Celdas del Tablero.
     */
    std::vector <std::vector<Reglas::Celda> > datos;

    /**
     * Este es un vector que guarda apuntadores a los Jugadores de la Partida.
     */
    std::vector< Reglas::Jugador* > jugadores;

    /**
     * Muestra las celdas ocupadas pos los Jugadores.
     */
    std::vector< Reglas::Celda* > celdas_ocupadas;
    /**
     * Esta es una lista que almacena las Barreras que han sido colocadas en 
     * el Tablero.
     */
    std::list<Barrera> barreras_colocadas;

    /**
     * Este es el objeto que nos ayuda a ver las Celdas cuales nodos de un Grafo.
     */
    Grafo *grafo;

    /**
     *Este método es solo para apoyar a los constructores. Le da un tamaño maximo
     * horizontal y vertical al tablero y le asigna posición a todas las Celdas.
     * Pone todas las Celdas libres, y sus lados como bloqueados.
     */
    void init_tablero();

    /**
     * Realiza el trabajo para los constructores copia de Tablero. Copia todo el
     * estado del Tablero origen en el que se está creando (jugadores, Barreras,
     * etc). Mantiene el Agente en los Jugadores.
     * @param orig
     */
    void copiar(const Tablero& orig);
};
}
/**
     * Este es una sobrecarga del operador << de cout, para poder imprimir de
     * manera bonita e informativa a stdout la clase Tablero. Ejemplo
     * @code
     * Reglas::Tablero t = new Reglas::Tablero();
     * std::cout << *c << endl;
     * @endcode
     */
    std::ostream& operator<< (std::ostream &out, const Reglas::Tablero &tab);

#endif	/* _TABLERO_HPP */

