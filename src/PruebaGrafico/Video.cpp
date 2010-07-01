
#include "Video.hpp"
using namespace irr;

Video::Video(std::string video) {
        if (video=="AUTO")
            this->SeleccionaDriverType();
        else if(video=="OPENGL")
            this->setDriver(video::EDT_OPENGL);
        else if(video== "DIRECT9")
            this->setDriver(video::EDT_DIRECT3D9);
        else if(video=="DIRECT8")
            this->setDriver(video::EDT_DIRECT3D8);
        else if(video=="NULL")
            this->setDriver(video::EDT_NULL);
        else
            this->SeleccionaDriverType();

}

Video::Video(const Video& orig) {
}

Video::~Video() {
}
void Video::SeleccionaDriverType(){

#ifdef _IRR_COMPILE_WITH_SOFTWARE_
    this->setDriver(video::EDT_SOFTWARE);
    std::cout<<"software"<<std::endl;
#endif
 #ifdef _IRR_COMPILE_WITH_OPENGL_
this->setDriver(video::EDT_OPENGL);
    std::cout<<"openGL"<<std::endl;
#endif

#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
    this->setDriver(video::EDT_DIRECT3D8);
    std::cout<<"direct3d8"<<std::endl;
#endif
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
    this->setDriver(video::EDT_DIRECT3D9);
    std::cout<<"direct3d9"<<std::endl;
#endif

}

void Video::setDriver(video::E_DRIVER_TYPE tipo){
    this->type=tipo;
}
video::E_DRIVER_TYPE Video::getVideoType(){
    return this->type;
}
 IrrlichtDevice* Video::creaDevice( bool fullscreen){

     this->DetectaEscritorio();
     if(!fullscreen)
         return irr::createDevice(this->getVideoType(),core::dimension2d<u32>(1024, 768), this->DPixel,fullscreen,true, true);
     else
         return irr::createDevice(this->getVideoType(),this->DResolution, 32,fullscreen,true, true);
 }
void Video::DetectaEscritorio(){
  IrrlichtDevice* device;
  device=irr::createDevice(video::EDT_SOFTWARE);
  video::IVideoModeList *VL = device->getVideoModeList();
  this->DResolution=VL->getDesktopResolution();
  this->DPixel=VL->getDesktopDepth();

  device->drop();

  }