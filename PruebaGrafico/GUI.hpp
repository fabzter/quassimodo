

#ifndef _GUI_HPP
#define	_GUI_HPP

#include<irrlicht.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Tablero.hpp>
#include<vector>
#include<string>
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
    B_COUNT

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
L"Partida rapida en el que el segundo competidor es el Agente Integrado en la aplicacion",
L"Partida rapida de dos Agentes Inteligentes creados por algun usuario",
L"Cambia algunas propiedades de la Aplicacion",
L"Creditos",
L"Ayuda de como usar la Aplicacion",
0
};


class GUI {
public:
    GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env);
    GUI(const GUI& orig);
    virtual ~GUI();
    void SetJugadores();
    void setCamara();
    void setGUI();
private:
     void ColocaAntorchas();
     void setSkin();
     void setBotones();
    Grafico::Tablero *t;
    std::vector<Grafico::Antorcha*> antorchas;
    std::vector<Grafico::Pieza*> jugadores;
    std::vector<gui::IGUIButton *> botones;
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
};

#endif	/* _GUI_HPP */

