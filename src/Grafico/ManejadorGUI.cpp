

#include "ManejadorGUI.hpp"

using namespace irr;

Grafico::ManejadorGUI::ManejadorGUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Tablero* t,Skin* skin,bool grafico) {
    this->menu=new Menu(smgr,env,t,skin);
    this->Gui=NULL;
    if(grafico)
        this->Gui=new GUI(smgr,env,skin);

}

Grafico::ManejadorGUI::ManejadorGUI(const ManejadorGUI& orig) {
}

Grafico::ManejadorGUI::~ManejadorGUI() {
    if(this->Gui!=NULL)
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

 std::string Grafico::ManejadorGUI::getPath(bool grafico,int noA){
     if(grafico){
         return this->Gui->getPath();
     }
     else{
         return this->pideAgente(noA);
     }

     
 }

 void  Grafico::ManejadorGUI::OpenFileDialog(){
     this->Gui->OpenFileDialog();
 }

 char Grafico::ManejadorGUI::setMenu(bool grafico){

     if(grafico){
        this->menu->setMenuP();
        return '0';
     }
     else
        return this->menu->MenuConsola();
     
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
  std::string Grafico::ManejadorGUI::pideAgente(int noA){
    std::string agente;
    std::cout<<std::endl<<"Escribe la ruta del Agente :"<<noA<<std::endl;
    std::cin>>agente;
    return agente;
  }

  void Grafico::ManejadorGUI::creditos(bool grafico){

      if(!grafico){
          std::string creditos;
          creditos="\t\t****Quassimodo fue desarrollado por:****\n\nFabrizio Alonso Hernandez Hernandez \t\t <faboster@gmail.com>\n"
                  "Hugo Cesar Hernandez Pinha \t\t\t <eltokyo@gmail.com>";

          this->MsgBox(creditos.c_str(),grafico);
      }
      else{
          this->Gui->setCreditos();
      }

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