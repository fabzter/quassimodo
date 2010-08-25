#include "ManejadorAgentes.hpp"
#include "AgenteAstar.hpp"
#include "AgenteCaminaAdelante.hpp"
#include "AgenteMinimax2.hpp"
#include "AgenteHumano.hpp"
#include "AgenteMinimax4.hpp"

Agentes::ManejadorAgentes::ManejadorAgentes()
{
    std::string nombre;

    nombre = "Agente A*";
    this->registrar(nombre, new FabricaAstar());

    nombre = "Agente Camina Hacia Adelante";
    this->registrar(nombre, new FabricaCaminaAdelante());

    nombre = "Agente MiniMax2";
    this->registrar(nombre, new FabricaMinimax2());

    nombre = "Agente MiniMax4";
    this->registrar(nombre, new FabricaMinimax4());

    nombre = "Agente Humano";
    this->registrar(nombre, new FabricaHumano());
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