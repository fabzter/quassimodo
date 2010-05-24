

#ifndef _Menu_HPP
#define	_Menu_HPP
#include<string.h>
#include<irrlicht.h>
#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/vector3d.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Tablero.hpp>
#include<Grafico/Skin.hpp>
#include<string>
#include<vector>
#include<wchar.h>
#include"Enums.hpp"
//TODO: documentar
using namespace irr;
class Menu {
public:
    Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin);

    Menu(const Menu& orig);
    virtual ~Menu();
    void setMenuP(Grafico::Tablero* t);
    void dropMenuP();
    void SetEscala(int x,int y,int z);

private:
     void setSkin(bool dialogo=false);
      void SetJugadores();
      void setPosicionJugadores();
     void setBotones();
    Grafico::Tablero* t;
    std::vector<Grafico::Pieza*> jugadores;
    std::vector<gui::IGUIButton *> botones;
    bool haymenu;
    Grafico::Skin* skin;
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;

};

#endif	/* _MenuHPP */

