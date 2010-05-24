
#include <stdlib.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Constantes.hpp>
#include"EventReceiver.hpp"
#include<irrlicht.h>
#include<vector>
#include<Grafico/Tablero.hpp>
#include<Grafico/Celda.hpp>
#include<Reglas/Agente.hpp>
#include<Scripting/Manejador.hpp>
#include<Scripting/Excepciones.hpp>
#include<Reglas/Excepciones.hpp>
#include<Grafico/Jugador.hpp>
#include<Grafico/Barrera.hpp>
#include<string.h>
#include"Partida.hpp"
#include"Menu.hpp"
#include "Juego.hpp"
#include <cstdlib>
#include <irrlicht/ISceneManager.h>

using namespace irr;
using namespace Grafico;
void MsgBox(gui::IGUIEnvironment* env,const char* msg );
int main(int argc, char** argv) {

video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	IrrlichtDevice* device = irr::createDevice(driverType,
			core::dimension2d<u32>(800, 600), 16, false, false, false);


	if (device == 0)
		return 1; // could not create selected driver.



	device->setWindowCaption(L"Prueba téxturas tablero");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
        gui::IGUIEnvironment* env = device->getGUIEnvironment();


      int x=0,y=0,z=0;
     // scene::ICameraSceneNode* cam= smgr->addCameraSceneNode(0, core::vector3df(x,y,z), core::vector3df(0,0,0));
     scene::ICameraSceneNode* cam =  smgr->addCameraSceneNodeMaya(0,200.f,200.f,200.0f);


            smgr->setShadowColor(video::SColor(150,0,0,0));

            Juego *app=new Juego(smgr,env);
            device->setEventReceiver(app->getEventReceiver());
            bool haymenu=true;
             bool curso=false;


        
   
while(device->run())
	{

            if (device->isWindowActive())
		{

		driver->beginScene(true, true, video::SColor(0,00,00,00));
             
                smgr->drawAll();
                env->drawAll();
                driver->endScene();
            }
            else{
             device->yield();
            }
	}

	device->drop();

    return (EXIT_SUCCESS);
}

