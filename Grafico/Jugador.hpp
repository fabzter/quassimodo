
#ifndef _JUGADOR_HPP_
#define	_JUGADOR_HPP_
#include<irrlicht.h>
#include"Pieza.hpp"
#include<Reglas/Jugador.hpp>

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
    Jugador(scene::ISceneManager* smgr,int num, Reglas::Agente *a);
    /**
     *Constructor copia de la clase Jugador
     * @param orig una referencia a un objeto de la clase Jugador del namespace Grafico
     */
    Jugador(const Jugador& orig);
    virtual ~Jugador();

private:

};
}
#endif	/* _JUGADOR_HPP */

