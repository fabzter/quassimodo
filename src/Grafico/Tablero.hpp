
#ifndef _TABLERO_HPP_
#define	_TABLERO_HPP_
#include "Pieza.hpp"
#include<Reglas/Tablero.hpp>
#include<irrlicht.h>
#include<vector>
#include"Celda.hpp"
#include<cstdlib>

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
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Tablero(scene::ISceneManager* smgr,Skin* skin,scene::ISceneNode* parent);
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
    void DibujaTodo(scene::ISceneManager* smgr,Skin* skin);
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
     /**
      *Método que a partir de la posicion de la celda en le tablero, por ejemplo la celda que es ta en la pos (5,3)
      * te regresa la posicion de esa celda pero en la escena (osea las coordenadas Gráficas)
      * @param pos Un vector que nos indica la posicion
      * @return Un vector3df que indica la posicion de la celda pedida.
      */
     irr::core::vector3df getPosicionCelda(std::vector<int> pos);
     /**
      *Regresa el tamaño que ocupa la formacion de cada una de las celdas, osea el tamaño q tiene desde
      * el inicio de la celda hasta el final de la celda en un eje
      * @return Un vector3df que indica el tamaño de la linea de celdas
      */
     irr::core::vector3df getsizeLineaCeldas();
     /**
      *Regresa el tamaño que tiene una de las celdas (puesto que todas son del mismo tamaño)
      * @return Un vector3df que indica el tamaño de la celda
      */
     irr::core::vector3df getsizeCelda();
     /**
      * Regresa cunatas celdas va a saltar el jugador
      * @param actual posicion actual del jugador
      * @param nueva posicion nueva del jugador
      * @return un entero que indica el número de celdas a moverse.
      */
     int getCeldasAMover(const std::vector<int> & actual,const std::vector<int> & nueva);

private:
    /**
     * Vector de vectores en que contiene las celdas que se van a dibujar en la escena
     */
    std::vector <std::vector<Grafico::Celda*> > datos;

};
}

#endif	/* _TABLERO_HPP */
