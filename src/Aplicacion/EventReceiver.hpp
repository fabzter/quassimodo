
#ifndef _EVENTRECEIVER_HPP
#define	_EVENTRECEIVER_HPP
#include<irrlicht.h>



 /**
     * Una estructura en la cual almacenamos el estado del raton contiene.
     * Posicion: un vector de la posición en la cual se encuentra el raton.
     *LeftButtonDown: un bool el cua indica si el botón izquierdo del raton, fue presionado.
     */
     struct SMouseState
	{
		irr::core::position2di Posicion;
		bool LeftButtonDown;
		//SMouseState() : LeftButtonDown(false) { }
	};

/**
 * Clase encargada de Recibir los Eventos del Teclado y del Mouse para que despues sean manejados.
 * hereda de la clase IEventREceiver de irrlicht
 */
class Aplicacion;
class EventReceiver  : public irr::IEventReceiver {

public:
    /**
     *Constructor de la clase EventReceiver, inicializa el elstado de todas las teclas en false
     */
    EventReceiver(Aplicacion* app);
    /**
     *Constructor copia de la clase EventReceiver
     * @param orig un objeto de la clase EventReceiver
     */
    EventReceiver(const EventReceiver& orig);
    /**
     *Destructor de la clase EventReceiver
     */
    virtual ~EventReceiver();
    /**
     *Método que se llama automaticamente cada vez que un evento es disparado.
     * aqui es donde se distingue que tipo de evento fue, si un evento del teclado o del mouse.
     * @param event obleto de la clase SEvent del namespace irr.
     * @return false si no es un evento q nos interesa.
     */
    virtual bool OnEvent(const irr::SEvent& event);
     /**
      *Regresa el estado del mouse
      * @return una referecia al struct SMouseState
      */
    const SMouseState& GetMouseState(void) const;
    /**
     *regresa si una tecla del teclado fue presionada.
     * @param keyCode la tecla de la cual se quiere saber si fue presionada o no.
     * @return true si la tecla fue presionada, false en otro caso.
     */

    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const;


private:
        /**
         * Una estructura de tipo SMouseState q nos proporcionara datos del mouse.
         */
       SMouseState MouseState;
       /**
        * un arrego de bool en el cual se almacena el estado de todas las teclas del teclado.
        */
	bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
        /**
         * Si el click se le dio a un botón éste étodo checa a cual boton fue y realiza
         * la acción correspondiente.
         * @param id id del elemento al que se le dio click
         */
        void Click_a_Boton(irr::s32 id);
        /**
         * Cuando el Jugadoracaba de realizar la jugada, se arma la jugada que pidio
         */
        void ArmaJugada();
        /**
         * Un apuntador Nucleo de la aplicacion (la clase Juego)
         */
        Aplicacion* app;
        /**
         * un entero en donde almacenamos el numero del agente elegido
         */
        int noA;
        /**
         * Una bandera que indica si la partida ha iniciado o no.
         */
        bool piniciada;
        bool AmbosHumanos;
        bool mover;
        bool barrera;
        bool barr_norte;
        irr::core::position2d<irr::s32> celda;

};


#endif	/* _EVENTRECEIVER_HPP */

