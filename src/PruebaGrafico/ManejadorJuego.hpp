
#ifndef _MANEJADORJUEGO_HPP
#define	_MANEJADORJUEGO_HPP

#include "ManejadorGUI.hpp"
#include "Partida.hpp"
#include "AnimacionEnd.hpp"
#include"tokayoCamera.hpp"
#include <Grafico/Skin.hpp>
#include<Reglas/Excepciones.hpp>
#include<Scripting/Excepciones.hpp>
#include<string.h>
#include <string>

using namespace irr;
/**
 *Clase encargada de controlar todo el juego, desde aqui se puede iniciar una nueva partida,
 * eliminar los menus o volver a ellos. etc.
 */
class ManejadorJuego {
public:
    /**
     *Constructor de la clase, se encarga de inicializar los atributos y colocar el menu princpal
     * @param smgr un apuntador al manejador de escena del juego
     * @param env un apuntador al creador de GUI que proporciona irrlicht
     */
    ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    /**
     * Constructor copia de la clase
     *@param orig un objeto de la clase ManejadorJuego
     */
    ManejadorJuego(const ManejadorJuego& orig);
    /**
     * Destructor
     */
    virtual ~ManejadorJuego();
    /**
     * Coloca el Menu en la escena
     */
    void setMenu();
    /**
     * elimina el menú principal y si se han seleccionado los agentes inicia la partida
     */
    void setPartida();
    /**
     *Método encargado aumentar o disminuir la escala del juego
     * @param x Un entero que representa la escala que tendrá en el eje de las X
     * @param y Un entero que representa la escala que tendrá en el eje de las Y
     * @param z Un entero que representa la escala que tendrá en el eje de las Z
     */
    void setEscala(int x,int y,int z);
    /**
     *Matodo encargado de pedir la siguiente juega al agente en turno
     * @return true si la partida sigue en curso, false en caso contrario
     */
    bool SiguienteJugada();
    /**
     * Indica si hay un ganador o no
     * @return true si hay ganador, false en caso contrario
     */
    bool hayGanador();
    /**
     *Obtiene el objeto Manejador de la GUI
     * @return un apuntador a un objeto de la clase ManejadorGUI
     */
    ManejadorGUI* getManejadorGUI();
    /**
     *Coloca el path del agente para ser cargado posteriormente
     * @param Agente path en donde se encuaentra el Agente
     * @param noAgente numero del Agente
     */
    void setAgente(std::string Agente,int noAgente);
    /**
     * Elimina los agentes almacenados.
     */
    void clearAgentes();
    /**
     * Inicia una partida Rápida, este metodo generalmente es como un demo
     * de la aplicación
     */
    void quick();
    /**
     * Imprime los valores de la camara, como son posicion, objetivo y rotación
     */
    void printCam();
    /**
     *Cambia el texto en el boton selector de agentes
     * @param bAgente numero de boton del agente a cambiar
     */
    void CambiaTextoAgnt(int bAgente);
    /**
     * Cambia la vista de la camara en la partida
     * @param vista numero de vista a colocar
     */
    void cambiaVistaJuego(int vista);
    void aumentaAngulo(bool inner);
    void disminuyeAngulo(bool inner);
private:
    /**
     *Metodo que separa el nombre del archivo del path del agente
     * @param str path en donde se encuentra el agente
     * @return string con el nombre del archivo que contiene al agente
     */
    std::string SplitNombre(std::string str);
    /**
     *Coloca la camara de la partida asi como tambien el tope de esta
     * para que no pase mas abajo del terreno del piso
     */
    void setCamJuego();
    /**
     * Coloca la camara del Menu prinipal
     */
    void setCamMenu();
    /**
     *Coloca el terreno y el skydome del ambiente
     */
    void setSkinAmbiente();
    /**
     *Elimina de la escena el terreno y el skydome
     */
    void dropSkinAmbiente();
    /**
     * Un apuntador al manejador de la escena del juego
     */
    scene::ISceneManager* smgr;
    /**
     * un apuntador al creador de GUI que proporciona irrlicht
     */
    gui::IGUIEnvironment* env;
    /**
     * Vector en donde se almacena el path de los agentes
     */
    std::vector<std::string> Agentes;
    /**
     * bandera que indica si se han cargado agentes o no
     */
    bool hayagente;
    /**
     * Un apuntador a un objeto de la clase ManejadorGUI el cual maneja la interaccion con el usuario
     */
    ManejadorGUI *mgui;
    /**
     * Un apuntador de la clase Partida que es la que se encarga de mantener la partida
     */
    Partida *partida;
    /**
     * un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Grafico::Skin* skin;
    /***/
    AnimacionEnd *aniend;
    /**
     * un apuntador a un nodo de terreno que es donde se carga y amacena nuestro piso del juego
     */
    scene::ITerrainSceneNode* terrain ;
    /**
     * Un apunador a un nodo escena que es donde se carga y almacena nuestro cielo
     */
    scene::ISceneNode* skydome;
    /**
     * Un nodo camara que es la camara que se maneja durante el juego y el menú
     */
    scene::ICameraSceneNode* cam;
    scene::ILightSceneNode* luna;

};

#endif	/* _MANEJADORJUEGO_HPP */

