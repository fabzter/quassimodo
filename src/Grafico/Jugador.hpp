
#ifndef _JUGADOR_HPP_
#define	_JUGADOR_HPP_
#include<irrlicht.h>
#include"Pieza.hpp"
#include<Reglas/Jugador.hpp>
#include "Skin.hpp"
//TODO: terminar de documentar
namespace Grafico{
    using namespace irr;
 /**
  * Clase del namespace Grafico de la clase Jugador, se encarga de la parte grafica como la de Reglas.
  */
class Jugador:public Grafico::Pieza,public Reglas::Jugador {
public:
    /**
     *Constructor del Jugador carga el modelo del jugador, y se manda a dibujar
     * @param smgr Un apuntador al  manejador de la escena
     * @param num parametro requerido por Reglas::Jugador
     * @param a parametro requerido por ReglasJugador
     */
    Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a,Skin* skin);
    /**
     *Constructor copia de la clase Jugador
     * @param orig una referencia a un objeto de la clase Jugador del namespace Grafico
     */
    Jugador(const Jugador& orig);
    virtual ~Jugador();
    /**
     *Gira el Jugador según lo indicado por giro
     * @param giro un vector de 3 dimensiones en donde cada una representa el eje en el plano y el girro que se hará en
     * ese eje.
     */
    void Gira(core::vector3df giro);

    bool Mover(scene::ISceneManager* smgr,core::vector3df npos);

    bool endAnimacion();

private:
    scene::IAnimationEndCallBack* callback;

};
}
#endif	/* _JUGADOR_HPP */

