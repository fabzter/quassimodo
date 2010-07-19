#include "ManejadorOpciones.hpp"
#include <iostream>
#include <fstream>

Opciones::ManejadorOpciones::ManejadorOpciones(int argc, char *argv[])
{
    namespace po = boost::program_options;
    using namespace std;

    this->descriptionCommandLine = this->descriptionConfigFile = NULL;

    //Describimos las opciones de linea de commandos
    this->descriptionCommandLine = 
            new po::options_description("Opciones disponibles como argumentos a"
                                                            " la aplicación:");
    this->descriptionCommandLine->add_options()
            ("help,h", "Muestra mensaje de ayuda.")
            ("texto,t", "Inicia la partida en modo de texto.")
            ("fullscreen,f", "Inicia la partida en modo fullscreen, se ignora"
                             "si se usa modo de texto.")
            ("agentes,a", po::value< vector<string> >()->multitoken(),
                                "Especifica el path de los dos agentes a usar.\n"
                                "-a /path/a/agente1.py /path/a/agente2.py")
            ("velocidad,v", po::value<int>(&(this->velocidad))->default_value(250),
                            "Especifica la velocidad de la animación de los "
                            "agentes. Default = 250")
            ;

    //Describimos las opciones de archivo de configuración
    this->descriptionConfigFile = 
            new po::options_description("Opciones usadas en el archivo de "
                                                            "configuración");
    this->descriptionConfigFile->add_options()
            //modelos
            ("skin.modelos.tablero", 
            po::value<string>()->default_value(string("conf/skin_default/Tablero.3ds")),
                                        "Path relativo al modelo del tablero.")
            ("skin.modelos.jugador_1",
            po::value<string>()->default_value(string("conf/skin_default/Mono.3ds")),
                                        "Path relativo al modelo del Jugador 1.")
            ("skin.modelos.jugador_2",
            po::value<string>()->default_value(string("conf/skin_default/MonoBlanco.3ds")),
                                        "Path relativo al modelo del Jugador 2.")
            ("skin.modelos.jugador_sombra",
            po::value<string>()->default_value(string("conf/skin_default/MonoSombra.3ds")),
                                        "Path relativo al modelo de la sombra de"
                                        " los Jugadores.")
            ("skin.modelos.antorcha",
            po::value<string>()->default_value(string("conf/skin_default/Lumbrera.3ds")),
                                        "Path relativo al modelo de la antorcha.")
            ("skin.modelos.barrera",
            po::value<string>()->default_value(string("conf/skin_default/Barrera.3ds")),
                                        "Path relativo al modelo de las barreras.")
            ("skin.modelos.celda",
            po::value<string>()->default_value(string("conf/skin_default/Celda.3ds")),
                                        "Path relativo al modelo del celda.")
            //texturas
            ("skin.texturas.tablero",
            po::value<string>()->default_value(string("conf/skin_default/Moss0138_10_S.jpg")),
                                        "Path relativo a la textura del tablero.")
            ("skin.texturas.antorcha",
            po::value<string>()->default_value(string("conf/skin_default/fire.bmp")),
                                        "Path relativo a la textura de la antorcha.")
            ("skin.texturas.barrera",
            po::value<string>()->default_value(string("conf/skin_default/BrickOldDirty0078_S.jpg")),
                                        "Path relativo al modelo la textura de las barreras.")
            ("skin.texturas.celda",
            po::value<string>()->default_value(string("conf/skin_default/Moss0138_2_S.jpg")),
                                        "Path relativo a la textura de las celdas.")
            ("skin.texturas.terreno",
            po::value<string>()->default_value(string("conf/skin_default/piso3_TX.jpg")),
                                    "Path relativo a la textura del terreno.")
            ("skin.texturas.terreno_height",
            po::value<string>()->default_value(string("conf/skin_default/piso3_HM.bmp")),
                            "Path relativo al archivo de relieve del terreno.")
            ("skin.texturas.skydome",
            po::value<string>()->default_value(string("conf/skin_default/3.tree.skydome.png")),
                                    "Path relativo a la textura del skydome.")
            //fonts
            ("gui.fonts.default",
            po::value<string>()->default_value(string("conf/gui_default/defaultfont2.png")),
                                            "Path relativo a la font default.")
            ("gui.fonts.menu_button",
            po::value<string>()->default_value(string("conf/gui_default/bigfont.png")),
                                            "Path relativo a la font del menú.")
            ("gui.fonts.tooltip",
            po::value<string>()->default_value(string("conf/gui_default/tooltipfont.png")),
                                            "Path relativo a la font del tooltip.")
            ("gui.fonts.buttons",
            po::value<string>()->default_value(string("conf/gui_default/botonFont.png")),
                                            "Path relativo a la font de los botones.")
            ("gui.fonts.window",
            po::value<string>()->default_value(string("conf/gui_default/windowfont.png")),
                                            "Path relativo a la font de la ventana.")
            //botones
            ("gui.botones.vuelta_1",
            po::value<string>()->default_value(string("conf/gui_default/boton4_2-1_vuelta.png")),
                            "Path relativo a la imagen del botón de vuelta 1.")
            ("gui.botones.vuelta_2",
            po::value<string>()->default_value(string("conf/gui_default/boton4_2-2_vuelta.png")),
                            "Path relativo a la imagen del botón de vuelta 2.")
            ("gui.botones.vuelta_3",
            po::value<string>()->default_value(string("conf/gui_default/boton5_2-2_vuelta.png")),
                            "Path relativo a la imagen del botón de vuelta 3.")
            ("gui.botones.frente",
            po::value<string>()->default_value(string("conf/gui_default/boton5_2-1_frente.png")),
                            "Path relativo a la imagen del botón de vuelta al frente.")
            ("gui.botones.pausa_1",
            po::value<string>()->default_value(string("conf/gui_default/boton1_2_pausa.png")),
                            "Path relativo a la imagen del botón de pausa 1.")
            ("gui.botones.pausa_2",
            po::value<string>()->default_value(string("conf/gui_default/boton2_2_pausa.png")),
                            "Path relativo a la imagen del botón de pausa 2.")
            ("gui.botones.menu",
            po::value<string>()->default_value(string("conf/gui_default/boton3_2_menu.png")),
                            "Path relativo a la imagen del botón de volver a menú.")
            //cfg
            ("gui.gui_cfg",
            po::value<string>()->default_value(string("conf/gui_default/boton3_2_menu.png")),
            "Path relativo al archivo de configuración de la gui de irrlicht.")
            ;

    //Parseamos la linea de comando segun las opciones de linea de comando.
    po::parsed_options parsedCommandLine =
            po::command_line_parser(argc, argv).options(*(this->descriptionCommandLine))
                                    .allow_unregistered().run();

    //Parseamos el archivo de configuracion según las opciones del archivo de conf
    std::ifstream ifs("conf/opciones.conf");
    po::parsed_options parsedConfigFile =
            po::parse_config_file(ifs, *(this->descriptionConfigFile), false);

    po::store(parsedCommandLine, this->vm);
    po::store(parsedConfigFile, this->vm);
    po::notify(vm);

    //Si se puso "-h" u opciones inválidas mostramos la ayuda y nos salimos! :D
    if(this->vm.count("help") > 0 || 
            !po::collect_unrecognized(parsedCommandLine.options, po::include_positional).empty())
    {
        cout << '\n' << *this->descriptionCommandLine << endl;
        exit(0);
    }

    this->video_mode = this->vm.count("texto")? "NULL": "AUTO";
}

Opciones::ManejadorOpciones::ManejadorOpciones(const ManejadorOpciones& orig)
{
}

Opciones::ManejadorOpciones::~ManejadorOpciones()
{
    delete this->descriptionCommandLine;
    delete this->descriptionConfigFile;
}

const std::string & Opciones::ManejadorOpciones::getVideoMode()
{
    return this->video_mode;
}

std::string Opciones::ManejadorOpciones::getAgentePath(int num)
{
    using namespace std;
    string path = "";
    vector<string> path_vect;

    if( !this->vm.count("agentes"))
    {
        return path;
    }

    path_vect = this->vm["agentes"].as<vector<string> >();
    if(path_vect.empty())
    {
        return path;
    }

    return path_vect.at(num);
}

/*GETTERS DE LOS MODELOS*/

std::string Opciones::ManejadorOpciones::getTableroModeloPath()
{
    return this->vm["skin.modelos.tablero"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getJugador1ModeloPath()
{
    return this->vm["skin.modelos.jugador_1"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getJugador2ModeloPath()
{
    return this->vm["skin.modelos.jugador_2"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getJugadorSombraModeloPath()
{
    return this->vm["skin.modelos.jugador_sombra"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getAntorchaModeloPath()
{
    return this->vm["skin.modelos.antorcha"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBarreraModeloPath()
{
    return this->vm["skin.modelos.barrera"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getCeldaModeloPath()
{
    return this->vm["skin.modelos.celda"].as<std::string>();
}

/*GETTERS DE LAS TEXTURAS*/

std::string Opciones::ManejadorOpciones::getTableroTexturaPath()
{
    return this->vm["skin.texturas.tablero"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getAntorchaTexturaPath()
{
    return this->vm["skin.texturas.antorcha"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBarreraTexturaPath()
{
    return this->vm["skin.texturas.barrera"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getCeldaTexturaPath()
{
    return this->vm["skin.texturas.celda"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getTerrenoTexturaPath()
{
    return this->vm["skin.texturas.terreno"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getTerrenoHeightTexturaPath()
{
    return this->vm["skin.texturas.terreno_height"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getSkydomeTexturaPath()
{
    return this->vm["skin.texturas.skydome"].as<std::string>();
}

/*GETTERS DE LAS FONTS*/

std::string Opciones::ManejadorOpciones::getDefaultFontPath()
{
    return this->vm["gui.fonts.default"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getMenuFontPath()
{
    return this->vm["gui.fonts.menu_button"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getTooltipFontPath()
{
    return this->vm["gui.fonts.tooltip"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getButtonFontPath()
{
    return this->vm["gui.fonts.buttons"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getWindowFontPath()
{
    return this->vm["gui.fonts.window"].as<std::string>();
}

/*GETTERS DE LOS BOTONES*/

std::string Opciones::ManejadorOpciones::getBotonVuelta1Path()
{
    return this->vm["gui.botones.vuelta_1"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonVuelta2Path()
{
    return this->vm["gui.botones.vuelta_2"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonVuelta3Path()
{
    return this->vm["gui.botones.vuelta_3"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonFrentePath()
{
    return this->vm["gui.botones.frente"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonPausa1Path()
{
    return this->vm["gui.botones.pausa_1"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonPausa2Path()
{
    return this->vm["gui.botones.pausa_2"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getBotonMenuPath()
{
    return this->vm["gui.botones.menu"].as<std::string>();
}

std::string Opciones::ManejadorOpciones::getGUIConfig()
{
    return this->vm["gui.gui_cfg"].as<std::string>();
}

bool Opciones::ManejadorOpciones::isFullScreen()
{
    return this->vm.count("fullscreen") > 0;
}

int Opciones::ManejadorOpciones::getVelocidad()
{
    return this->vm["velocidad"].as<int>();
}
