
#ifndef _TABLERO_HPP_
#define	_TABLERO_HPP_
#include "Pieza.hpp"
#include<Reglas/Tablero.hpp>
#include<irrlicht.h>
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
    /**
     *Método encargado de colocar a todo el tablero en una nueva posición
      * @param x Un entero que representa la nueva posición que tendrá en el eje de las X
      * @param y Un entero que representa la nueva posición que tendrá en el eje de las Y
      * @param z Un entero que representa la nueva posición que tendrá en el eje de las Z
     */
    void setPosicionTablero(int x,int y,int z);
      /**
         *Método encargado aumentar o disminuir la escala del tablero
         * @param x Un entero que representa la escala que tendrá en el eje de las X
         * @param y Un entero que representa la escala que tendrá en el eje de las Y
         * @param z Un entero que representa la escala que tendrá en el eje de las Z
         */
    void setEscalaTablero(int x,int y,int z);
     /**
         *Regresa la posicion en donde se encuentra colocado el tablero
         * @return Un vector3df que indica la posicion del tablero
         */
        irr::core::vector3df getPosicionTablero();
        /**
         *Método que a partir de la posicion de la celda en le tablero, por ejemplo la celda que es ta en la pos (5,3)
         * te regresa la posicion de esa celda pero en la escena (osea las coordenadas Gráficas)
         * @param x Un entero que representa la posicionen el eje X
         * @param y Un entero que representa la posicionen el eje Y
         * @return Un vector3df que indica la posicion de la celda pedida.
         */
         irr::core::vector3df getPosicionCelda(int x,int y);


private:
    /**
     * Vector de vectores en que contiene las celdas que se van a dibujar en la escena
     */
    std::vector <std::vector<Grafico::Celda*> > datos;
};
}

#endif	/* _TABLERO_HPP */

