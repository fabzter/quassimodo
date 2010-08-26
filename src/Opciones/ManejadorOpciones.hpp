#ifndef MANEJADOROPCIONES_HPP
#define	MANEJADOROPCIONES_HPP
#include <boost/program_options.hpp>
#include <string>
#include <vector>

/**
 * Este namespace contiene las clases que se encargan del manejo de las opciones
 * del juego, tanto las recibidas por línea de comando, como las recibidas por
 * medio del archivo de configuración. 
 * 
 * La clase con la que se interactúa es la clase ManejadorOpciones.
 */
namespace Opciones
{
    /**
     * El punto principal de interacción para obtener las opciones del Juego.
     * Contiene un método para cada opción. :S
     */
class ManejadorOpciones
{
public:
    /**
     * El único constructor de esta clase que debe ser usado.
     * @param argc el número de argumentos pasados a la aplicación.
     * @param argv el array de parámetros pasados a la aplicación.
     */
    ManejadorOpciones(int argc, char *argv[]);
    ManejadorOpciones(const ManejadorOpciones& orig);
    virtual ~ManejadorOpciones();

    /**
     * Nos indica el tipo de presentación que se especificó en las opciones.
     * @return una referencia a la std::string que representa el modo de video
     * solocitado. (Puede contener "NULL" o "AUTO").
     */
    const std::string &getVideoMode();

    /**
     * Nos indica el path especificado desde línea de comandos para el agente
     * número "num". En caso de no haber sido especificado, regresa una string
     * vacía.
     * @param num el número del agente del que se quiere el path.
     * @return Una string que contiene el path del agente, o una string vacía.
     */
    std::string getAgentePath(int num);

    /* GETTERS PARA LOS MODELOS*/
    
    /**
     * Nos indica el Path al modelo del Tablero especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo del
     * Tablero.
     */
    std::string getTableroModeloPath();

    /**
     * Nos indica el Path al modelo del Jugador 1, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo del
     * Jugador 1.return this->vm["skin.modelos.jugador_2"].as<std::string>();
     */
    std::string getJugador1ModeloPath();

    /**
     * Nos indica el Path al modelo del Jugador 2, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo del
     * Jugador 2.
     */
    std::string getJugador2ModeloPath();

    /**
     * Nos indica el Path al modelo de las barrerasdel jugador 1, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * las barrerasdel jugador 1.
     */
    std::string getBarrera1ModeloPath();
    /**
     * Nos indica el Path al modelo de las barrerasdel jugador 2, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * las barrerasdel jugador 2.
     */
    std::string getBarrera2ModeloPath();


    /**
     * Nos indica el Path al modelo de las celdas, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * las celdas.
     */
    std::string getCeldaModeloPath();

    /*GETTERS PARA LAS TEXTURAS*/
    
    /**
     * Nos indica el Path a la textura del Tablero, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de textura del
     * Tablero.
     */
    std::string getTableroTexturaPath();
    /**
     * Nos indica el Path a la textura de la antorcha, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de textura de
     * la antorcha.
     */
    std::string getAntorchaTexturaPath();
    /**
     * Nos indica el Path a la textura de la barrera del jugador 1, especificado
     * en el archivo de configuración.
     * @return una string que contiene el path relativo al archivo de textura de
     * las barreras.
     */
    std::string getBarrera1TexturaPath();
    /**
     * Nos indica el Path a la textura de la barrera del jugador 2, especificado
     * en el archivo de configuración.
     * @return una string que contiene el path relativo al archivo de textura de
     * las barreras.
     */
    std::string getBarrera2TexturaPath();

    /**
     * Nos indica el Path a la textura de las celdas, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de textura de
     * las celdas.
     */
    std::string getCeldaTexturaPath();

    /**
     * Nos indica el Path a la textura del terreno, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de textura del
     * terreno.
     */
    std::string getTerrenoTexturaPath();

    /**
     * Nos indica el Path a la textura del skydome, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de textura del
     * skydome.
     */
    std::string getSkydomeTexturaPath();


    /*GETTERS PARA LAS FONTS*/

    /**
     * Nos indica el path a la font default, especificada en el archivo de
     * configuración.
     * @return una string con el Path relativo al archivo de font default.
     */
    std::string getDefaultFontPath();

    /**
     * Nos indica el path a la font del menú, especificada en el archivo de
     * configuración.
     * @return una string con el Path relativo al archivo de font del menú.
     */
    std::string getMenuFontPath();

    /**
     * Nos indica el path a la font default, especificada en el archivo de
     * configuración.
     * @return una string con el Path relativo al archivo de font default.
     */
    std::string getTooltipFontPath();

    /**
     * Nos indica el path a la font de los botones, especificada en el archivo de
     * configuración.
     * @return una string con el Path relativo al archivo de la font de los
     * botones.
     */
    std::string getButtonFontPath();

    /**
     * Nos indica el path a la font de la ventana, especificada en el archivo de
     * configuración.
     * @return una string con el Path relativo al archivo de la font de la
     * ventana.
     */
    std::string getWindowFontPath();

    /*GETTERS DE LOS BOTONES*/

    /**
     * Nos indica el Path a la imagen del primer botón de vuelta, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del primero botón de
     * vuelta.
     */
    std::string getBotonVuelta1Path();
    /**
     * Nos indica el Path a la imagen del primer botón de vuelta presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del primero botón de
     * vuelta presionado.
     */
    std::string getBotonVuelta1PresPath();

    /**
     * Nos indica el Path a la imagen del segundo botón de vuelta, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del segundo botón de
     * vuelta.
     */
    std::string getBotonVuelta2Path();
    /**
     * Nos indica el Path a la imagen del segundo botón de vuelta presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del segundo botón de
     * vuelta presionado.
     */
    std::string getBotonVuelta2PresPath();

    /**
     * Nos indica el Path a la imagen del tercer botón de vuelta, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del tercer botón de
     * vuelta.
     */
    std::string getBotonVuelta3Path();
    /**
     * Nos indica el Path a la imagen del tercer botón de vuelta presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del tercer botón de
     * vuelta presionado.
     */
    std::string getBotonVuelta3PresPath();

    /**
     * Nos indica el Path a la imagen del botón de vuelta al frente,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón de vuelta al
     * frente.
     */
    std::string getBotonFrentePath();
    /**
     * Nos indica el Path a la imagen del botón de vuelta al frente presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón de vuelta al
     * frente presionado.
     */
    std::string getBotonFrentePresPath();

    /**
     * Nos indica el Path a la imagen del primer botón de pausa, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del primer botón de
     * pausa.
     */
    std::string getBotonPausa1Path();
    /**
     * Nos indica el Path a la imagen del primer botón de pausa presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del primer botón de
     * pausa presionado.
     */
    std::string getBotonPausa1PresPath();

    /**
     * Nos indica el Path a la imagen del segundo botón de pausa, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del segundo botón de
     * pausa.
     */
    std::string getBotonPausa2Path();
    /**
     * Nos indica el Path a la imagen del segundo botón de pausa presionado,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del segundo botón de
     * pausa presionado.
     */
    std::string getBotonPausa2PresPath();

    /**
     * Nos indica el Path a la imagen del botón de regreso al menú, especificado
     * en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón de vuelta al
     * menú..
     */
    std::string getBotonMenuPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de regreso al menú,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón presionado 
     * de vuelta al menú..
     */
    std::string getBotonMenuPresPath();
    /**
     * Nos indica el Path a la imagen del botón de mover al pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * mover al pinguino
     */
    std::string getMoverPinguinoPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de mover al pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * mover al pinguino
     */
    std::string getMoverPinguinoPressPath();
      /**
     * Nos indica el Path a la imagen del botón de mover al robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * mover al pinguino
     */
    std::string getMoverRobotPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de mover al robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * mover al pinguino
     */
    std::string getMoverRobotPressPath();
        /**
     * Nos indica el Path a la imagen del botón de poner barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de poner barrera del pinguino
     */
    std::string getSetBarreraPinguinoPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de poner barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de poner barrera del pinguino
     */
    std::string getSetBarreraPinguinoPressPath();
      /**
     * Nos indica el Path a la imagen del botón de poner barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de poner barrera del pinguino
     */
    std::string getSetBarreraRobotPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de poner barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de poner barrera del pinguino
     */
    std::string getSetBarreraRobotPressPath();
    /**
     * Nos indica el Path a la imagen del botón de Girar al este la barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de Girar al este la barrera del pinguino
     */
    std::string getGiraEsteBarreraPinguinoPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de Girar al este la barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de Girar al este la barrera del pinguino
     */
    std::string getGiraEsteBarreraPinguinoPressPath();
      /**
     * Nos indica el Path a la imagen del botón de Girar al este la barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de Girar al este la barrera del pinguino
     */
    std::string getGiraEsteBarreraRobotPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de Girar al este la barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de Girar al este la barrera del pinguino
     */
    std::string getGiraEsteBarreraRobotPressPath();
               /**
     * Nos indica el Path a la imagen del botón de Girar al Norte la barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de Girar al Norte la barrera del pinguino
     */
    std::string getGiraNorteBarreraPinguinoPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de Girar al Norte la barrera del pinguino,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de Girar al Norte la barrera del pinguino
     */
    std::string getGiraNorteBarreraPinguinoPressPath();
      /**
     * Nos indica el Path a la imagen del botón de Girar al Norte la barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón
     * de Girar al Norte la barrera del pinguino
     */
    std::string getGiraNorteBarreraRobotPath();
    /**
     * Nos indica el Path a la imagen del botón presionado de Girar al Norte la barrera del robot,
     * especificado en el archivo de configuración.
     * @return una string con el path relativo al archivo del botón  presionado
     * de Girar al Norte la barrera del pinguino
     */
    std::string getGiraNorteBarreraRobotPressPath();


    /*Otros*/

    /**
     * Nos indica el path al archivo cfg de la skin de irrlicht.
     * @return una string con el path relativo al archivo de cfg del irrlicht.
     */
    std::string getGUIConfig();

    /**
     * Nos indica el path al archivo de la pantalla de créditos.
     * @return una string con el path relativo al archivo de la pantalla de
     * créditos.
     */
    std::string getCreditos();

    /**
     * Nos indica si se especificó la opción de fullscreen como argumente en la
     * línea de comandos.
     * @return true de haberse especificado, false de lo contrario.
     */
    bool isFullScreen();

    /**
     * Nos indica la velocidad que se específico en la línea de comandos.
     * @return la velocidad especificada con la opción "-v".
     */
    int getVelocidad();
private:
    boost::program_options::options_description* descriptionCommandLine;
    boost::program_options::options_description* descriptionConfigFile;
    boost::program_options::variables_map vm;
    std::string video_mode;
    int velocidad;
};
}

#endif	/* MANEJADOROPCIONES_HPP */

