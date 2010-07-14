#include "ManejadorOpciones.hpp"
#include <iostream>

ManejadorOpciones::ManejadorOpciones(int argc, char *argv[])
{
    namespace po = boost::program_options;
    using namespace std;
    
    this->description = new po::options_description("Opciones disponibles:");
    this->description->add_options()
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

    po::parsed_options parsed =
            po::command_line_parser(argc, argv).options(*(this->description))
                                    .allow_unregistered().run();

    po::store(parsed, this->vm);
    po::notify(vm);

    //Si se puso "-h" u opciones inválidas mostramos la ayuda y nos salimos! :D
    if(this->vm.count("help") > 0 || 
            !po::collect_unrecognized(parsed.options, po::include_positional).empty())
    {
        cout << '\n' << *this->description << endl;
        exit(0);
    }

    this->video_mode = this->vm.count("texto")? "NULL": "AUTO";
}

ManejadorOpciones::ManejadorOpciones(const ManejadorOpciones& orig)
{
}

ManejadorOpciones::~ManejadorOpciones()
{
    delete this->description;
}

const std::string & ManejadorOpciones::getVideoMode()
{
    return this->video_mode;
}

std::string ManejadorOpciones::getAgentePath(int num)
{
    using namespace std;
    string path = "";
    vector<string> path_vect;

    if( !this->vm.count("agentes"))
    {
        return path;
    }

    path_vect = vm["agentes"].as<vector<string> >();
    if(path_vect.empty())
    {
        return path;
    }

    return path_vect.at(num);
}

bool ManejadorOpciones::isFullScreen()
{
    return this->vm.count("fullscreen") > 0;
}

int ManejadorOpciones::getVelocidad()
{
    return this->vm["velocidad"].as<int>();
}