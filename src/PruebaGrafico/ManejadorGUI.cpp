

#include "ManejadorGUI.hpp"

using namespace irr;

ManejadorGUI::ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Grafico::Skin* skin,bool grafico) {
    this->menu=new Menu(smgr,env,t,skin);
    if(grafico)
        this->Gui=new GUI(smgr,env,skin);
}

ManejadorGUI::ManejadorGUI(const ManejadorGUI& orig) {
}

ManejadorGUI::~ManejadorGUI() {
    delete(this->Gui);
    delete(this->menu);
}
void ManejadorGUI::dropMenu(){
    this->menu->dropMenuP();
}
void ManejadorGUI::MsgBox(const char* msg,bool grafico,GUI_BOTONES_OK idMsg){

    if (grafico)
        this->Gui->MsgBox(msg,idMsg);
    else
        this->MsgBoxConsola(msg);

}

 void ManejadorGUI::AgntVSAgnt(bool ambos){
     
    // this->menu->dropMenuP();
     if(ambos)
         this->Gui->AgntVSAgnt();
     else
         this->Gui->AgntVSMkn();
 }

 std::string ManejadorGUI::getPath(bool grafico,int noA){
     if(grafico){
         return this->Gui->getPath();
     }
     else{
         return this->pideAgente(noA);
     }

     
 }

 void  ManejadorGUI::OpenFileDialog(){
     this->Gui->OpenFileDialog();
 }

 char ManejadorGUI::setMenu(bool grafico){

     if(grafico){
        this->menu->setMenuP();
        return '0';
     }
     else
        return this->menu->MenuConsola();
     
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
 void ManejadorGUI::MsgBoxConsola(const char* msg){
    std::cout<<std::endl<<"###############################################################################";
    std::cout<<std::endl<<msg<<std::endl;
    std::cout<<std::endl<<"###############################################################################"<<std::endl<<std::endl;
 }
  std::string ManejadorGUI::pideAgente(int noA){
    std::string agente;
    std::cout<<std::endl<<"Escribe la ruta del Agente :"<<noA<<std::endl;
    std::cin>>agente;
    return agente;
  }

  void ManejadorGUI::creditos(bool grafico){
      char* creditos;
      creditos="\t\t****Quassimodo fue desarrollado por:****\n\nFabrizio Alonso Hernandez Hernandez \t\t <faboster@gmail.com>\n"
              "Hugo Cesar Hernandez Pinha \t\t\t <eltokyo@gmail.com>";

      this->MsgBox(creditos,grafico);

  }

  void ManejadorGUI::dropAvsA(){

      this->Gui->dropAvsA();

  }
  void ManejadorGUI::dropBotonesPartida(){
      this->Gui->dropBotonesPartida();
  }
