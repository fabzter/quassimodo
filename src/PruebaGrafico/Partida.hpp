
#ifndef _PARTIDA_HPP
#define	_PARTIDA_HPP
#include<irrlicht.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Tablero.hpp>
#include<Grafico/Barrera.hpp>
#include<Grafico/Celda.hpp>
#include<Grafico/Jugador.hpp>
#include<Reglas/Agente.hpp>
#include<Scripting/Manejador.hpp>
#include<Reglas/Juez.hpp>
#include<vector>
#include<Grafico/Skin.hpp>
using namespace irr;
class Partida {
    friend class ManejadorJuego;
public:

    /**
     * Constructor de la case partida, se encarga de inicializar todas las variables exepto
     * los jugadores,
     * @param smgr Un apuntador al  manejador de la escena
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Partida(scene::ISceneManager* smgr,Grafico::Skin* skin);
    /**
     *reserva memoria e inicializa al tablero y al juez
     * @param smgr Un apuntador al  manejador de la escena
     */
    void init(scene::ISceneManager* smgr);
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
     * Cambia la escala del juego por si acaso los modelos llegan a ser muy grandes o muy pequeños
     * actualiza el tamaño de todo, tanto de las celdas, el tablero, los jugadores, el juez, las antorchas etc.
     * @param x Un entero que representa la escala que tendrá en el eje de las X
     * @param y Un entero que representa la escala que tendrá en el eje de las Y
     * @param z Un entero que representa la escala que tendrá en el eje de las Z
     */
    void SetEscala(int x,int y,int z);
    /**
     * Una vez que se ha contruido la partida, este método les avisa a los
     * Jugadores que su partida está por empezar.
     */
    void iniciarPartida();

    /**
     * Éste método es el punto principal de interacción con la clase Partida.
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
     * @throws PartidaTerminada si se intenta llamar cuando la partida ha
     * terminado por cualquier motivo.
     */
    bool siguienteJugada(scene::ISceneManager* smgr);

    /**
     * Nos indica el valor de la bandera esta_en_curso.
     * @return true si la Partida está andando, false si se ha detenido por
     * algún motivo (como que hay un ganador o se han roto las reglas).
     */
    bool estaEnCurso();

    /**
     * Nos indica el valor de la bandera hay_ganador
     * @return true si existe un ganador, false de lo contrario.
     */
    bool hayGanador();

    /**
     *Método que sirve para gargar los dos jugadores al tablero a partir de la ruta en donde estan
     * ubicados dichos agentes
     * @param rutaAgente1 un string que contiene la ruta en la que se encuentra el agente 1
     * @param rutaAgente2 un string que contiene la ruta en la que se encuentra el agente 2
     * @param  smgr un apuntador al manejador de la escena
     * @return true si los agentes fueron cargados exitosamente, false en caso contrario.
     */
    bool SetJugadores(std::string rutaAgente1,std::string rutaAgente2,scene::ISceneManager* smgr,scene::IAnimationEndCallBack* callback);
    /**
     *Elimina la partida en curso (los jugadores, las barreras colocadas,etc) y coloca la partida
     * en blanco (antes de colocar a os jugadores)
     * @param smgr Un apuntador al  manejador de la escena
     */
    void NuevaPartida(scene::ISceneManager* smgr);
    /**
     *Calcula el centro del tablero y lo regresa
     * @return un vector de tamaño 3 en donde tiene el centro del tablero
     */
    core::vector3df getCentro();
    /**
     *imprime el tablero en consola
     */
    void impimeTablero();

private:
    /**
     *Coloca las antorchas su posicion
     */
    void ColocaAntorchas();
    /**
     * Llamada por siguienteJugada, si la Jugada es válida. Realiza la parte de
     * actualización especificada en siguienteJugada.
     * @sa siguienteJugada.
     * @param j es la Jugada con la que se a actualizar el Tablero.
     * @param idJugador es el id del Jugador que realiza la Jugada.
     */
    void actualizarTablero(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr);
    /**
     * Método llamado por Actualizar tablero, este metodo es encargado de calcular la posicion
     * en la que el jugador se movera y colocara a el jugador en dicha posicion
     * @param j Jugada a realizar por el jugador
     * @param idJugador id del jugador que quiere realizar el movimiento
     * @param  smgr un apuntador al manejador de la escena
     */
    bool MoverJugador(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr);
    /**
     *Coloca la barrera segun la posicion que indica la jugada
     * @param j un objeto de la clase Reglas::Jugada que indica la jugada a realizarse
     * @param idJugador id del juegador que realizara la Jugada
     * @param  smgr un apuntador al manejador de la escena
     */
    void SetBarrera(Reglas::Jugada &j, int idJugador,scene::ISceneManager* smgr);
    /**
     *Elimina las Barreras de la partida y de la escena actual.
     */
    void dropBarreras();
    /**
     *Elimina los jugadores de la partida y de la escena actual.
     */
    void dropJugadores();
 
    /**
     * Tablero sobre el que se realizará la partida.
     */
    Grafico::Tablero *t;
    /**
     * Un vector de antorchas que será la iluminación del juego
     */
    std::vector<Grafico::Antorcha*> antorchas;
    /**
     * Un vector de barreras, esto pues ya que las barreras serán dibujadas y no queremos que se
     * eliminen hasta el final de la partida.
     */
    std::vector<Grafico::Barrera*> Barreras;

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
      *  un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
         * @param sombra Un booleano que indica si la antorcha llevara sombra o no por default no lleva.
      */
     Grafico::Skin* skin;
     /**
      * vector en donde se almacena la escala que tendrá el juego.
      */
     core::vector3df escala;


};

#endif	/* _PARTIDA_HPP */

