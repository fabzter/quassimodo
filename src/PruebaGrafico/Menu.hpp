

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

using namespace irr;
/**
 * Enum que contiene los identificadores para cada boton en em Menú de la aplicación
 */
enum GUI_BOTONES{

    B_AGENTE_VS_MAKINA=0,
    B_AGENTE_VS_AGENTE,
    B_OPCIONES,
    B_CREDITOS,
    B_AYUDA,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    B_COUNT=5

};
enum GUI_BOTONES_AGENTES{

    BA_AGENTE_1=10,
    BA_AGENTE_2,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BA_COUNT=2

};
enum GUI_BOTONES_OPCION{

    BO_INICIA=100,
    BO_CANCELA,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BO_COUNT=2

};

const wchar_t* const GUI_BOTONES_TEXTO[B_COUNT+1]=
{
L"Agente VS Maquina",
L"Agente VS Agente",
L"Opciones",
L"Creditos",
L"Ayuda",
0
};
const wchar_t* const GUI_BOTONES_TOLTIP[B_COUNT+1]=
{
L"Partida rápida en el que el segundo competidor es el Agente Integrado en la aplicación",
L"Partida rápida de dos Agentes Inteligentes creados por algun usuario",
L"Cambia algunas propiedades de la Aplicación",
L"Creditos",
L"Ayuda de como usar la Aplicación",
0
};


class Menu {
public:
    Menu(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin);

    Menu(const Menu& orig);
    virtual ~Menu();
    void setMenuP(Grafico::Tablero* t);
    void dropMenuP();
    void MsgBox(const char* msg );
     void AgntVSAgnt();
     std::string getPath();
     void  OpenFileDialog();
private:
      void setCamara();
     void setSkin(bool dialogo=false);
      void SetJugadores(Grafico::Tablero* t);
     void setBotones();
    
    std::vector<Grafico::Pieza*> jugadores;
    std::vector<gui::IGUIButton *> botones;
    bool haymenu;
    Grafico::Skin* skin;
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    gui::IGUIFileOpenDialog *op;
    gui::IGUIWindow* AvsA;
};

#endif	/* _MenuHPP */

