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
            ("skin.modelos.tablero", 
            po::value<string>()->default_value(string("conf/skin_default/Tablero.3ds")),
                                        "Path relativo al modelo del tablero.")
            ;

    //Parseamos la linea de comando segun las opciones de linea de comando.
    po::parsed_options parsedCommandLine =
            po::command_line_parser(argc, argv).options(*(this->descriptionCommandLine))
                                    .allow_unregistered().run();

    //Parseamos el archivo de configuracion según las opciones del archivo de conf
    std::ifstream ifs("conf/opciones.conf");
    po::parsed_options parsedConfigFile =
            po::parse_config_file(ifs, *(this->descriptionConfigFile), true);

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

std::string Opciones::ManejadorOpciones::getTableroModeloPath()
{
    return this->vm["skin.modelos.tablero"].as<std::string>();
}

bool Opciones::ManejadorOpciones::isFullScreen()
{
    return this->vm.count("fullscreen") > 0;
}

int Opciones::ManejadorOpciones::getVelocidad()
{
    return this->vm["velocidad"].as<int>();
}