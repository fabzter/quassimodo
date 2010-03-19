

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
class Celda:public Grafico::Pieza{
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
    /**
     *Obtenemos el tamaño de la celda
     * @return un vector que tiene el tamaño de cada uno de sus ejes en sus ejes.
     */
    core::vector3df getSize();
    /**
     *Coloca un apuntador ala celda del namespace de reglas
     * @param celdaR un apuntador al una celda del namespace Reglas.
     */
    void setCeldaR (const Reglas::Celda *celdaR);
    /**
     *Coloca la celda en la posicion que le corresponde en la escena, según la posicion en la que esta
     * en la clase Celda del namespace Reglas.
     */
    void colocar();
private:
    /**
     * Un apuntador a  una clase de Reglas::Celda esta apunta a una celda en el tablero
     *
     */
    const Reglas::Celda *celdaR;
    /**
     * Tamaño de la celda en los 3 ejes!
     */
     core::vector3df size;

};
}
#endif	/* _CELDA_HPP */

