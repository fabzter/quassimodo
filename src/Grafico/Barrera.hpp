

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
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param VelAnim velocidad de animacion que tendrá la barrera
     * @param parent Nodo escena del que será hijo la barrera.
     * @param idJugador un entero que indica el id del jugador, ya que segun el numero de jugador es el tipo de barrera que colocará
     */
    Barrera(scene::ISceneManager* smgr,Skin* skin,int VelAnim,scene::ISceneNode* parent,int idJugador);
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
     * Coloca la barrera para que tenga dirección Este
     * @param pos posicion en la que estara la barrera
     * @return  un vector que contiene la nueva posicion ya con el giro en la barrera
     */
    core::vector3df giraEste(core::vector3df pos);
    /**
     *Coloca la barrera en la escena y actualiza en el namespace Reglas
     * @param posg posicion que tendra la barrera en la escena
     * @param pos posicion que tendrá en el namespace grafico, esta es la posicion de la celda
     * en la que se quiere que se coloque la barrera
     * @param dir dirección en la que la barrera estará ( ya sea ESTE o NORTE )
     * @param smgr Un apuntador al  manejador de la escena
     * @param animacion booleano que sirve en caso de que no se quiera la animacion y solo colocar la barrera
     */
    void ColocaBarrera(irr::core::vector3df posg,const std::vector<int> &pos,Reglas::Direccion dir,scene::ISceneManager* smgr,bool animacion=true);
    /**
     * Método que indica si la animación de colocar la barrera termino o no.
     * @return  true si la animacion termino, false en caso contrario.
     */
    bool endAnimacion();
private:
    /**
     * velocidad de animacion que tendrá la barrera
     */
    int velAnim;

};
}
#endif	/* _BARRERA_HPP */

