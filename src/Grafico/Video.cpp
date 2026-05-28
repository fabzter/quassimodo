#include "Video.hpp"
using namespace irr;

Grafico::Video::Video(std::string video) {
	if (video=="AUTO")
		this->SeleccionaDriverType();
	else if(video=="OPENGL")
		this->setDriver(video::EDT_OPENGL);
	else if(video=="NULL")
		this->setDriver(video::EDT_NULL);
	else
		this->SeleccionaDriverType();

}

Grafico::Video::Video(const Video& orig) {
}

Grafico::Video::~Video() {
}
void Grafico::Video::SeleccionaDriverType(){

#ifdef _IRR_COMPILE_WITH_OPENGL_
	this->setDriver(video::EDT_OPENGL);
	std::cout<<"openGL"<<std::endl;
#else
	this->setDriver(video::EDT_NULL);
	std::cout<<"null"<<std::endl;
#endif

}

void Grafico::Video::setDriver(video::E_DRIVER_TYPE tipo){
	this->type=tipo;
}
video::E_DRIVER_TYPE Grafico::Video::getVideoType(){
	return this->type;
}
 IrrlichtDevice* Grafico::Video::creaDevice( bool fullscreen){

	 if(!fullscreen)
		 return irr::createDevice(this->getVideoType(),core::dimension2d<u32>(1024, 768), 32,fullscreen,true, true);
	 else
		 return irr::createDevice(this->getVideoType(),core::dimension2d<u32>(1024, 768), 32,fullscreen,true, true);
 }
void Grafico::Video::DetectaEscritorio(){
  this->DResolution=core::dimension2d<u32>(1024, 768);
  this->DPixel=32;
}
