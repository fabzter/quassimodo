

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
 void ManejadorGUI::AgntVSAgnt(bool ambos){
     this->menu->dropMenuP();
     if(ambos)
         this->Gui->AgntVSAgnt();
     else
         this->Gui->AgntVSMkn();
 }
 std::string ManejadorGUI::getPath(){
     return this->Gui->getPath();
 }
 void  ManejadorGUI::OpenFileDialog(){
     this->Gui->OpenFileDialog();
 }
 void ManejadorGUI::setMenu(){
     this->Gui->dropAvsA();
     this->menu->setMenuP();
 }
 void ManejadorGUI::SetTextBtnAngt(int num, std::string text){
     this->Gui->setTextAgnt(num,text.c_str() );
 }
 void ManejadorGUI::setEscala(int x,int y,int z){
     this->menu->SetEscala(x,y,z);
 }
 void ManejadorGUI::setMenuPartida(){
     this->Gui->setBotonesPartida();
 }