#include "ModuloPython.hpp"

Scripting::ModuloPython::ModuloPython()
{
    this->esta_extraida_clase = this->esta_cargado = false;
    this->ayudante = NULL;
}

Scripting::ModuloPython::ModuloPython(const ModuloPython& orig)
{
}

Scripting::ModuloPython::~ModuloPython()
{
    if(this->ayudante != NULL)
        delete this->ayudante;
}

void Scripting::ModuloPython::cargar(std::string ruta, Reglas::Tablero &t)
{
    using namespace std;
    using namespace boost::python;
    
    if(this->esta_cargado)
        return;
    
    string file_name;
    string directories;
    //creamos el modulo
    dict locals;
    locals["ruta"] = ruta;
    try
    {
        //primero extraemos la ruta y el nombre del módulo a cargar
        this->modulo = import("__main__");
        this->namespace_modulo = this->modulo.attr("__dict__");
        directories = extract<string>( eval("os.path.dirname(ruta)",
                        this->namespace_modulo, locals) );
        file_name = extract<string>( eval("os.path.splitext(os.path.basename(ruta))[0]",
                        this->namespace_modulo, locals) );
        
        //agregamos la ruta del modulo al path de python
        locals["ruta"] = directories;
        exec("sys.path.append(ruta)\n", this->namespace_modulo, locals);
    }
    catch(error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    
    try
    {
        this->modulo = import(str(file_name));
    }
    catch(error_already_set& e)
    {
        throw ScriptMalo("Archivo no encontrado o con errores de sintaxis.");
    }

    try
    {
        //obtenemos el diccionario del namespace del modulo
        this->namespace_modulo = this->modulo.attr("__dict__");

        //exponemos el Tablero
        this->namespace_modulo["tablero"] = object(boost::python::ptr(&t));
        //y le quitamos el metodo moverJugador
        exec("tablero.moverJugador = None\n", this->namespace_modulo,
                                                this->namespace_modulo);
        //le quitamos setBarrera
        exec("tablero.setBarrera = None\n", this->namespace_modulo,
                                                this->namespace_modulo);
        //exponemos el ayudante
        this->ayudante = new Reglas::AyudanteDeAgente(t);
        this->namespace_modulo["ayudante"] =
                object(boost::python::ptr(this->ayudante));

        //ejecutamos el archivo
        object ignored = exec_file( str(ruta),
                                   this->namespace_modulo,
                                   this->namespace_modulo);
    }
    catch(error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }

    this->esta_cargado = true;
}

Reglas::Agente* Scripting::ModuloPython::getAgente()
{
    using namespace boost::python;
    if(!this->esta_cargado)
        throw ModuloNoCargado();

    Reglas::Agente *a = NULL;
    AgentePythonWrapper *aWrap = NULL;
    try
    {
        this->extraer_clase();

        this->instancias_clase.push_front( this->agente_clase() );
        
        a = extract<Reglas::Agente*>
                (this->instancias_clase.front());

        aWrap = new AgentePythonWrapper(a);
    }
    catch(error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    
    return aWrap;
}
void Scripting::ModuloPython::finalizar()
{
    
}

void Scripting::ModuloPython::manejar_excepcion_python
                                          (boost::python::error_already_set& e)
{
    manejar_excepcion_python_libre(e, this->namespace_modulo,
                                       this->namespace_modulo);
}

void Scripting::ModuloPython::extraer_clase()
{
    using namespace boost::python;
    if(this->esta_extraida_clase)
        return;
    try
    {
        object clase =
        exec("l = dir()\n"
             "if l.count('Reglas') == 0: raise Error()\n"
             "clase = None\n"
             "for i in l:\n"
             "    res = eval(i)\n"
             "    if type(res) == type(Reglas.Agente):\n"
             "        clase = res\n"
             "        break\n"
             "if clase is None: raise Error()\n"
                , this->namespace_modulo, this->namespace_modulo);
        this->agente_clase = this->namespace_modulo["clase"];
    }
    catch(error_already_set& e)
    {
        this->manejar_excepcion_python(e);
    }
    this->esta_extraida_clase = true;
}