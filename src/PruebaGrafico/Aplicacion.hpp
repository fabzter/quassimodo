

#ifndef _APLICACION_HPP
#define	_APLICACION_HPP
#include<irrlicht/irrlicht.h>
#include<string>
#include"Video.hpp"
#include"Juego.hpp"
#include"EventReceiver.hpp"
using namespace irr;
/**
 *Esta clase es la encargada de manejar a Toda la aplicación
 * inicializar los motores graficos, el recibidor de evenos, selecciona el driver de video
 * y del LOOP proncipal del juego
 */
class Aplicacion {
public:
   /**
    *Constructor de la clase aplicacion, inicializa todo segun los argumentos dados
    * @param pathJ1 el path del agente del jugador 1 (se tienen que dar ambos paths para que el juego inicie
    * directamente a la partida, sin mostrar el menú)
    * @param pathj2 el path del agente del jugador 2
    * @param video Driver de video a Seleccionar, si se quiere ocuper un driver especifico OPENGL, DIRECT9, DIRECT8 o NULL.
    * La opcion de NULL indica que no se mostraran graficos en 3D y que solo la partida se mostrara en consola
    * @param fullscreen si se quiere que la apicacion se muestre en fullscreen o no.
    * @param Vel velocidad de la animacion de la partida,
    */
    Aplicacion(std::string pathJ1,std::string pathj2,std::string video="AUTO",bool fullscreen=false, int Vel=5);
    /**
     *Constructor copia de la clase Aplicación
     * @param orig un objeto de la clase aplicacion
     */
    Aplicacion(const Aplicacion& orig);
    /**
     * Destructor
     */
    virtual ~Aplicacion();

    /**
     *Inicia el Loop principal de la aplicación
     */
    void run();
private:
    /**
     * Un objeto de la clase video encargado de Seleccionar el driver de video
     */
    Video *Dvideo;
    /**
     * Un objeto de la clase Juego que es el nucleo de la aplicación
     */
    Juego *juego;
    /**
     * El recibidor de eventos
     */
    EventReceiver *eventos;
    /**
     * La parte masimportande del motor grafico, con este objeto
     * tendremos acceso a todo el motor de irrlicht
     */
    IrrlichtDevice* device;
    /**
     * interfaz al driver de video, el cual permite realizar funciones en 2D y 3D.
     */
    video::IVideoDriver* Vdriver;
    /**
     * un apuntador al manejador de escena del juego
     */
    scene::ISceneManager* smgr;
    /**
     * un apuntador al creador de GUI que proporciona irrlicht
     */
    gui::IGUIEnvironment* env;


};

#endif	/* _APLICACION_HPP */