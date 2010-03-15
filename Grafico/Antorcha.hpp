

#ifndef _ANTORCHA_HPP
#define	_ANTORCHA_HPP
#include<irrlicht/irrlicht.h>
#include "Pieza.hpp"
namespace Grafico{
using namespace irr;
    /**
     * Clase del paquete Grafico, es una antorcha con fuego que sirve como iluminación al juego
     */
   class Antorcha : public Pieza {

    public:
        /**
         *Constructor de la clase Antorcha  se crea pegada al piso por lo que solo se podra mover en dirección X o Z.
         * @param smgr Un apuntador al  manejador de la escena que nos servira para crear la antorcha y el fuego
         * @param x Un entero que representa la posicion que tendrá en el eje de las X
         * @param z Un entero que representa la posicion que tendrá en el eje de las z
         * @param sombra Un booleano que indica si la antorcha llevara sombra o no por default no lleva.
         */
        Antorcha(scene::ISceneManager* smgr,int x, int z,bool sombra=false);
        /**
         *Constructor copia de la clase Antorcha
         * @param orig una referecnia a un objeto Antorcha
         */
        Antorcha(const Antorcha& orig);
        /**
         *Destructor de la clase Antorcha
         */
        virtual ~Antorcha();
        /**
         *Método encargado de dibujar la antorcha, este sólo se llama una sola vez pues depues de ser dibujado, el manejador de la
         * escena se encarga de mantenerlo.
         * @param smgr Un apuntador al  manejador de la escena
         * @param sombra Un booleano que indica si la antorcha llevara sombra o no
         */
        void dibuja(scene::ISceneManager* smgr,bool sombra);
        /**
         *Método encargado de colocar a la antorcha en una nueva posición
         * @param posicion un vector3df que contiene la nueva posición en donde se desea colocar la antorcha
         */
        void setPosicionAntorcha(core::vector3df posicion);
        /**
         *Método encargado de colocar a la antorcha en una nueva posición
         * @param x Un entero que representa la nueva posición que tendrá en el eje de las X
         * @param y Un entero que representa la nueva posición que tendrá en el eje de las Y
         * @param z Un entero que representa la nueva posición que tendrá en el eje de las Z
         */
        void setPosicionAntorcha(int x, int y ,int z);
        /**
         *Regresa la posicion en donde se encuentra colocada la antorcha
         * @return Un vector3df que indica la posicin de la antorcha
         */
        core::vector3df getPosicionAntorcha();

    private:
        /**
         *Metodo auxiliar que sirve para colocarle los valores a los vectores de posicion de la antorcha y del fuego
         * @param x Un entero que representa la posicion que tendrá en el eje de las X
         * @param y Un entero que representa la posicion que tendrá en el eje de las Y
         * @param z Un entero que representa la posicion que tendrá en el eje de las Z
         */
        void setVectPosicion(int x, int y, int z);
   
        
         /**
         * Un vector de 3 enteros que indica la posicion del fuego.
         */
        core::vector3df posicionF;
     
        /**
         * Un nodo escena que contiene el fuego
         */
        scene::ISceneNode *nfuego;
        /**
         * La textura o la imagen que la que el fuego toma
         */
        video::ITexture *fuego;

    };
}
#endif	/* _ANTORCHA_HPP */

