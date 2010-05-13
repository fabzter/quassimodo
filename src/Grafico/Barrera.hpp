

#ifndef _BARRERA_HPP_
#define	_BARRERA_HPP_
#include<Reglas/Barrera.hpp>
#include<Reglas/Enums.hpp>
#include"Pieza.hpp"
#include<irrlicht.h>
#include <irrlicht/vector3d.h>
#include "Skin.hpp"

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
    Barrera(scene::ISceneManager* smgr,Skin* skin);
    /**
     *Constructor copia de la clase Barrera
     * @param orig una referencia a un objeto de la clase Barera del namespace Grafico
     */
    Barrera(const Barrera& orig);

    virtual ~Barrera();
    /**
     *Coloca la barrera para que tenga dirección norte
     */
    void giraNorte();
    /**
     *Coloca la barrera para que tenga dirección Este
     */
    void giraEste();
    /**
     *Coloca la barrera en la escena y actualiza en el namespace Reglas
     * @param posg posicion que tendra la barrera en la escena
     * @param pos posicion que tendrá en el namespace grafico, esta es la posicion de la celda
     * en la que se quiere que se coloque la barrera
     * @param dir dirección en la que la barrera estará ( ya sea ESTE o NORTE )
     */
    void ColocaBarrera(irr::core::vector3df posg,const std::vector<int> &pos,Reglas::Direccion dir);
private:

};
}
#endif	/* _BARRERA_HPP */

