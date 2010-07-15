#ifndef MANEJADOROPCIONES_HPP
#define	MANEJADOROPCIONES_HPP
#include <boost/program_options.hpp>
#include <string>
#include <vector>
namespace Opciones{
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
    boost::program_options::options_description* description;
    boost::program_options::variables_map vm;
    std::string video_mode;
    int velocidad;
};
}

#endif	/* MANEJADOROPCIONES_HPP */

