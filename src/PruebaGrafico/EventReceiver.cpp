 
#include "EventReceiver.hpp"

 using namespace irr;

EventReceiver::EventReceiver()
{
for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    KeyIsDown[i] = false;
}

// This is the one method that we have to implement
bool EventReceiver::OnEvent(const SEvent& event)
{

 // revisa si el evento fue un evento del teclado, y si es así se modifica el estado de la tecla presionada
if (event.EventType == irr::EET_KEY_INPUT_EVENT){
    KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
}

// si el efento fue un evento del mouse almecanamos la posicion de éste, y si presiono el botón zquierdo o no.
if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
{
    bool evento=false;
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

