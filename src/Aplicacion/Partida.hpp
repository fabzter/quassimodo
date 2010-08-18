
#ifndef _PARTIDA_HPP_
#define	_PARTIDA_HPP_
#include<Reglas/Juez.hpp>
#include<Reglas/Jugador.hpp>
#include<vector>
#include<Scripting/Manejador.hpp>
#include<Scripting/Excepciones.hpp>

/**
 * Clase base para realizar una partida, esta contiene los métodos principales para que las clases hijas puedan
 * mantener una partida ya sea en modo consola o en modo gráfico.
 */
class Partida {
public:

    /**
     * Constructor de la case partida, se encarga de inicializar todas las variables exepto
     * los jugadores
     */

    Partida();
    /**
     * Constructor copia de la clase Partida
     * @param orig un objeto de la clase Partida
     */
    Partida(const Partida& orig);
    /**
     * Destructor
     */
    virtual ~Partida();

    /**
     * Una vez que se ha contruido la partida, este método les avisa a los
     * Jugadores que su partida está por empezar.
     */
    virtual void iniciarPartida() =0;
    /**
     * Éste método es el punto principal de interacción con la clase PartidaGrafica.
     * Le pida una Jugada del Jugador que al que le toca realizar una, y de ser
     * ésta válida, actualiza el Tablero para reflejar los cambios hechos por la
     * misma. Esto último incluye actualizar la lista de Barreras colocadas,
     * bloquear las Direcciones de las Celdas dependiendo de las Barreras,poner
     * a los Jugadores en su lugar, poner como ocupadas las Celdas sobre las que
     * están los Jugadores.
     * @return true si se realizó la jugada
     * @return false si la Jugada ha terminado (por ejemplo si ganó un Jugador)
     * @throws ReglasRotas si la Jugada no comple con las reglas.
     * @throws ScriptMalo Si el script tiene errores de sintaxis o algún otro
     * tipo de error relacionado con el lenguage de scripting.
     * @throws PartidaTerminada si se intenta llamar cuando la PartidaGrafica ha
     * terminado por cualquier motivo.
     */
    virtual bool siguienteJugada()=0;
    /**
     *Método que sirve para gargar los dos jugadores al tablero a partir de la ruta en donde estan
     * ubicados dichos agentes
     * @param rutaAgente1 un string que contiene la ruta en la que se encuentra el agente 1
     * @param rutaAgente2 un string que contiene la ruta en la que se encuentra el agente 2
     * @param t Un apuntador al tablero de la partida.
     * @return true si los agentes fueron cargados exitosamente, false en caso contrario.
     */
    virtual bool SetJugadores(std::string rutaAgente1,std::string rutaAgente2)=0;
    /**
     * Nos indica el valor de la bandera esta_en_curso.
     * @return true si la Partida está andando, false si se ha detenido por
     * algún motivo (como que hay un ganador o se han roto las reglas).
     */
    virtual bool estaEnCurso();

    /**
     * Nos indica el valor de la bandera hay_ganador
     * @return true si existe un ganador, false de lo contrario.
     */
    bool hayGanador();
    /**
     * Regresa el numero del jugador ganador.
     * @return un entero que indica el jugador que gano la partida.
     */
    int getJugadorGanador();
    /**
     * regresa el agente con el error
     * @return un caracter que contiene el numero del agente con error
     */
    char getAgenteConError();


protected:
    /**
     * Extrae los agentes de las rutas dadas y regresa un vector con apuntadores a ellos
     * @param rutaAgente1 string que contiene la ruta del agente 1
     * @param rutaAgente2 string que contiene la ruta del agente 1
     * @param t un apuntador al tablero de la partida.
     * @return Un vector que contiene los apuntadores a los 2 agentes dados.
     */
    virtual std::vector<Reglas::Agente*> getAgentes(std::string rutaAgente1,std::string rutaAgente2,Reglas::Tablero* t);
    /**
     * Llamada por siguienteJugada, si la Jugada es válida. Realiza la parte de
     * actualización especificada en siguienteJugada.
     * @sa siguienteJugada.
     * @param j es la Jugada con la que se a actualizar el Tablero.
     * @param idJugador es el id del Jugador que realiza la Jugada.
     */
    virtual void actualizarTablero(Reglas::Jugada &j, int idJugador)=0;
    /**
     * En si este es le método siguente jugada, pues cada clase que hereda de esta manda a llamar a este método
     * solo que cada una le pasa un apuntador a su tablero.
     * @param t un apuntador al tablero de la partida
     * @return true si se realizó la jugada
     * @return false si la Jugada ha terminado (por ejemplo si ganó un Jugador)
     */
    bool Siguiente(Reglas::Tablero *t);
    /**
     * Un vector de Jugadores, esto pues ya que los Jugadores serán dibujadas y no queremos que se
     * eliminen hasta el final de la partida.
     */

    std::vector< Reglas::Jugador* > jugadores;
    /**
     * Este es el Juez que se usará en ésta partida. Atado al Tablero con que se
     * construye la Partida, éste Juez se construye en el contructor de Partida.
     */
     Reglas::Juez* juez;
    /**
     * El identificador del Jugador al que se le pedirá la siguiente Jugada.
     */
     int jugador_en_turno;

     /**
      * Tiene el id del Jugador que ha ganado la Partida.
      */
     int jugador_ganador;

     /**
      * Bandera que se prende en true cuando hay un ganador.
      */
     bool hay_ganador;

     /**
      * Bandera que se encuentra en true mientras la Partida no ha terminado.
      */
     bool en_curso;
     /**
      * variale que indica el agente en el que hubo error al cargarse;
      */
     char errorEnAgente;



};

#endif	/* _PARTIDA_HPP_ */

