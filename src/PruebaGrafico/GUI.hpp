

#ifndef _GUI_HPP
#define	_GUI_HPP
#include<irrlicht/irrlicht.h>
#include<Grafico/Skin.hpp>
#include"Enums.hpp"
#include<iostream>
using namespace irr;
class GUI {
public:
    GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Skin* Skin);
    GUI(const GUI& orig);
    virtual ~GUI();
     void MsgBox(const char* msg );
     void AgntVSAgnt();
     std::string getPath();
     void  OpenFileDialog();
     void dropAvsA();
private:
    void setSkin();
    scene::ISceneManager* smgr;
    gui::IGUIEnvironment* env;
    gui::IGUIFileOpenDialog *op;
    gui::IGUIWindow* AvsA;
    Grafico::Skin* skin;

};

#endif	/* _GUI_HPP */

