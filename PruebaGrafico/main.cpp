/*
 * File:   main.cpp
 * Author: tokayo
 *
 * Created on 15 de marzo de 2010, 10:17
 */

#include <stdlib.h>
#include<Grafico/Antorcha.hpp>
#include<Grafico/Constantes.hpp>
#include<Grafico/EventReceiver.hpp>
#include<irrlicht.h>
#include<vector>
#include<Grafico/Tablero.hpp>
#include<Grafico/Celda.hpp>
#include<Reglas/Agente.hpp>
#include<Scripting/Manejador.hpp>
#include<Grafico/Jugador.hpp>
#include<Grafico/Barrera.hpp>
#include"Partida.hpp"
#include"GUI.hpp"
using namespace irr;
using namespace Grafico;

int main(int argc, char** argv) {

video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	// create device
       EventReceiver receiver;

	IrrlichtDevice* device = irr::createDevice(driverType,
			core::dimension2d<u32>(800, 600), 16, false, false, false,&receiver);


	if (device == 0)
		return 1; // could not create selected driver.



	device->setWindowCaption(L"Prueba téxturas tablero");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
        gui::IGUIEnvironment* env = device->getGUIEnvironment();

      int x=0,y=0,z=0;
     // scene::ICameraSceneNode* cam= smgr->addCameraSceneNode(0, core::vector3df(x,y,z), core::vector3df(0,0,0));
     scene::ICameraSceneNode* cam =  smgr->addCameraSceneNodeMaya(0,200.f,200.f,200.0f);

                //texturas!!!!

         video::ITexture *tablero= driver->getTexture( "Texturas/celda7.jpg" ) ;
           video::ITexture *sky= driver->getTexture( "Texturas/3.tree.skydome.png" ) ;
            smgr->setShadowColor(video::SColor(150,0,0,0));



         Partida *p=new Partida(smgr);

   
           p->SetJugadores("../bin/agenteBarreras2.py","../bin/agenteBarreras2.py");
           
           p->iniciarPartida();
            //GUI *gui=new GUI(smgr,env);
         


         smgr->addSkyDomeSceneNode(sky);
    
          core::vector3df v;//=a->getPosicionEscena();
          // x=v.X,y=v.Y,z=v.Z;
while(device->run())
	{

            if (device->isWindowActive())
		{

		driver->beginScene(true, true, video::SColor(0,00,00,00));
                if(receiver.IsKeyDown(irr::KEY_KEY_Q)){
                    p->SetEscala(1,1,1);
                   // b->setPosicion(x++,y,z);

                }
                if(receiver.IsKeyDown(irr::KEY_KEY_W)){
                   p->SetEscala(2,2,2);
                   // b->setPosicion(x--,y,z);
                }
               
 
                if(receiver.IsKeyDown(irr::KEY_RETURN)){
                    p->siguienteJugada();
                    /*gui::IGUIWindow* window = env->addWindow(core::rect<s32>(100, 100 , 300 , 200 ),false,	L"Test window");
                    env->addStaticText(L"Please close me",
						core::rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);*/
                }


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
