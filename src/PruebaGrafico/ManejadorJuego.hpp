/* 
 * File:   ManejadorJuego.hpp
 * Author: tokayo
 *
 * Created on 11 de mayo de 2010, 19:26
 */

#ifndef _MANEJADORJUEGO_HPP
#define	_MANEJADORJUEGO_HPP

#include "Menu.hpp"
#include "Partida.hpp"
#include "AnimacionEnd.hpp"
#include<Reglas/Excepciones.hpp>
#include<Scripting/Excepciones.hpp>
#include<string.h>
using namespace irr;
class ManejadorJuego {
public:
   ManejadorJuego(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    ManejadorJuego(const ManejadorJuego& orig);
    virtual ~ManejadorJuego();
    void setMenu();
    void setPartida();
   bool SiguienteJugada();
    bool hayGanador();
    Menu* getMenu();
    void setAgente(std::string Agente,int noAgente);
    void clearAgentes();
    void quick();
private:
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    std::vector<std::string> Agentes;
    bool hayagente;
    Menu *menu;
    Partida *partida;
    AnimacionEnd *aniend;

};

#endif	/* _MANEJADORJUEGO_HPP */

