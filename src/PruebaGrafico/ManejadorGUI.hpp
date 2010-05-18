

#ifndef _MANEJADORGUI_HPP
#define	_MANEJADORGUI_HPP
#include<irrlicht/irrlicht.h>
#include<Grafico/Skin.hpp>
#include<Grafico/Tablero.hpp>
#include"Menu.hpp"
#include"GUI.hpp"
//TODO: documentar
using namespace irr;
class ManejadorGUI {
public:
    ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin);
    ManejadorGUI(const ManejadorGUI& orig);
    virtual ~ManejadorGUI();
    void dropMenu();
    void MsgBox(const char* msg);
    Menu* getMenu();
     void AgntVSAgnt();
     std::string getPath();
     void  OpenFileDialog();
     void setMenu();
private:
    Menu* menu;
    GUI* Gui;
    Grafico::Tablero* t;

};

#endif	/* _MANEJADORGUI_HPP */

