
#ifndef _TABLERO_HPP_
#define	_TABLERO_HPP_
#include "Pieza.hpp"
#include<Reglas/Tablero.hpp>
#include<irrlicht/irrlicht.h>
#include<vector>
#include"Celda.hpp"

namespace Grafico{
    using namespace irr;
/**
 * Contiene el tablero, tanto de la parte grafica como el de las reglas.
 */
class Tablero: public Pieza, public Reglas::Tablero  {
public:
    /**
     *Constructor de Tablero carga los objetos del tablero que se van a dibujar, llama al constructor de la
     * clase Reglas::Tablero
     * @param smgr Un apuntador al  manejador de la escena
     */
    Tablero(scene::ISceneManager* smgr);
    /**
     *Constructor copia de la clase Tablero
     * @param orig una referencia a un objeto de la clase Tablero del namespace Grafico
     */
    Tablero(const Tablero& orig);
    virtual ~Tablero();
    /**
     *Metodo encargado de dibujar todo tanto el tablero como las celdas
     * @param smgr Un apuntador al  manejador de la escena
     */
    void DibujaTodo(scene::ISceneManager* smgr);
    

private:
    /**
     * Vector de vectores en que contiene las celdas que se van a dibujar en la escena
     */
    std::vector <std::vector<Grafico::Celda*> > datos;
};
}

#endif	/* _TABLERO_HPP */

