
#include "Aplicacion.hpp"
using namespace irr;

Aplicacion::Aplicacion(Opciones::ManejadorOpciones &opciones) {

  this->Dvideo = new Grafico::Video(opciones.getVideoMode().c_str());
  if (this->Dvideo->getVideoType() == video::EDT_NULL)
    this->grafico = false;
  else
    this->grafico = true;
  this->velAnimacion = opciones.getVelocidad();
  std::string err = "";
  if (this->grafico) {
    this->device = this->Dvideo->creaDevice(opciones.isFullScreen());
    this->Vdriver = this->device->getVideoDriver();
    this->smgr = this->device->getSceneManager();
    this->env = this->device->getGUIEnvironment();
    try {
      this->skin = new Grafico::Skin(this->smgr, this->env,
                                     device->getFileSystem(), opciones);
      this->eventos = new EventReceiver(this);
      this->device->setEventReceiver(this->eventos);
      this->device->setResizable(false);
    } catch (Grafico::SkinNoCargado &e) {
      std::cerr << "[D2] SkinNoCargado: " << e.what() << std::endl;
      err = e.what();
      err += " \n>>>>>Cambio a modo Consola<<<<< ";
      this->grafico = false;
      this->dropIrrlicht();
      this->setNull();
    }

  } else {
    this->setNull();
  }

  this->juego = new ManejadorJuego(this->smgr, this->env, this->skin,
                                   this->velAnimacion, this->grafico);

  if (err != "")
    this->juego->getManejadorGUI()->MsgBox(err.c_str(), this->grafico,
                                           Grafico::BOK_ADVERTENCIA);

  this->quick(opciones.getAgentePath(0), opciones.getAgentePath(1));
}

Aplicacion::Aplicacion(const Aplicacion &orig) {}

Aplicacion::~Aplicacion() {
  delete (this->juego);
  if (this->grafico) {
    this->dropIrrlicht();
  }
}

void Aplicacion::run() {
  if (!this->grafico)
    this->loopConsola();
  else
    this->loopGrafico();
}

ManejadorJuego *Aplicacion::getManJuego() { return this->juego; }

void Aplicacion::nuevoJuego() {
  std::string err = "";
  try {
    delete this->juego;
  } catch (std::exception &e) {
    err = e.what();
    this->juego->getManejadorGUI()->dropMenu();
    this->smgr->clear();
  }

  this->juego = new ManejadorJuego(this->smgr, this->env, this->skin,
                                   this->velAnimacion, this->grafico);
  if (err != "")
    this->juego->getManejadorGUI()->MsgBox(err.c_str(), true,
                                           Grafico::BOK_ADVERTENCIA);
}

void Aplicacion::loopGrafico() {

  this->device->setWindowCaption(L"Quassimodo");
  std::cerr << "[D2] device=" << (void *)this->device
            << " driver=" << (void *)this->Vdriver << std::endl;
  this->smgr->setAmbientLight(video::SColorf(0.4f, 0.4f, 0.4f, 1.0f));

  // D2: Ensure camera is set up BEFORE game loop starts.
  // setPartida() skips camera setup in -a quick-start path.
  if (!this->smgr->getActiveCamera()) {
    scene::ICameraSceneNode *cam = this->smgr->addCameraSceneNode();
    cam->setPosition(core::vector3df(300, 400, -400));
    cam->setTarget(core::vector3df(300, 0, 300));
    this->smgr->setActiveCamera(cam);
  }

  s32 lastFPS = -1;

  // Call setPartida before entering the render loop (handles p_rapida)
  if (this->p_rapida) {
    this->juego->setPartida();
    this->p_rapida = false;
  }

  while (this->device->run()) {
    static int frame = 0;
    if (frame++ < 3)
      std::cerr << "[D2] frame " << frame
                << " salir=" << this->juego->getSalir() << std::endl;
    bool dibuja = false;

    if (this->device->isFullscreen()) {
      dibuja = true;
    } else if (this->device->isWindowActive()) {
      dibuja = true;
    }

    if (dibuja) {
      this->juego->despachaJugada();
      this->Vdriver->beginScene(true, true, video::SColor(0, 0, 0, 0));
      this->smgr->drawAll();
      this->env->drawAll();
      this->Vdriver->endScene();

      const s32 fps = Vdriver->getFPS();

      if (lastFPS != fps) {
        core::stringw str = L"Quassimodo [";
        str += Vdriver->getName();
        str += "] FPS:";
        str += core::stringw(std::to_wstring(fps).c_str());

        device->setWindowCaption(str.c_str());
        lastFPS = fps;
      }
    } else {
      this->device->yield();
    }
  }
}
void Aplicacion::loopConsola() {

  if (this->p_rapida || this->seleccionaOpcion(this->juego->setMenu())) {
    if (this->p_rapida) {
      this->juego->setPartida();
      this->p_rapida = false;
    }
    while (this->juego->enCurso()) {
      try {
        this->juego->SiguienteJugada();
      } catch (std::exception &e) {
        this->loopConsola();
        break;
      }
      this->juego->imprimeTableroConsola();
    }
  }
  if (this->juego->hayGanador()) {
    this->juego->getManejadorGUI()->MsgBox(this->juego->getMsjGanador(), false);
    this->juego->getManejadorGUI()->MsgBox(" ¡¡Bye!! ", false);
  }
}

bool Aplicacion::seleccionaOpcion(char op) {

  switch (op) {
  case 'a':
  case 'A':
    if (this->juego->SetAgentesConsola(true))
      return true;
    else {
      this->loopConsola();
      return false;
    }
    break;
  case 'b':
  case 'B':
    if (this->juego->SetAgentesConsola(false))
      return true;
    else {
      this->loopConsola();
      return false;
    }
    break;
  case 'c':
  case 'C':
    this->juego->getManejadorGUI()->creditos(false);
    this->loopConsola();
    return false;
    break;
  case 'd':
  case 'D':
    this->juego->getManejadorGUI()->MsgBox(" ¡¡Proximamente!! ", false);
    this->loopConsola();
    return false;
    break;
  case 'e':
  case 'E':
    this->juego->getManejadorGUI()->MsgBox(" ¡¡Bye!! ", false);
    return false;
    break;
  default:
    this->juego->getManejadorGUI()->MsgBox(" ¡¡Opcion incorrecta!! ", false);
    this->loopConsola();
    return false;
    break;
  }
}
void Aplicacion::quick(std::string pathJ1, std::string pathj2) {

  if (pathJ1 != "" && pathj2 != "") {
    this->juego->setAgente(pathJ1, 0);
    this->juego->setAgente(pathj2, 1);
    this->p_rapida = true;
  } else {
    this->p_rapida = false;
  }
}
void Aplicacion::setNull() {
  this->skin = NULL;
  this->device = NULL;
  this->Vdriver = NULL;
  this->smgr = NULL;
  this->env = NULL;
}

void Aplicacion::dropIrrlicht() {

  this->Vdriver->removeAllHardwareBuffers();
  this->Vdriver->removeAllTextures();
  this->smgr->clear();
  this->env->clear();
  this->device->drop();
}
