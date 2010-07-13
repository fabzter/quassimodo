

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
     * @param grafico indica si la gui sera modo grfico o solo mensajes en consola.
     */
    ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin,bool grafico=true);
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
     */
    void AgntVSAgnt(bool ambos);
    /**
     *manda a llamar al metodo de la clase GUI
     * @as GUI::getPath()
     * @return un string con el path del archivo seleccionado
     * @param grafico si se va a pedir desde modo grafico o modo consola el path
     * @param noA numero del agente a pedir,se ignora en modo 3D
     */
     std::string getPath(bool grafico=true,int noA=1);
     /**
      *abrimos una caja de "OpenFile"
      * @as GUI:: OpenFileDialog
      */
     void  OpenFileDialog();
     /**
      *Se coloca el menú principal del Juego
      *@param grafico indica si el menu estara en 3D o en consola.
      * @return  un caracter que representa la opcion seleccionada en modo consola, o un caracter cualquiera si fue ejecutado en modo grafico
      */
     char setMenu(bool grafico=true);
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
     /**
      *Muestra los creditos en la aplicacion. ya se a en modo consola o grafico.
      * @param grafico si se va mostrar en modo consola o modo grafico.
      */
     void creditos(bool grafico);
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

private:

    /**
     *Manda un mensaje en consola 
     * @param msg mensaje a enviar.
     */
    void MsgBoxConsola(const char* msg);
    /**
     *Pide el path del agente desde modo consola
     * @param noA numero del agente que se va a pedir
     * @return un string que contiene la direccion del archivo del agente.
     */
    std::string pideAgente(int noA=1);

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

