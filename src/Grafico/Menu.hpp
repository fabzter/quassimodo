

#ifndef _Menu_HPP
#define	_Menu_HPP
#include<string.h>
#include<irrlicht.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/vector3d.h>
#include"Antorcha.hpp"
#include"Tablero.hpp"
#include"Skin.hpp"
#include<string>
#include<vector>
#include<wchar.h>
#include"Enums.hpp"
//TODO: acabar de documentar
using namespace irr;
namespace Grafico{
/**
 * Clase que se encarga del menú principal, poner las piezas de los jugadores (solo para el menú estas no son las mismas que se uilizan en la partida)
 */
class Menu {
public:
    /**
     *Constructor de la clase menu, inicializa los atrubutos y coloca el menu en la escena
     * @param smgr un apuntador al manejador de escena del juego
     * @param env un apuntador al manejador de GUI de irrlicht
     * @param t un apuntador a un objeto de la case Grafico::Tablero
     * @param skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin);
    /**
     *Constructor copia de la clase menu
     * @param orig un objeto de la calse Menu
     */
    Menu(const Menu& orig);
    /**
     *Destructor
     */
    virtual ~Menu();
    /**
     *Coloca los jugadores, los botones, y el la tipografia del menu
     */
    void setMenuP();
    /**
     * Elimina el menú de la escena, los botones, como los jugadores
     */
    void dropMenuP();
    /**
     *Método encargado aumentar o disminuir la escala del menu
     * @param x Un entero que representa la escala que tendrá en el eje de las X
     * @param y Un entero que representa la escala que tendrá en el eje de las Y
     * @param z Un entero que representa la escala que tendrá en el eje de las Z
     */
    void SetEscala(int x,int y,int z);
    /**
     *Desplega el menu en modo consola
     * @return la opcion seleccionada del menu
     */
    char MenuConsola();    

private:
    /**
     *Carga la tipogafia en los botones del menu
     */
     void setSkin();
     /**
      * Coloca las piezas de los jugadores
      */
     void SetJugadores();
     /**
      *Coloca las piezas de los jugadores en su posicion
      */
     void setPosicionJugadores();
     /**
      *Pone los botones del Menu asi como el ToolTip
      */
     void setBotones();
     /**
      * un apuntador a un objeto de la case Grafico::Tablero
      */
    Grafico::Tablero* t;
    /**
     * Un vector en donde estan las piezas de los jugadores del menu
     */
    std::vector<Grafico::Pieza*> jugadores;
    /**
     * Vector en donde se almacenan los botones del Menu
     */
    std::vector<gui::IGUIButton *> botones;
    /**
     * Indica siel menu se esta  mostrando en la aplicacion o no.
     * @return true si se muestra, false en caso contrario.
     */
    bool menuEnEscena();
    /**
     * bandera que indica si se coloco un menu o no.
     */
    bool haymenu;
    /**
     * un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Skin* skin;
    /**
     * un apuntador al manejador de escena del juego
     */
    scene::ISceneManager* smgr;
    /**
     * un apuntador al manejador de GUI de irrlicht
     */
    gui::IGUIEnvironment* env;
    /**
     * La escala que tendrá en el juego;
     */
    core::vector3df escala;
    /**
     * Nodo del que seran hijos las piezas del menú.
     */
    scene::ISceneNode *parent;

};
}
#endif	/* _MenuHPP */

