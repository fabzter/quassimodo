 #include "EventReceiver.hpp"
#include "Aplicacion.hpp"

 using namespace irr;

EventReceiver::EventReceiver(Aplicacion* app)
{
    this->app=app;
    this->piniciada=false;
    this->noA=-1;
    this->mover=this->barr_este=this->AmbosHumanos=false;
for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    KeyIsDown[i] = false;
}

bool EventReceiver::OnEvent(const SEvent& event)
{

 // revisa si el evento fue un evento del teclado, y si es así se modifica el estado de la tecla presionada
    switch(event.EventType){
        case irr::EET_KEY_INPUT_EVENT:
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;          
            break;
            // si el elento fue un evento del mouse almecanamos la posicion de éste, y si presiono el botón zquierdo o no.
       case irr::EET_MOUSE_INPUT_EVENT:
            switch(event.MouseInput.Event)
            {
                    case EMIE_MOUSE_MOVED:
                            MouseState.Posicion.X = event.MouseInput.X;
                            MouseState.Posicion.Y = event.MouseInput.Y;
                            break;
                    case EMIE_LMOUSE_PRESSED_DOWN:
                            MouseState.LeftButtonDown = true;
                            break;
                    case EMIE_LMOUSE_LEFT_UP:
                            MouseState.LeftButtonDown = false;
                            core::position2d<s32> pp=MouseState.Posicion;
                            if( this->piniciada && this->app->getManJuego()->estaHaciendoJugada()){
                                int celda=this->app->getManJuego()->ChecaJugada(pp,this->mover,this->barr_este);
                                if(celda>=0){
                                    this->app->getManJuego()->setJugada(celda,this->mover,this->barr_este);
                                    this->mover=false;}
                            }
                            break;
            }
            break;
            //si es un evento dela GUI
        case irr::EET_GUI_EVENT:

            if(event.GUIEvent.EventType== gui::EGET_BUTTON_CLICKED){
 
                this->Click_a_Boton( event.GUIEvent.Caller->getID() );
   
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
void EventReceiver::Click_a_Boton(irr::s32 id){
    switch(id){
        case B_AGENTE_VS_MAKINA:
            this->app->getManJuego()->setAgente("Agente Humano", 0);
            this->app->getManJuego()->AgntVsMkn();
            break;
        case B_AGENTE_VS_AGENTE:
            //this->app->getManJuego()->getManejadorGUI()->AgntVSAgnt(true);
             break;
         case B_OPCIONES:
             this->app->getManJuego()->getManejadorGUI()->MsgBox("¡¡Proximamente!!");
             break;
        case  B_CREDITOS:
             this->app->getManJuego()->getManejadorGUI()->creditos();
             break;
        case B_SALIR:
             this->app->getManJuego()->setSalir(true);
             break;
        case BO_INICIA:
             this->app->getManJuego()->setAgente(this->app->getManJuego()->getManejadorGUI()->getAgenteSeleccionado(), 1);
             this->app->getManJuego()->getManejadorGUI()->dropAvsA();
             if( this->app->getManJuego()->setPartida(this->AmbosHumanos) )
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
       case BP_PAUSA:
             this->app->getManJuego()->Pausar();
             break;
        case BJ_MOVER:
            this->app->getManJuego()->setOpcionesMover();
            this->mover=true;
            break;
       case BP_MENU:
             this->piniciada=false;
             this->app->nuevoJuego();
             break;
       case BO_CREDITOS:
             this->app->getManJuego()->getManejadorGUI()->dropCreditos();
             break;
    }
}
void EventReceiver::ArmaJugada(){

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

