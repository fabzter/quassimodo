

#include "ManejadorGUI.hpp"
#include "GUI.hpp"
#include "Menu.hpp"
using namespace irr;

ManejadorGUI::ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin) {
    this->t=t;
    this->menu=new Menu(smgr,env,t,skin);
    this->Gui=new GUI(smgr,env,skin);
}

ManejadorGUI::ManejadorGUI(const ManejadorGUI& orig) {
}

ManejadorGUI::~ManejadorGUI() {
}
void ManejadorGUI::dropMenu(){
    this->menu->dropMenuP();
    this->Gui->dropAvsA();
}
void ManejadorGUI::MsgBox(const char* msg){
    this->Gui->MsgBox(msg);
}
Menu* ManejadorGUI::getMenu(){
    return this->menu;
}
 void ManejadorGUI::AgntVSAgnt(){
     this->menu->dropMenuP();
     this->Gui->AgntVSAgnt();
 }
 std::string ManejadorGUI::getPath(){
     return this->Gui->getPath();
 }
 void  ManejadorGUI::OpenFileDialog(){
     this->Gui->OpenFileDialog();
 }
 void ManejadorGUI::setMenu(){
     this->Gui->dropAvsA();
     this->menu->setMenuP(this->t);
 }
