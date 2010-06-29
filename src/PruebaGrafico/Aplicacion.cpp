
#include "Aplicacion.hpp"
using namespace irr;


Aplicacion::Aplicacion(std::string pathJ1,std::string pathj2,std::string video,bool fullscreen, int Vel){
    
    this->Dvideo=new Video(video);
    if(this->Dvideo->getVideoType()==video::EDT_NULL) this->grafico=false; else this->grafico=true;
    this->device=this->Dvideo->creaDevice(fullscreen);
    this->Vdriver = this->device->getVideoDriver();
    this->smgr = this->device->getSceneManager();
    this->env =this->device->getGUIEnvironment();
    this->device->setResizable(false);

    if(this->grafico)
        this->skin=new Grafico::Skin(this->smgr,this->env,device->getFileSystem());
     else
        this->skin==NULL;

    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->grafico);

    if(this->grafico){
        this->skin=new Grafico::Skin(this->smgr,this->env,device->getFileSystem());
        this->eventos=new EventReceiver(this);
        this->device->setEventReceiver(this->eventos);
    }
    
}

Aplicacion::Aplicacion(const Aplicacion& orig) {
}

Aplicacion::~Aplicacion() {
    delete (this->juego);
    this->Vdriver->removeAllHardwareBuffers();
    this->Vdriver->removeAllTextures();
    this->smgr->clear();
    this->env->clear();
    this->device->drop();
}

void Aplicacion::run(){
    if(!this->grafico)
        this->loopConsola();
    else
        this->loopGrafico();

   
}

ManejadorJuego* Aplicacion::getManJuego(){
    return this->juego;
}

void Aplicacion::nuevoJuego(){
    delete this->juego;
    this->smgr->clear();
    //this->env->clear();
    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->grafico);
}

void Aplicacion::loopGrafico(){

     this->device->setWindowCaption(L"Quassimodo");
    this->smgr->setShadowColor(video::SColor(150,0,0,0));

    while(this->device->run()&& !this->juego->getSalir())
            {
        bool dibuja=false;

        if(this->device->isFullscreen()){
            dibuja=true;}
        else if (this->device->isWindowActive()){
            dibuja=true;}

        if(dibuja)
            {
            this->Vdriver->beginScene(true, true, video::SColor(0,0,0,0));
            this->smgr->drawAll();
            this->env->drawAll();
            this->Vdriver->endScene();
            }
        else{
            this->device->yield();
                }
        }


}
void Aplicacion::loopConsola(){

   if( this->seleccionaOpcion(this->juego->setMenu()) )
   {
        while(this->juego->enCurso())
        {
            this->juego->SiguienteJugada();
            this->juego->imprimeTableroConsola();
        }
   }
    if(this->juego->hayGanador())
    {
        this->juego->getManejadorGUI()->MsgBox( "Hay un ganador! desde menu",false);
        this->loopConsola();
    }

}

bool Aplicacion::seleccionaOpcion(char op){

          switch(op){
            case 'a': case 'A':
                this->juego->SetAgentesConsola(true);
                return true;
                break;
            case 'b': case 'B':
               this->juego->SetAgentesConsola(false);
                return true;
                break;
            case 'c':case 'C':
                this->juego->getManejadorGUI()->creditos(false);
                this->loopConsola();
                return false;
                break;
            case 'd':case 'D':
                this->juego->getManejadorGUI()->MsgBox(" ¡¡Proximamente!! ",false);
                this->loopConsola();
                return false;
                break;
            case 'e': case 'E':
                this->juego->getManejadorGUI()->MsgBox(" ¡¡Bye!! ",false);
                return false;
                break;
            default:
                this->juego->getManejadorGUI()->MsgBox(" ¡¡Opcion incorrecta!! ",false);
                this->loopConsola();
                return false;
                break;
    }
          
  }