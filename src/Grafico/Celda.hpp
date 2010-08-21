

#ifndef _CELDA_HPP_
#define	_CELDA_HPP_
#include<Reglas/Celda.hpp>
#include<irrlicht.h>
#include"Pieza.hpp"
#include "Skin.hpp"
#include <stdlib.h>
#include <time.h>
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
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     * @param parent Nodo escena del que la celda será hijo, en este caso el tablero.
     */
    Celda(scene::ISceneManager* smgr,Skin* skin,scene::ISceneNode* parent);
    /**
     *Constructor copia de la clase Celda
     * @param orig una referencia a un objeto de la clase Celda del namespace Grafico
     */
    Celda(const Celda& orig);
    /**
     * Destructor de la clase Celda
     */
    virtual ~Celda();

    /**
     *Coloca un apuntador ala celda del namespace de reglas
     * @param celdaR un apuntador al una celda del namespace Reglas.
     */
    void setCeldaR (const Reglas::Celda *celdaR);
    /**
     *Coloca la celda en la posicion que le corresponde en la escena, según la posicion en la que esta
     * en la clase Celda del namespace Reglas. a partir de los parametros dados será la posicion que se mueva
     * por ejemplo si x, y z son cero la celda con coordenadas (0,0) estara colocada en (x,y,z) osea en 0,0,0
     * @param x Un entero que representa la posición que tendrá en el eje de las X
     * @param y Un entero que representa la posición que tendrá en el eje de las Y
     * @param z Un entero que representa la posición que tendrá en el eje de las Z
     */
    void colocar(int x=0,int y=0,int z=0);
     /**
      *Método encargado aumentar o disminuir la escala de la celda
      * @param x Un entero que representa la escala que tendrá en el eje de las X
      * @param y Un entero que representa la escala que tendrá en el eje de las Y
      * @param z Un entero que representa la escala que tendrá en el eje de las Z
      */
    void SetEscalaCelda(int x, int y, int z);
    /**
     * Resalta la celda actual.
     */
    void ResaltarCelda();
    /**
     * Regresa la celda al estado normal.
     */
    void dropResaltado();
private:
    /**
     * Un apuntador a  una clase de Reglas::Celda esta apunta a una celda en el tablero
     *
     */
    const Reglas::Celda *celdaR;
    /**
     * Un apuntador al  manejador de la escena
     */
    scene::ISceneManager* smgr;
    Skin* skin;

    

};
}
#endif	/* _CELDA_HPP */

