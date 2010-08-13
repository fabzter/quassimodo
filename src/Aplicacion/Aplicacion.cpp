#include "Aplicacion.hpp"
using namespace irr;


Aplicacion::Aplicacion(Opciones::ManejadorOpciones &opciones){
    
    this->Dvideo=new Grafico::Video( opciones.getVideoMode().c_str() );
    if(this->Dvideo->getVideoType()==video::EDT_NULL) this->grafico=false; else this->grafico=true;
    this->velAnimacion=opciones.getVelocidad();
    std::string err="";
        this->device=this->Dvideo->creaDevice (opciones.isFullScreen() );
        this->Vdriver = this->device->getVideoDriver();
        this->smgr = this->device->getSceneManager();
        this->env =this->device->getGUIEnvironment();
        try{
            this->skin=new Grafico::Skin(this->smgr,this->env,device->getFileSystem(),opciones);
            this->eventos=new EventReceiver(this);
            this->device->setEventReceiver(this->eventos);
            this->device->setResizable(false);
        }
        catch(Grafico::SkinNoCargado &e){
            err=e.what();
            err+=" \n>>>>>Saliendo de la Aplicación<<<<< ";
            this->grafico=false;
            this->dropIrrlicht();
            this->setNull();
        }
        
    

    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion,this->grafico);

    if(err!="")
        this->juego->getManejadorGUI()->MsgBox(err.c_str(),this->grafico,Grafico::BOK_ADVERTENCIA);
   
    this->quick(opciones.getAgentePath(0),opciones.getAgentePath(1));
}

Aplicacion::Aplicacion(const Aplicacion& orig) {
}

Aplicacion::~Aplicacion() {
    delete (this->juego);
    if(this->grafico){
        this->dropIrrlicht();
    }
}

void Aplicacion::run(){

        this->loopGrafico();

}

ManejadorJuego* Aplicacion::getManJuego(){
    return this->juego;
}

void Aplicacion::nuevoJuego(){
    std::string err="";
    try{
        delete this->juego;
    }
    catch(std::exception& e){
        err=e.what();
        this->juego->getManejadorGUI()->dropMenu();
        this->smgr->clear();
        
    }
    
    if(this->grafico)
        this->smgr->clear();
    
    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion,this->grafico);
    if(err!="")
        this->juego->getManejadorGUI()->MsgBox(err.c_str(),true,Grafico::BOK_ADVERTENCIA);
}

void Aplicacion::loopGrafico(){

    this->device->setWindowCaption(L"Quassimodo");
    this->smgr->setShadowColor(video::SColor(150,0,0,0));
    s32 lastFPS = -1;
   
    while(this->device->run()&& !this->juego->getSalir())
            {
        bool dibuja=false;

        if(this->device->isFullscreen()){
            dibuja=true;}
        else if (this->device->isWindowActive()){
            dibuja=true;}

        if(dibuja)
            {
            this->juego->despachaJugada();
            this->Vdriver->beginScene(true, true, video::SColor(0,0,0,0));
            this->smgr->drawAll();
            this->env->drawAll();
            this->Vdriver->endScene();

            const s32 fps = Vdriver->getFPS();

            if (lastFPS != fps)
		{
			core::stringw str = L"Quassimodo [";
			str += Vdriver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
            if(this->p_rapida){
                this->juego->setPartida();
                this->p_rapida=false;
            }
            
            }
        else{
            this->device->yield();
                }
        }
}


void Aplicacion::quick(std::string pathJ1,std::string pathj2){

    if(pathJ1!=""&& pathj2!=""){
        this->juego->setAgente(pathJ1,0);
        this->juego->setAgente(pathj2,1);
        this->p_rapida=true;
    }
    else{
    this->p_rapida=false;}
}
void Aplicacion::setNull(){
    this->skin=NULL;
    this->device=NULL;
    this->Vdriver =NULL;
    this->smgr =NULL;
    this->env=NULL;

}

void Aplicacion::dropIrrlicht(){

    this->Vdriver->removeAllHardwareBuffers();
    this->Vdriver->removeAllTextures();
    this->smgr->clear();
    this->env->clear();
    this->device->drop();
}
