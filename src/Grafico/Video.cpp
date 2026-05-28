#include "Video.hpp"
using namespace irr;

Grafico::Video::Video(std::string video) {
  if (video == "AUTO")
    this->SeleccionaDriverType();
  else if (video == "OPENGL")
    this->setDriver(video::EDT_OPENGL);
  else if (video == "NULL")
    this->setDriver(video::EDT_NULL);
  else
    this->SeleccionaDriverType();
}

Grafico::Video::Video(const Video &orig) {}

Grafico::Video::~Video() {}
void Grafico::Video::SeleccionaDriverType() {
  // D2: IrrlichtMt removed _IRR_COMPILE_WITH_OPENGL_ macro.
  // OpenGL works on macOS (proven by engine spike).
  this->setDriver(video::EDT_OPENGL);
}

void Grafico::Video::setDriver(video::E_DRIVER_TYPE tipo) { this->type = tipo; }
video::E_DRIVER_TYPE Grafico::Video::getVideoType() { return this->type; }
IrrlichtDevice *Grafico::Video::creaDevice(bool fullscreen) {

  if (!fullscreen)
    return irr::createDevice(video::EDT_OPENGL,
                             core::dimension2d<u32>(1024, 768), 16, false,
                             false, false, nullptr);
  else
    return irr::createDevice(video::EDT_OPENGL,
                             core::dimension2d<u32>(1024, 768), 16, true, false,
                             false, nullptr);
}
void Grafico::Video::DetectaEscritorio() {
  this->DResolution = core::dimension2d<u32>(1024, 768);
  this->DPixel = 32;
}
