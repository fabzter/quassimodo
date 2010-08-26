#ifndef MANEJADORAGENTES_HPP
#define	MANEJADORAGENTES_HPP
#include <Reglas/Agente.hpp>
#include "FabricaAgente.hpp"
#include <string>
#include <map>

/**
 * Este namespace contiene a nuestros Agente implementados. En realidad uno
 * sólo debe interactuar con la clase ManejadorAgentes, que sirve como catálogo
 * y fábrica de nuestros Agentes.
 */
namespace Agentes
{

/**
 * Esta clase sirve tanto de catálogo, como fábrica de agentes. Es decir, 
 * nosotros no necesitamos instanciar directamente a ningún Agente, sino que
 * el manejador nos entrega una lista de los Agentes (getNombresAgentes), y
 * podemos pedirle instancia del que queramos (makeAgente).
 */
class ManejadorAgentes
{
public:
    ManejadorAgentes();
    ManejadorAgentes(const ManejadorAgentes& orig);
    virtual ~ManejadorAgentes();

    /**
     * Regresa una lista de las strings que identican a todos los agentes
     * registrados en el ManejadorAgentes.
     * @return un vector de strings con los nombres que identican a los Agentes
     * que se le pueden pedir al ManejadorAgentes.
     */
    const std::vector<std::string> getNombresAgentes() const;

    /**
     * Crea un Agente, de agún tipo de los Agentes regisstrados en el
     * ManejadorAgentes. Deja la responsabilidad del limpiar el objeto al
     * llamador.
     * @param nombre la string que identifica al tipo de Agente que se quiere
     * pedir.
     * @return un apuntador al tipo de agente solicitado.
     * @throws std::out_of_range si no existe un Agente con dicho nombre.
     */
    Reglas::Agente* makeAgente(std::string& nombre) const;

private:
    std::map<std::string, FabricaAgente*> agentes;
    /**
     * Registra a un nuevo tipo de Agente al manejador.
     * @param nombre La string con la que será identificado el Agente.
     * @param apuntador un objeto de alguna clase derivada de FabricanteAgente,
     * que entregue un nuevo objeto del tipo de Agente que se registra. El
     * manejador se apropia del apuntador.
     */
    void registrar(std::string &nombre, FabricaAgente *fabrica);
};

}
#endif	/* MANEJADORAGENTES_HPP */

