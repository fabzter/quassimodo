

#ifndef _GUI_HPP
#define	_GUI_HPP
#include<irrlicht/irrlicht.h>
#include<Grafico/Skin.hpp>
#include"Enums.hpp"
#include<iostream>
#include<vector>
//TODO: documentar
using namespace irr;
class GUI {
public:
    GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Skin* Skin);
    GUI(const GUI& orig);
    virtual ~GUI();
     void MsgBox(const char* msg );
     void AgntVSAgnt();
     void AgntVSMkn();
     std::string getPath();
     void  OpenFileDialog();
     void dropAvsA();
     void setTextAgnt(int num,const char* text);
private:
    void setSkin();
    void dibujaSelector(bool ambos);
    void charTowchar(wchar_t m[],const char* msg);
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    gui::IGUIFileOpenDialog *op;
    gui::IGUIWindow* AvsA;
    Grafico::Skin* skin;
    std::vector<gui::IGUIButton *> botonAgente;

};

#endif	/* _GUI_HPP */

