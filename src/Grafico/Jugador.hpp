
#ifndef _JUGADOR_HPP_
#define	_JUGADOR_HPP_
#include<irrlicht.h>
#include"Pieza.hpp"
#include<Reglas/Jugador.hpp>
#include "Skin.hpp"
#include"JumpAnimator.hpp"
#include <Agentes/AgenteHumano.hpp>
namespace Grafico{
    using namespace irr;
 /**
  * Clase que se encarga de la parte grafica como la de Reglas.
  */
class Jugador:public Grafico::Pieza,public Reglas::Jugador {
public:
    /**
     *Constructor del Jugador carga el modelo del jugador, y se manda a dibujar
     * @param smgr Un apuntador al  manejador de la escena
     * @param num parametro requerido por Reglas::Jugador
     * @param a parametro requerido por ReglasJugador
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param VelAnim velocidad de animacion que tendrá la barrera y el jugador
     * @param parent Nodo escena del que el Jugador será hijo.
     */
    Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,Skin* skin,int VelAnim,scene::ISceneNode* parent,bool Humano=false);
    /**
     *Constructor del Jugador carga el modelo del jugador, y se manda a dibujar
     * @param num parametro requerido por Reglas::Jugador
     * @param smgr Un apuntador al  manejador de la escena
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param VelAnim velocidad de animacion que tendrá la barrera y el jugador
     */
    Jugador(scene::ISceneManager* smgr,int num,Skin* skin,int VelAnim,scene::ISceneNode* parent);
    /**
     * Inicializa las variables de la clase.
     * @param num parametro requerido por Reglas::Jugador
     * @param smgr Un apuntador al  manejador de la escena
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param VelAnim velocidad de animacion que tendrá la barrera y el jugador
     */
    void init(scene::ISceneManager* smgr,int num,Skin* skin,int VelAnim);
    /**
     *Constructor copia de la clase Jugador
     * @param orig una referencia a un objeto de la clase Jugador del namespace Grafico
     */
    Jugador(const Jugador& orig);
    /**
     * Destructor de la clase.
     */
    virtual ~Jugador();
    /**
     *Gira el Jugador según lo indicado por giro
     * @param giro un vector de 3 dimensiones en donde cada una representa el eje en el plano y el girro que se hará en
     * ese eje.
     */
    void Gira(core::vector3df giro);
    /**
     * Método que se encarga de mover a la pieza del jugador, y manda a actualizar su posicion en el tablero.
     * @param smgr Un apuntador al  manejador de la escena
     * @param npos posicion a la que se movera el jugador.
     * @param numceldas numero de celdas que se movera el jugador, 1 o mas (sirve para indicarle a la animacion el tamaño del salto)
     */
    void Mover(scene::ISceneManager* smgr,core::vector3df npos,int numceldas);
    /**
     * Método que indica si la animación de colocar al jugador termino o no.
     * @return  true si la animacion termino, false en caso contrario.
     */
    bool endAnimacion();
    /**
     * Indica si el Jugador es Humano o no.
     * @return true si es humano, false en caso contrario.
     */
    bool IsHumano();
    /**
     * Si el jugador es Humano se colocara la jugada que eligio, al agente, para que pueda continuar la partida
     * @param j jugada a realizar.
     */
    void setJugada(Reglas::Jugada &j);
    /**
     * si el jugador estahaciendo la jugada o no
     * @return true si esta elaborando la jugada, false en caso contrario.
     */
    bool IsHaciendoJugada();
    /**
     * Coloca si se esta haciendo una jugada o no
     * @param haciendo booleano que indica si se hace una jugada o no
     */
    void setHaciendoJugada(bool haciendo);
    /**
     * Regresa el agente con el cual esta trabajando el jugador
     * @return un apuntador a un objet del tipo Reglas::Agente
     */
    Reglas::Agente* getAgente();

private:
    /**
     * velocidad de animacion que tendrá el jugador
     */
    int velAnim;
    Reglas::Agente *agente;
    bool Humano;
    bool haciendoJugada;
};
}
#endif	/* _JUGADOR_HPP */

