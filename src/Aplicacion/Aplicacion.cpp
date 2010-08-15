#include "Aplicacion.hpp"
using namespace irr;


Aplicacion::Aplicacion(Opciones::ManejadorOpciones &opciones){
    
    this->Dvideo=new Grafico::Video( opciones.getVideoMode().c_str() );
    if(this->Dvideo->getVideoType()==video::EDT_NULL) this->grafico=false; else this->grafico=true;
    this->velAnimacion=opciones.getVelocidad();
    std::string err="";
    if(!this->grafico){
        err=">>> No pudo ser cargada la parte de video! (puede que el driver de la tarjeta de video no esté soportado) <<<<";
    }
    else{
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
    }
        
    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion);

    if(err!="")
        this->juego->getManejadorGUI()->MsgBox(err.c_str(),false,Grafico::BOK_ADVERTENCIA);
   
    
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

    if(this->grafico)
        this->loopGrafico();

}

ManejadorJuego* Aplicacion::getManJuego(){
    return this->juego;
}

void Aplicacion::nuevoJuego(){
    delete (this->juego);
    this->smgr->clear();
    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion);
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
            }
        else{
            this->device->yield();
                }
        }
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
