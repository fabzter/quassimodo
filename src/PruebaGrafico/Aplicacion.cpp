
#include "Aplicacion.hpp"
#include "Juego.hpp"
using namespace irr;


Aplicacion::Aplicacion(std::string pathJ1,std::string pathj2,std::string video,bool fullscreen, int Vel){
    
    this->Dvideo=new Video(video);
    this->device = irr::createDevice(this->Dvideo->getVideoType(),core::dimension2d<u32>(800, 600), 16,fullscreen,true, false);
    this->Vdriver = this->device->getVideoDriver();
    this->smgr = this->device->getSceneManager();
    this->env =this->device->getGUIEnvironment();
    this->juego=new Juego(this->smgr,this->env);
    this->eventos=new EventReceiver(this->juego);
    this->device->setEventReceiver(this->eventos);
}

Aplicacion::Aplicacion(const Aplicacion& orig) {
}

Aplicacion::~Aplicacion() {
}
void Aplicacion::run(){
    this->device->setWindowCaption(L"Quassimodo");
    this->smgr->setShadowColor(video::SColor(150,0,0,0));
    while(this->device->run()&& !this->juego->getSalir())
            {

                if (this->device->isWindowActive())
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

            this->device->drop();
    }
