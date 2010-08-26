#ifndef _MANEJADORGUI_HPP
#define	_MANEJADORGUI_HPP
#include<irrlicht/irrlicht.h>
#include"Skin.hpp"
#include"Tablero.hpp"
#include"Menu.hpp"
#include"GUI.hpp"
#include<string>
#include<iostream>
#include"Enums.hpp"
using namespace irr;
namespace Grafico{
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
     * Manda a llamar al metodo de la clase GUI o manda un mensaje en la consola
     * @as GUI::MsgBox
     * @param grafico si se mandara mensaje de consola o grafico
     */
    void MsgBox(const char* msg,bool grafico=true,GUI_BOTONES_OK idMsg=BOK_INFO);
    /**
     *Método que se encarga de lanzar al selector de los agentes
     * @as GUI::AgntVSAgnt
     * @as GUI::AgntVSMkn
     * @param ambos si se van a seleccionar ambos agentes o no.
     * @param lista liste de Agentes a seleccionar
     */
    void AgntVSAgnt(bool ambos,std::vector<std::string> lista);
     /**
      * regresa el Agente seleccionado en el combo Box
      * @return un string que contiene el nombre del agente.
      */
     std::string getAgenteSeleccionado();
     /**
      *Se coloca el menú principal del Juego
      */
     void setMenu();
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
     /**
      *Muestra los creditos en la aplicacion. ya se a en modo consola o grafico.
      */
     void creditos();
     /**
      *Borra la ventana de selector de agentes
      */
     void dropAvsA();
     /**
      *Elimina los botones de la partida en curso.
      */
     void dropBotonesPartida();
     /**
      * @as GUI::Pausar
      */
     void Pausar(bool pausa);
     /**
      * @as GUI::dropCreditos
      */
     void dropCreditos();
     /**
      * @as GUI::setBotonesJugador
      */
     void setBotonesJugador(int idjugador);
     /**
      * as GUI::dropBotonesJugador
      */
     void dropBotonesJugador();
     /**
      * as GUI::setBotonesBarrera
      */
     void setBotonesBarrera();
     /**
      * @as GUI::cambiaGiro
      */
     void cambiaGiro(bool este);
     /**
      * @as GUI::setAyuda
      */
     void setAyuda();
     /**
      * @as GUI::dropAyuda
      */
     void dropAyuda();

private:

    /**
     *Manda un mensaje en consola 
     * @param msg mensaje a enviar.
     */
    void MsgBoxConsola(const char* msg);

    /**
     * un apuntador e un objeto de la Clase Menu
     */
    Menu* menu;
    /**
     * Un apuntador a un objeto de la clase GUI
     */
    GUI* Gui;


};
}
#endif	/* _MANEJADORGUI_HPP */

