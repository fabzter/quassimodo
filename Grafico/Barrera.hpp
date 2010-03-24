

#ifndef _BARRERA_HPP_
#define	_BARRERA_HPP_
#include<Reglas/Barrera.hpp>
#include"Pieza.hpp"
#include<irrlicht.h>

namespace Grafico{
  using namespace irr;
    /**
     * Clase Barrera Se encarga de dibujar la barrera en la escena, contiene la clase Barrera del namespace
     * Reglas. esta clase se debe de encargar de todos los cambios que se le quiera hacer en el namespace
     * Reglas (la clase Barrera) deba de verse reflejado en la escena.
     */

class Barrera: public Grafico::Pieza,public Reglas::Barrera {
public:
     /**
     *Constructor de Barrera carga los objetos del tablero que se van a dibujar, llama al constructor de la
     * clase Reglas::Barrera
     * @param smgr Un apuntador al  manejador de la escena
     */
    Barrera(scene::ISceneManager* smgr);
    /**
     *Constructor copia de la clase Barrera
     * @param orig una referencia a un objeto de la clase Barera del namespace Grafico
     */
    Barrera(const Barrera& orig);
    virtual ~Barrera();
private:

};
}
#endif	/* _BARRERA_HPP */

