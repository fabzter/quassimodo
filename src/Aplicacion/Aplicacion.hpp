

#ifndef _APLICACION_HPP
#define	_APLICACION_HPP
#include<irrlicht/irrlicht.h>
#include<string>
#include<Grafico/Video.hpp>
#include"ManejadorJuego.hpp"
#include"EventReceiver.hpp"
#include<Opciones/ManejadorOpciones.hpp>
#include<iostream>
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
    * @param opciones Un objeto que sirve para manejar las opciones de creacion de la aplicacion
    */
    Aplicacion(Opciones::ManejadorOpciones &opciones);
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
    /**
     * Regresa un apuntador al manejador del juego.
     * @return  un apuntador al manejador del juego.
     */
    ManejadorJuego* getManJuego();
    /**
     * Relinicializa el manejador del juego
     */
    void nuevoJuego();
private:
    /**
     * Loop principal de la aplicacion si solo se ejecuta en modo consola
     */
    void loopConsola();
    /**
     *Loop principal de la aplicacion si se ejecuta en modo 3D
     */
    void loopGrafico();
    /**
     *Maneja la opcion seleccionada del menu de consola.
     * @param op un caracter que representa la opcion
     * @return true si la opcion seleccionada inica partida, false en caso contrario
     */
    bool seleccionaOpcion(char op);
    /**
     * Prepara, la palicacion para iinciar una partida de inmediato, con los argumentos dados al 
     * ejecutable.
     * @pathJ1 el path del agente del jugador 1
     * @param pathj2 el path del agente del jugador 2
     */
    void quick(std::string pathJ1,std::string pathj2);
    /**
     * Pone las variables en NULL
     */
    void setNull();
    /**
     * Limpia las variables de irrlicht (libera la memoria del GPU) y elimina los objetos
     * que se esten utilizando por el engine.
     */
    void dropIrrlicht();

    /**
     * Un objeto de la clase video encargado de Seleccionar el driver de video
     */
    Grafico::Video *Dvideo;
    /**
     * Un objeto de la clase Juego que es el nucleo de la aplicación
     */
    ManejadorJuego *juego;
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
    /**
     * indica si la partida sera en modo grafico o no
     */
    bool grafico;
    /**
     * un apuntador a un objeto de la clase Grafico::skin que tiene los modelos y las texturas del programa
     */
    Grafico::Skin* skin;
    /**
     * Indica si se iniciará de inmediato la partida con los agentes pasados como argumentos
     * al ejecutable.
     */
    bool p_rapida;
    /**
     * velocidad de la animacion en modo gráfico
     */
    int velAnimacion;


};

#endif	/* _APLICACION_HPP */