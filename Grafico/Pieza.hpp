

#ifndef _PIEZA_HPP_
#define	_PIEZA_HPP_
#include<irrlicht.h>
namespace Grafico{
 /**
  * Clase Pieza del paquete grafico que contiene la posicion en la que se  encuentra la pieza grafica (el tablero, el jugador la barrera etc.).
  * la malla en donde es cargada la pieza. (el objeto de direct x maya, etc.) y el nodo en el que se dibuja la pieza.
  */
using namespace irr;
class Pieza {

public:
    /**
     *Constructor de la calse Pieza.
     */

    Pieza();
    /**
     *Constructor copia de la  clase pieza
     * @param orig una referencia hacia un objeto de la clase Pieza del namespace Grafico
     */
    Pieza(const Pieza& orig);
    /**
     *Destructor de la clase pieza.
     */
    virtual ~Pieza();
    /**
         *Método encargado de colocar a la pieza en una nueva posición
         * @param posicion un vector3df que contiene la nueva posición en donde se desea colocar la pieza
         */
         virtual void setPosicion(core::vector3df posicion);
        /**
         *Método encargado de colocar a la pieza en una nueva posición
         * @param x Un entero que representa la nueva posición que tendrá en el eje de las X
         * @param y Un entero que representa la nueva posición que tendrá en el eje de las Y
         * @param z Un entero que representa la nueva posición que tendrá en el eje de las Z
         */
         virtual void setPosicion(int x, int y ,int z);
           /**
         *Regresa la posicion en donde se encuentra colocada la pieza
         * @return Un vector3df que indica la posicion de la pieza
         */
        virtual irr::core::vector3df getPosicionEscena();
         /**
         *Regresa el nodo en donde está cargada la pieza
         * @return un apuntador al nodo de la pieza
         */
        virtual scene::IAnimatedMeshSceneNode* getNodo();
        /**
         *Metodo que se encarga de dibujar la pieza, este método es llamado en el constructor de casi todas las piezas
         * @param smgr un apuntador al manejador de la escena
         */
        virtual void dibuja(scene::ISceneManager* smgr);
        /**
         *Método encargado aumentar o disminuir la escala de la pieza
         * @param x Un entero que representa la escala que tendrá en el eje de las X
         * @param y Un entero que representa la escala que tendrá en el eje de las Y
         * @param z Un entero que representa la escala que tendrá en el eje de las Z
         */
        virtual void setEscala(int x, int y ,int z);
        /**
         *Coloca la malla que es la que será dibujada en la escena
         * @param mesh Una malla en donde es cargada la pieza
         */
        virtual void setMesh(scene::IAnimatedMesh* mesh);
            /**
          *Obtenemos el tamaño de la pieza
         * @return un vector que tiene el tamaño de cada uno de sus ejes en sus ejes.
         */
        core::vector3df getSize();
         /**
          *Obtenemos la escala en la que la pieza está dibujada
         * @return un vector que tiene la escala de cada uno de sus ejes
         */
        core::vector3df getEscala();
        /**
         *Elimina de la escena la pieza actual.
         */
        void drop();

protected:
         /**
         * Un vector de 3 enteros que indica la posicion de la pieza
         */
     core::vector3df posicion;
             /**
         * Una malla en donde es cargada la pieza
         */
        scene::IAnimatedMesh* mesh;
        /**
         * Un nodo escena en donde es colocadola malla que contiene la pieza
         */
        scene::IAnimatedMeshSceneNode* nodoA;
        /**
         * Tamaño de la pieza en los 3 ejes!
        */
        core::vector3df size;

};
}
#endif	/* _PIEZA_HPP */

