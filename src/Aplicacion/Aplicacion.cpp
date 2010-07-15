
#include "Aplicacion.hpp"
using namespace irr;


Aplicacion::Aplicacion(Opciones::ManejadorOpciones &opciones){
    
    this->Dvideo=new Grafico::Video( opciones.getVideoMode().c_str() );
    if(this->Dvideo->getVideoType()==video::EDT_NULL) this->grafico=false; else this->grafico=true;

    this->velAnimacion=opciones.getVelocidad();

    if(this->grafico){
        this->device=this->Dvideo->creaDevice (opciones.isFullScreen() );
        this->Vdriver = this->device->getVideoDriver();
        this->smgr = this->device->getSceneManager();
        this->env =this->device->getGUIEnvironment();
        this->device->setResizable(false);
        this->skin=new Grafico::Skin(this->smgr,this->env,device->getFileSystem(),opciones);
        this->eventos=new EventReceiver(this);
        this->device->setEventReceiver(this->eventos);
    }
     else{
        this->skin=NULL;
        this->device=NULL;
        this->Vdriver =NULL;
        this->smgr =NULL;
        this->env=NULL;
     }

    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion,this->grafico);

    this->quick(opciones.getAgentePath(0),opciones.getAgentePath(1));
}

Aplicacion::Aplicacion(const Aplicacion& orig) {
}

Aplicacion::~Aplicacion() {
    delete (this->juego);
    if(this->grafico){
        this->Vdriver->removeAllHardwareBuffers();
        this->Vdriver->removeAllTextures();
        this->smgr->clear();
        this->env->clear();
        this->device->drop();
    }
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
    if(this->grafico)
        this->smgr->getVideoDriver()->deleteAllDynamicLights();
    //this->smgr->clear();
    
    this->juego=new ManejadorJuego(this->smgr,this->env,this->skin,this->velAnimacion,this->grafico);
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
void Aplicacion::loopConsola(){

   
   if( this->p_rapida || this->seleccionaOpcion( this->juego->setMenu() ) )
   {
       if(this->p_rapida){
            this->juego->setPartida();
            this->p_rapida=false;
       }
        while(this->juego->enCurso())
        {
            try{
                this->juego->SiguienteJugada();
            }
            catch(std::exception &e)
            {
                this->loopConsola();
                break;
            }
            this->juego->imprimeTableroConsola();
            
            
        }
   }
   if(this->juego->hayGanador())
        {
            this->juego->getManejadorGUI()->MsgBox(this->juego->getMsjGanador(),false);
            this->juego->getManejadorGUI()->MsgBox(" ¡¡Bye!! ",false);
        }

 

}

bool Aplicacion::seleccionaOpcion(char op){

          switch(op){
            case 'a': case 'A':
                if( this->juego->SetAgentesConsola(true) )
                    return true;
                else{
                    this->loopConsola();
                    return false;}
                break;
            case 'b': case 'B':
                if( this->juego->SetAgentesConsola(false) )
                    return true;
                else{
                    this->loopConsola();
                    return false;}
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
void Aplicacion::quick(std::string pathJ1,std::string pathj2){

    if(pathJ1!=""&& pathj2!=""){
        this->juego->setAgente(pathJ1,0);
        this->juego->setAgente(pathj2,1);
        this->p_rapida=true;
    }
    else{
    this->p_rapida=false;}
}
