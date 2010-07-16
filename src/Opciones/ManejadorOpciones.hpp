#ifndef MANEJADOROPCIONES_HPP
#define	MANEJADOROPCIONES_HPP
#include <boost/program_options.hpp>
#include <string>
#include <vector>
namespace Opciones
{
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
     * Nos indica el Path al modelo de la sombra de los jugadores, especificado
     *  en el archivo de configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * la sombra de los Jugadores.
     */
    std::string getJugadorSombraModeloPath();

    /**
     * Nos indica el Path al modelo de la antorcha, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * la antorcha.
     */
    std::string getAntorchaModeloPath();

    /**
     * Nos indica el Path al modelo de las barreras, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * las barreras.
     */
    std::string getBarreraModeloPath();

    /**
     * Nos indica el Path al modelo de las celdas, especificado en el archivo de
     * configuración.
     * @return una string que contiene el path relativo al archivo de modelo de
     * las celdas.
     */
    std::string getCeldaModeloPath();

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

