 #include "EventReceiver.hpp"
#include "Aplicacion.hpp"

 using namespace irr;

EventReceiver::EventReceiver(Aplicacion* app)
{
    this->app=app;
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

              this->piniciada=this->app->getManJuego()->SiguienteJugada();

             if(  event.KeyInput.Key==irr::KEY_KEY_Q)
                 if(!this->piniciada && this->app->getManJuego()->quick()){
                     this->piniciada=true;
                 }
             if(  event.KeyInput.Key==irr::KEY_KEY_P){
                this->app->getManJuego()->printCam();
             }
            if(  event.KeyInput.Key==irr::KEY_KEY_C){
                this->app->getManJuego()->crea();
            }
            if(  event.KeyInput.Key==irr::KEY_KEY_A){
                this->app->getManJuego()->aumenta();
            }

            
            if(  event.KeyInput.Key==irr::KEY_KEY_D){
                this->app->getManJuego()->destruye();
            }
            
                
            break;
            // si el elento fue un evento del mouse almecanamos la posicion de éste, y si presiono el botón zquierdo o no.
       case irr::EET_MOUSE_INPUT_EVENT:
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
                        this->app->getManJuego()->setAgente("../bin/agenteBarreras2.py", 1);
                       this->app->getManJuego()->getManejadorGUI()->AgntVSAgnt(false);
                        break;
                    case B_AGENTE_VS_AGENTE:
                        this->app->getManJuego()->getManejadorGUI()->AgntVSAgnt(true);
                         break;
                    case B_OPCIONES:
                       this->app->getManJuego()->getManejadorGUI()->MsgBox("Proximamente!!");
                        break;
                    case  B_CREDITOS:
                        this->app->getManJuego()->getManejadorGUI()->creditos(true);
                        break;
                    case B_SALIR:
                        this->app->getManJuego()->setSalir(true);
                        break;
                    case BA_AGENTE_1:
                        this->noA=0;
                       this->app->getManJuego()->getManejadorGUI()->OpenFileDialog();
                        break;
                    case BA_AGENTE_2:
                        this->noA=1;
                        this->app->getManJuego()->getManejadorGUI()->OpenFileDialog();
                        break;
                    case BO_INICIA:
                        this->app->getManJuego()->getManejadorGUI()->dropAvsA();
                        if( this->app->getManJuego()->setPartida() )
                            this->piniciada=true;
                        break;
                    case BO_CANCELA:
                       this->app->getManJuego()->clearAgentes();
                       this->app->getManJuego()->getManejadorGUI()->dropAvsA();
                       //this->app->getManJuego()->getManejadorGUI()->setMenu();
                        break;
                    case BP_VISTA1:
                       this->app->getManJuego()->cambiaVistaJuego(1);
                        break;
                    case BP_VISTA2:
                       this->app->getManJuego()->cambiaVistaJuego(2);
                        break;
                    case BP_VISTA3:
                       this->app->getManJuego()->cambiaVistaJuego(3);
                        break;
                    case BP_VISTA4:
                       this->app->getManJuego()->cambiaVistaJuego(4);
                        break;
                    case BP_MENU:
                        this->piniciada=false;
                        this->app->nuevoJuego();
                         break;

                } 
            }
            //si es un evento que selecciono un archivo
            if(event.GUIEvent.EventType==gui::EGET_FILE_SELECTED){
               this->app->getManJuego()->setAgente(this->app->getManJuego()->getManejadorGUI()->getPath(), this->noA);
               this->app->getManJuego()->CambiaTextoAgnt(this->noA);
                this->noA=-1;

              }
            //si es un evento de presionar el botón de OK en un msgBox
            if(event.GUIEvent.EventType==gui::EGET_MESSAGEBOX_OK){
                switch(event.GUIEvent.Caller->getID()){
                    case BOK_ERROR:
                        this->app->nuevoJuego();
                        break;
                }
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

