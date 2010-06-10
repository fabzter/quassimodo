
#include "GUI.hpp"
using namespace irr;

GUI::GUI(scene::ISceneManager* smgr,gui::IGUIEnvironment* env,Grafico::Skin* Skin) {
    this->smgr=smgr;
    this->env=env;
    this->skin=Skin;
    this->botonAgente.resize(2);
    this->botonPartida.resize(BP_COUNT);
    this->AvsA=NULL;
    botonesPartida=false;
   this->setSkin();
}

GUI::GUI(const GUI& orig) {
}

GUI::~GUI() {
}
void GUI::setSkin(){
     //gui::IGUISkin* skin = this->env->createSkin(gui::EGST_WINDOWS_METALLIC);
    
	gui::IGUISkin* skin =this->env->getSkin();

         //skin->setFont(this->skin->getGUIWindow(),gui::EGDF_WINDOW);

         skin->setFont(this->skin->getGUIBoton() ,gui::EGDF_BUTTON);
         skin->setFont(this->skin->getDefault(),gui::EGDF_DEFAULT);
         skin->setFont(this->skin->getMenuToolTip(),gui::EGDF_TOOLTIP);
        
         
      skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));

      //ponemos mas opaco los colores de la GUI
     for (u32 i=0; i<gui::EGDC_COUNT ; ++i)
     {
         video::SColor  col = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);
         col.setAlpha(250);
         skin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
     }
  // this->env->setSkin(skin);
    //skin->drop();
 }
void GUI::MsgBox(const char* msg ){

    wchar_t m[strlen(msg)] ;
    this->charTowchar(m,msg);
    gui::IGUIWindow* window=this->env->addMessageBox(L"Quassimodo dice:",m);

}
 void GUI::AgntVSAgnt(){
     this->dibujaSelector(true);
 }
 void GUI::AgntVSMkn(){
     this->dibujaSelector(false);
 }
std::string GUI::getPath(){

     const  wchar_t*   m=op->getFileName();
    size_t requiredSize=  wcslen(m);
    char* msg = (char*)malloc( requiredSize*sizeof(wchar_t));
    wcstombs( msg,m, requiredSize);
   char *py=strrchr(msg,'.');

   if(py!=NULL){
    py+=3;
    (*py)='\0';
   }
    std::string cosa(msg);
     std::cout<<msg<<std::endl;
    free(msg);

     return cosa;
  }

 void  GUI::OpenFileDialog(){
      op = this->env->addFileOpenDialog(L"Selecciona el Agente",true,0,5);
  }
void GUI::dropAvsA(){
    if(this->AvsA!=NULL)
        this->AvsA->remove();
}

void GUI::dibujaSelector(bool ambos){
      core::dimension2d<unsigned int> S_S=this->smgr->getVideoDriver()->getScreenSize();
      int d_a=100,d_al=150;
      core::rect<s32> recW=core::rect<s32>(100, 150 , S_S.Width-d_a ,S_S.Height-d_al);

      AvsA= this->env->addWindow(recW,true,L"Agentes:");

       this->env->addStaticText(L"Vs",core::rect<s32>((recW.getWidth()/2)-15,(recW.getHeight()/2)-30,(recW.getWidth()/2)+15,(recW.getHeight()/2)+40),\
               false,false,AvsA);

       this->env->addStaticText( L"Selecciona los Agentes", core::rect<s32>(35,35,300,60), false, false, AvsA );
       //botones de los agentes
      this->botonAgente[0]= this->env->addButton( core::rect<s32>( (recW.getWidth()/2)-185,(recW.getHeight()/2)-50 ,(recW.getWidth()/2)-35,(recW.getHeight()/2) ),\
               AvsA, BA_AGENTE_1, L"Agente?");
       this->botonAgente[1]= this->env->addButton( core::rect<s32>( (recW.getWidth()/2)+40,(recW.getHeight()/2)-50 ,(recW.getWidth()/2)+190,(recW.getHeight()/2) ),\
               AvsA, BA_AGENTE_2, L"Agente?");
      if(!ambos){
          this->botonAgente[1]->setText(L"Agente Camina");
          this->botonAgente[1]->setEnabled(ambos);
          this->botonAgente[1]-> setPressed(!ambos);
      }
       //botones de Opcion
       this->env->addButton( core::rect<s32>( recW.getWidth()-360,recW.getHeight()-60 ,recW.getWidth()-190,recW.getHeight()-20) ,\
               AvsA, BO_CANCELA, L"Cancelar");
       this->env->addButton( core::rect<s32>( recW.getWidth()-160,recW.getHeight()-60 ,recW.getWidth()-20,recW.getHeight()-20 ),\
               AvsA, BO_INICIA, L"Inicia");

}
void  GUI::charTowchar(wchar_t m[],const char* msg){
   size_t requiredSize=strlen(msg);
    mbstowcs( m,msg, requiredSize);

}
void GUI::setTextAgnt(int num,const char* text){
    if(this->AvsA!=NULL){
        wchar_t m[strlen(text)] ;
        this->charTowchar(m,text);
        this->botonAgente.at(num)->setText(m);
    }
}
void GUI::setBotonesPartida(){
    
    core::dimension2d<unsigned int> Scren_Size=this->smgr->getVideoDriver()->getScreenSize();
      int dis_ancho=20,dis_alto=20;
      float size=50,dan=dis_ancho;
      for(int i=0;i<BP_COUNT;i++){
          
            this->botonPartida.at(i)= this->env->addButton(core::rect<s32>( dan,dis_alto,dan+size, dis_alto+size ),0,i+1000,L"2") ;
            dan+=size+10;
      this->botonPartida.at(i)->setDrawBorder(true);

      }
      botonesPartida=true;
    
}
void GUI::dropBotonesPartida(){
    if(botonesPartida){

          for(int i=0;i<BP_COUNT;i++){
            this->botonPartida.at(i)->remove();
      }

    }

}