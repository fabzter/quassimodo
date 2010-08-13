

#include "ManejadorGUI.hpp"

using namespace irr;

Grafico::ManejadorGUI::ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin) {
    this->menu=new Menu(smgr,env,t,skin);
    this->Gui=new GUI(smgr,env,skin);

}

Grafico::ManejadorGUI::ManejadorGUI(const ManejadorGUI& orig) {
}

Grafico::ManejadorGUI::~ManejadorGUI() {
    delete(this->Gui);
    delete(this->menu);
}
void Grafico::ManejadorGUI::dropMenu(){
    this->menu->dropMenuP();
}
void Grafico::ManejadorGUI::MsgBox(const char* msg,bool grafico,GUI_BOTONES_OK idMsg){

    if (grafico)
        this->Gui->MsgBox(msg,idMsg);
    else
        this->MsgBoxConsola(msg);

}

 void Grafico::ManejadorGUI::AgntVSAgnt(bool ambos,std::vector<std::string> lista){
     
    // this->menu->dropMenuP();
     if(ambos)
         this->Gui->AgntVSAgnt();
     else
         this->Gui->AgntVSMkn(lista);
 }


 std::string Grafico::ManejadorGUI::getAgenteSeleccionado(){
     return this->Gui->getAgenteSeleccionado();
 }

 void Grafico::ManejadorGUI::setMenu(){

        this->menu->setMenuP();   
 }

 void Grafico::ManejadorGUI::SetTextBtnAngt(int num, std::string text){
     this->Gui->setTextAgnt(num,text.c_str() );
 }

 void Grafico::ManejadorGUI::setEscala(int x,int y,int z){
     this->menu->SetEscala(x,y,z);
 }

 void Grafico::ManejadorGUI::setMenuPartida(){
     this->Gui->setBotonesPartida();
 }

 void Grafico::ManejadorGUI::MsgBoxConsola(const char* msg){
    std::cout<<std::endl<<"###############################################################################";
    std::cout<<std::endl<<msg<<std::endl;
    std::cout<<std::endl<<"###############################################################################"<<std::endl<<std::endl;
 }


  void Grafico::ManejadorGUI::creditos(){

      this->Gui->setCreditos();

  }

  void Grafico::ManejadorGUI::dropAvsA(){

      this->Gui->dropAvsA();

  }
  void Grafico::ManejadorGUI::dropBotonesPartida(){
      this->Gui->dropBotonesPartida();
  }
void Grafico::ManejadorGUI::Pausar(bool pausa){
    this->Gui->Pausar(pausa);
}
void Grafico::ManejadorGUI::dropCreditos(){
    this->Gui->dropCreditos();
}