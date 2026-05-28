

#ifndef _GUI_HPP
#define	_GUI_HPP
#include <irrlicht.h>
#include"Skin.hpp"
#include"Enums.hpp"
#include<iostream>
#include<vector>
using namespace irr;
namespace Grafico{
/**
 * La clase GUI se encarga de ver todo lo relacionado con la interaccion con el usuario, excepto los botones del menú principal.
 * de las cosas que se encarga son de cuadros de dialogo, de open file dialogs etc.
 */
class GUI {
public:
    /**
     *Constructor de la clase GUI reserva memoria para los vectores.
     * @param smgr un apuntador al manejador de escena del Juego
     * @param env Un apuntador al manejador de GUI del juego
     * @param Skin un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Skin* Skin);
    /**
     *Constructor copia de la clase GUI
     * @param orig
     */
    GUI(const GUI& orig);
    /**
     *Destructor de la Clase GUI
     */
    virtual ~GUI();
    /**
     *Un msgBox  con el que solo tienes la opcion de Aceptar y desde aqui no se maneja
     * el evento de darle click al boton
     * @param msg una cadena de caracteres en la cual contiene el mensaje que queremos enviar
     *  @param idMsg id de la caja de dialogo para indicar si es un mensaje de error de informacion etc
     */
     void MsgBox(const char* msg ,GUI_BOTONES_OK idMsg);
     /**
      *Lanza el cuadro selector de los dos agentes.
      */
     void AgntVSAgnt();
     /**
      *Lanza el cuadro selector del agente, en este caso solo se elige uno y el otro ya esta fijo
      */
     void AgntVSMkn();
     /**
      *Obtenemos el path del agente seleccionado
      * @return un string el cual contiene el path deseado
      */
     std::string getPath();
     /**
      *lanza la caja de dialogo en la cual seleccionamos los agentes
      */
     void  OpenFileDialog();
     /**
      * elimina de la escena el cuadro selector de agentes
      */
     void dropAvsA();
     /**
      *Cambia el texto a los botones de los agentes segun el archivo seleccionado
      * @param num numero de boton del agente a cambiar el texto
      * @param text Texto a ser colocado en el Boton
      */
     void setTextAgnt(int num,const char* text);
     /**
      *Coloca los botones de la partida que son los de las vistas, salir al menú y pausa.
      */
     void setBotonesPartida();
     /**
     *Elimina nos botones de la partida  (botones de las vistas, pausa y salir al menu)
     */
    void dropBotonesPartida();
         /**
      * Cambia el boton de pausa, por el de continua o viceversa, y muestra o quita
      * un mensaje de PAUSA en la interfaz.
      * @param pausa bandera que indica que se hará si poner la pausa o quitarla.
      */
     void Pausar(bool pausa);
     /**
      * Coloca la imagen de los creditos en la escena.
      */
     void setCreditos();
     /**
      * Elimina la imagen de los creditos de la escena.
      */
     void dropCreditos();

private:
    /**
     * Coloca el skin de la GUI
     */
    void setSkin();
    /**
     *Metodo el cual dibuja el cuadro de seleccionar agentes.
     * este metodo es llamado por los dos descritos antes
     * @as AgntVSAgnt
     * @as AgntVSMkn
     * @param ambos bandera que indica si se quiere que se dibujen normal los 2 botones o solo 1
     */
    void dibujaSelector(bool ambos);
    /**
     *Método en el cual pasamos de  una cadena de char a wchar_t
     * @param m cadena de wchar_t en donde se almacenara el mensaje
     * @param msg cadena de char de la que queremos extraer el mensaje
     */
    void charTowchar(wchar_t m[],const char* msg);
    /**
     * Coloca el texto de PAUSA en el centro de la escena
     */
    void setTextPausa();
    /**
     * Elimina el el texto de PAUSA del centro de la escena
     */
    void dropTextPausa();
    
    /**
     * Un apuntador al manejador de la escena del juego
     */
    scene::ISceneManager* smgr;
    /**
     * Un apuntador al manejador de GUI del juego
     */
    gui::IGUIEnvironment* env;
    /**
     * Un apuntador a una ventana  de "Openfile" la cual nos serviar para que una vez presionado OK
     * podamos extraer el path del archivo seleccionado
     */
    gui::IGUIFileOpenDialog *op;
    /**
     * Un apuntador a la ventana de selector de Agentes
     */
    gui::IGUIWindow* AvsA;
    /**
     * Un apuntador a un objeto de la clase Grafico::Skin del cual extraemos las tipografias
     */
    Skin* skin;
    /**
     * Vector de botones en los que seleccionamos los agentes
     */
    std::vector<gui::IGUIButton *> botonAgente;
    /**
     * Vector de botones que son mostrados en la partida en curso
     */
    std::vector<gui::IGUIButton *> botonPartida;
    /**
     * bandera que indica si se colocaron los botones de la partida
     */
    bool botonesPartida;
    /**
     * nodo texto que se coloca en la escena al presionarse el botón de pausa
     */
    gui::IGUIStaticText* T_Pausa;
    /**
     * Imagen en la que se presenta la pantalla de creditos.
     */
    gui::IGUIImage* creditos;
};
}
#endif	/* _GUI_HPP */

