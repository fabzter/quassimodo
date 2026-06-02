
// D2.1: Video simplified to OpenGL-only.
// EDT_SOFTWARE/DIRECT3D8/DIRECT3D9 removed from IrrlichtMt fork.
// IVideoModeList / getVideoModeList() removed — use fixed defaults.
#include "Video.hpp"
using namespace irr;

Grafico::Video::Video(std::string video) {
        if (video=="OPENGL" || video=="AUTO")
            this->setDriver(video::EDT_OPENGL);
        else if(video=="NULL")
            this->setDriver(video::EDT_NULL);
        else
            this->SeleccionaDriverType();
}

Grafico::Video::Video(const Video& orig) {
    (void)orig;
}

Grafico::Video::~Video() {
}

void Grafico::Video::SeleccionaDriverType(){
    // D2.1: only EDT_OPENGL remains in the IrrlichtMt fork.
    this->setDriver(video::EDT_OPENGL);
}

void Grafico::Video::setDriver(video::E_DRIVER_TYPE tipo){
    this->type=tipo;
}

video::E_DRIVER_TYPE Grafico::Video::getVideoType(){
    return this->type;
}

IrrlichtDevice* Grafico::Video::creaDevice(bool fullscreen){
     this->DetectaEscritorio();
     if(!fullscreen)
         return irr::createDevice(this->getVideoType(),core::dimension2d<u32>(1024, 768), this->DPixel,fullscreen,true, true);
     else
         return irr::createDevice(this->getVideoType(),this->DResolution, 16,fullscreen,true, true);
}

void Grafico::Video::DetectaEscritorio(){
    // D2.1: IVideoModeList / getVideoModeList() removed from IrrlichtMt fork.
    // Use fixed desktop defaults; the window resolution is overridden in creaDevice anyway.
    this->DResolution = core::dimension2d<u32>(1024, 768);
    this->DPixel = 16;
}