

#ifndef _CELDA_HPP_
#define	_CELDA_HPP_
#include<Reglas/Celda.hpp>
#include<irrlicht/irrlicht.h>
#include"Pieza.hpp"

namespace Grafico{
/**
 * La clase celda se encarga de conocer el estado de cada celda en el trblero, deriva de la
 * clase Reglas::Celda, y se encarga de dibujarse a si misma en la escena
 */
class Celda:public Grafico::Pieza,public Reglas::Celda {
public:
    /**
     *Constructor de la clas celda inicia la celda del namespace Reglas
     * y dibuja la celda en la escena en la posicion (0,0,0).
     * @param smgr Un apuntador al  manejador de la escena
     */
    Celda(scene::ISceneManager* smgr);
    /**
     *Constructor copia de la clase Celda
     * @param orig una referencia a un objeto de la clase Celda del namespace Grafico
     */
    Celda(const Celda& orig);
    virtual ~Celda();
private:

};
}
#endif	/* _CELDA_HPP */

