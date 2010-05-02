

#ifndef _APLICACION_HPP
#define	_APLICACION_HPP
#include "Menu.hpp"
#include "Partida.hpp"
#include<Reglas/Excepciones.hpp>
#include<Scripting/Excepciones.hpp>
#include<string.h>
using namespace irr;
class Aplicacion {
public:
    Aplicacion(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    Aplicacion(const Aplicacion& orig);
    virtual ~Aplicacion();
    void setMenu();
    void setPartida();
    void SiguienteJugada();
    bool hayGanador();
private:
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    Menu *menu;
    Partida *partida;

};

#endif	/* _APLICACION_HPP */

