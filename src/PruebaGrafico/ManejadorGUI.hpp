

#ifndef _MANEJADORGUI_HPP
#define	_MANEJADORGUI_HPP
#include<irrlicht/irrlicht.h>
#include<Grafico/Skin.hpp>
#include<Grafico/Tablero.hpp>
#include"Menu.hpp"
#include"GUI.hpp"
#include<string>
using namespace irr;
/**
 * clase que se encarga de todos los objetos de la GUI
 */
class ManejadorGUI {
public:
    /**
     *Constructor que  inicializa los atributos de la calse
     * @param smgr un apuntador al manejador de la escena del Juego
     * @param env un apuntador al manejador de GUI del juego
     * @param t Un apuntador a un objeto de la Clase Grafico::Tablero
     * @param skin  un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin);
    /**
     *Constructor copia de la clase ManejadorGUI
     * @param orig un objeto de la clase ManejadorGUI
     */
    ManejadorGUI(const ManejadorGUI& orig);
    /**
     *Destructor
     */
    virtual ~ManejadorGUI();
    /**
     * Elimina de la escena el menú principal del juego
     */
    void dropMenu();
    /**
     * Manda a llamar al metodo de la clase GUI
     * @as GUI::MsgBox
     */
    void MsgBox(const char* msg);
    /**
     *Método que se encarga de lanzar al selector de los agentes
     * @as GUI::AgntVSAgnt
     * @as GUI::AgntVSMkn
     * @param ambos si se van a seleccionar ambos agentes o no.
     */
    void AgntVSAgnt(bool ambos);
    /**
     *manda a llamar al metodo de la clase GUI
     * @as GUI::getPath()
     * @return un string con el path del archivo seleccionado
     */
     std::string getPath();
     /**
      *abrimos una caja de "OpenFile"
      * @as GUI:: OpenFileDialog
      */
     void  OpenFileDialog();
     /**
      *Se coloca el menú principal en le Juego
      */
     void setMenu();
     /**
      *Cambia el texto en los botones de la ventana de seleccion de agentes
      * @param num numero de boton del agente
      * @param text texto a colocar
      */
     void SetTextBtnAngt(int num, std::string text);
     /**
      *Método encargado aumentar o disminuir la escala del menu
      * @param x Un entero que representa la escala que tendrá en el eje de las X
      * @param y Un entero que representa la escala que tendrá en el eje de las Y
      * @param z Un entero que representa la escala que tendrá en el eje de las Z
      */
     void setEscala(int x,int y,int z);
     /**
      *Coloca el menu de la partida
      * @as GUI::setBotonesPartida
      */
     void setMenuPartida();
private:
    /**
     * un apuntador e un objeto de la Clase Menu
     */
    Menu* menu;
    /**
     * Un apuntador a un objeto de la clase GUI
     */
    GUI* Gui;
    /**
     * Un apuntador a un objeto de la clase Grafico::Tablero
     */
    Grafico::Tablero* t;

};

#endif	/* _MANEJADORGUI_HPP */

