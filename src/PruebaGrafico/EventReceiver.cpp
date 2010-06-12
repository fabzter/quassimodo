 
#include "EventReceiver.hpp"

 using namespace irr;

EventReceiver::EventReceiver(ManejadorJuego* juego)
{
    this->juego=juego;
    this->piniciada=false;
    this->noA=-1;
for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    KeyIsDown[i] = false;
}

// This is the one method that we have to implement
bool EventReceiver::OnEvent(const SEvent& event)
{

 // revisa si el evento fue un evento del teclado, y si es así se modifica el estado de la tecla presionada
    switch(event.EventType){
        case irr::EET_KEY_INPUT_EVENT:
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            if(  this->piniciada && event.KeyInput.Key==irr::KEY_KEY_R)
              this->piniciada=this->juego->SiguienteJugada();
             if(  event.KeyInput.Key==irr::KEY_KEY_Q)
                 if(!this->piniciada){
                    this->juego->quick();
                     this->piniciada=true;
                 }
             if(  event.KeyInput.Key==irr::KEY_KEY_P){
                this->juego->printCam();
             }
            if(  event.KeyInput.Key==irr::KEY_KEY_A){
                this->juego->aumentaAngulo(true);
             }
            if(  event.KeyInput.Key==irr::KEY_KEY_S){
                this->juego->disminuyeAngulo(true);
             }
            if(  event.KeyInput.Key==irr::KEY_KEY_Z){
                this->juego->aumentaAngulo(false);
             }
            if(  event.KeyInput.Key==irr::KEY_KEY_X){
                this->juego->disminuyeAngulo(false);
             }
                
            break;
            // si el elento fue un evento del mouse almecanamos la posicion de éste, y si presiono el botón zquierdo o no.
       case irr::EET_MOUSE_INPUT_EVENT:
            //bool evento=false;
            switch(event.MouseInput.Event)
            {
                    case EMIE_LMOUSE_PRESSED_DOWN:
                            MouseState.LeftButtonDown = true;
                            break;
                    case EMIE_LMOUSE_LEFT_UP:
                            MouseState.LeftButtonDown = false;
                            break;

                    case EMIE_MOUSE_MOVED:
                            MouseState.Posicion.X = event.MouseInput.X;
                            MouseState.Posicion.Y = event.MouseInput.Y;
                            break;

            }
            break;
            //si es un evento dela GUI
        case irr::EET_GUI_EVENT:

            if(event.GUIEvent.EventType== gui::EGET_BUTTON_CLICKED){
                s32 id = event.GUIEvent.Caller->getID();
                switch(id){
                    case B_AGENTE_VS_MAKINA:
                        this->juego->setAgente("../bin/agenteBarreras2.py", 1);
                       this->juego->getManejadorGUI()->AgntVSAgnt(false);
                        break;
                    case B_AGENTE_VS_AGENTE:
                        this->juego->getManejadorGUI()->AgntVSAgnt(true);
                         break;
                    case B_OPCIONES:
                       this->juego->getManejadorGUI()->MsgBox("Proximamente!!");
                        break;
                    case B_SALIR:
                        this->juego->setSalir(true);
                        break;
                    case BA_AGENTE_1:
                        this->noA=0;
                       this->juego->getManejadorGUI()->OpenFileDialog();
                        break;
                    case BA_AGENTE_2:
                        this->noA=1;
                       this->juego->getManejadorGUI()->OpenFileDialog();
                        break;
                    case BO_INICIA:
                       this->juego->setPartida();
                          this->piniciada=true;
                        break;
                    case BO_CANCELA:
                       this->juego->clearAgentes();
                       this->juego->getManejadorGUI()->setMenu();
                        break;
                    case BP_VISTA1:
                       this->juego->cambiaVistaJuego(1);
                        break;
                    case BP_VISTA2:
                       this->juego->cambiaVistaJuego(2);
                        break;
                    case BP_VISTA3:
                       this->juego->cambiaVistaJuego(3);
                        break;
                    case BP_VISTA4:
                       this->juego->cambiaVistaJuego(4);
                        break;

                } 
            }
            if(event.GUIEvent.EventType==gui::EGET_FILE_SELECTED){
               this->juego->setAgente(this->juego->getManejadorGUI()->getPath(), this->noA);
               this->juego->CambiaTextoAgnt(this->noA);
                this->noA=-1;
              }
      
            break;
}

return false;

}

const SMouseState& EventReceiver::GetMouseState(void) const
{
	return MouseState;
}
bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}
EventReceiver::~EventReceiver(){
  }

