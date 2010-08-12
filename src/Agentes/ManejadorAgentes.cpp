#include "ManejadorAgentes.hpp"
#include "AgenteAstar.hpp"
#include "AgenteCaminaAdelante.hpp"

Agentes::ManejadorAgentes::ManejadorAgentes()
{
    std::string nombre;

    nombre = "Agente A*";
    this->registrar(nombre, new FabricaAstar());

    nombre = "Agente Camina Hacia Adelante";
    this->registrar(nombre, new FabricaCaminaAdelante());
}

Agentes::ManejadorAgentes::ManejadorAgentes(const ManejadorAgentes& orig)
{
}

Agentes::ManejadorAgentes::~ManejadorAgentes()
{
    for(std::map<std::string, FabricaAgente*>::iterator it = this->agentes.begin();
            it != this->agentes.end(); ++it)
    {
        delete it->second;
    }
}

const std::vector<std::string> Agentes::ManejadorAgentes::getNombresAgentes() const
{
    std::vector<std::string> nombres;
    for(std::map<std::string, FabricaAgente*>::const_iterator it = this->agentes.begin();
            it != this->agentes.end(); ++it)
    {
        nombres.push_back(it->first);
    }

    return nombres;
}

Reglas::Agente* Agentes::ManejadorAgentes::makeAgente(std::string& nombre) const
{
    FabricaAgente* fabrica = this->agentes.at(nombre);
    return (*fabrica)();
}

void Agentes::ManejadorAgentes::registrar(std::string& nombre,
                                                Agentes::FabricaAgente* fabrica)
{
    using namespace std;
    this->agentes.insert(std::make_pair<string, FabricaAgente*>(nombre, fabrica));
}