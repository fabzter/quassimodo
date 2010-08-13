
#ifndef PARTIDAGRAFICA_HPP
#define	PARTIDAGRAFICA_HPP
#include<Reglas/Juez.hpp>
#include<Reglas/Jugador.hpp>
#include<vector>
#include<irrlicht/irrlicht.h>
#include<Grafico/Barrera.hpp>
#include<Grafico/Tablero.hpp>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Jugador.hpp>
#include <Agentes/ManejadorAgentes.hpp>

using namespace irr;
using namespace Grafico;
class PartidaGrafica {
 friend class ManejadorJuego;
public:
/**
     * Constructor de la case PartidaGrafica, se encarga de inicializar todas las variables exepto
     * los jugadores,
     * @param smgr Un apuntador al  manejador de la escena
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param env un apuntador al creador de GUI que proporciona irrlicht
     * @param VelAnim velocidad de animacion que tendrá la barrera y el jugador
     */
    PartidaGrafica(scene::ISceneManager* smgr,Grafico::Skin* skin,gui::IGUIEnvironment* env,int VelAnim);

    /**
     * Constructor copia de la clase PartidaGrafica
     * @param orig un objeto de la clase PartidaGrafica
     */
    PartidaGrafica(const PartidaGrafica& orig);
    /**
     * Destructor
     */
    virtual ~PartidaGrafica();
    /**
     * Cambia la escala del juego por si acaso los modelos llegan a ser muy grandes o muy pequeños
     * actualiza el tamaño de todo, tanto de las celdas, el tablero, los jugadores, el juez, las antorchas etc.
     * @param x Un entero que representa la escala que tendrá en el eje de las X
     * @param y Un entero que representa la escala que tendrá en el eje de las Y
     * @param z Un entero que representa la escala que tendrá en el eje de las Z
     */
    void SetEscala(int x,int y,int z);
    /**
     *@as Partida::iniciarPartida
     */
    virtual void iniciarPartida();

    /**
     *@as Partida::siguienteJugada
     */
    virtual bool siguienteJugada();
    /**
     *@as Partida::SetJugadores
     */
    bool SetJugadores(std::string rutaAgente1,std::string rutaAgente2);

    /**
     *Calcula el centro del tablero y lo regresa
     * @return un vector de tamaño 3 en donde tiene el centro del tablero
     */
    core::vector3df getCentro();

    bool animacionesEnd();
    std::vector<std::string> getNombresAgentes();
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



private:
    /**
     *Coloca las antorchas su posicion
     */
    void ColocaAntorchas();
    /**
     *@as Partida::actualizarTablero
     */
    virtual void actualizarTablero(Reglas::Jugada &j, int idJugador);
    /**
     * Método llamado por Actualizar tablero, este metodo es encargado de calcular la posicion
     * en la que el jugador se movera y colocara a el jugador en dicha posicion
     * @param j Jugada a realizar por el jugador
     * @param idJugador id del jugador que quiere realizar el movimiento
     */
    bool MoverJugador(Reglas::Jugada &j, int idJugador);
    /**
     *Coloca la barrera segun la posicion que indica la jugada
     * @param j un objeto de la clase Reglas::Jugada que indica la jugada a realizarse
     * @param idJugador id del juegador que realizara la Jugada
     */
    void SetBarrera(Reglas::Jugada &j, int idJugador);
    /**
     *Elimina las Barreras de la PartidaGrafica y de la escena actual.
     */
    void dropBarreras();
    /**
     *Elimina los jugadores de la PartidaGrafica y de la escena actual.
     */
    void dropJugadores();
            /**
     * En si este es le método siguente jugada, pues cada clase que hereda de esta manda a llamar a este método
     * solo que cada una le pasa un apuntador a su tablero.
     * @param t un apuntador al tablero de la partida
     * @return true si se realizó la jugada
     * @return false si la Jugada ha terminado (por ejemplo si ganó un Jugador)
     */
    bool Siguiente(Reglas::Tablero *t);
    /**
     * Tablero sobre el que se realizará la PartidaGrafica.
     */
    Grafico::Tablero *t;
    /**
     * Un vector de antorchas que será la iluminación del juego
     */
    std::vector<Grafico::Antorcha*> antorchas;
    /**
     * Un vector de barreras, esto pues ya que las barreras serán dibujadas y no queremos que se
     * eliminen hasta el final de la PartidaGrafica.
     */
    std::vector<Grafico::Barrera*> Barreras;
     /**
      *  un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
         * @param sombra Un booleano que indica si la antorcha llevara sombra o no por default no lleva.
      */
     Grafico::Skin* skin;
     /**
      * vector en donde se almacena la escala que tendrá el juego.
      */
     core::vector3df escala;
     /**
      *Un apuntador al  manejador de la escena
      */
     scene::ISceneManager* smgr;
     /**
      *Un apuntador al creador de GUI que proporciona irrlicht
      */
     gui::IGUIEnvironment* env;
    /**
     * velocidad de animacion que tendrá la barrera y el jugador
     */
    int velAnim;
    /**
     *Este objeto es el encargado de administrar los agentes disponibles en el sistema.
     */
    Agentes::ManejadorAgentes* ManAgentes;

     scene::ISceneNode *parent;

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






#endif	/* PARTIDAGRAFICA_HPP */

