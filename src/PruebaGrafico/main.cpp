
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
#include "Aplicacion.hpp"
#include <cstdlib>

using namespace irr;
using namespace Grafico;
void MsgBox(gui::IGUIEnvironment* env,const char* msg );
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



        // Partida *p;//=new Partida(smgr);

   
           //p->SetJugadores("../bin/agenteBarreras2.py","../bin/agenteBarreras2.py");
           
          // p->iniciarPartida();
           // Menu *gui=new Menu(smgr,env);
            Aplicacion *app=new Aplicacion(smgr,env);
            bool haymenu=true;
             bool curso=false;


         smgr->addSkyDomeSceneNode(sky);
    
          core::vector3df v;//=a->getPosicionEscena();
          // x=v.X,y=v.Y,z=v.Z;
while(device->run())
	{

            if (device->isWindowActive())
		{

		driver->beginScene(true, true, video::SColor(0,00,00,00));
                if(receiver.IsKeyDown(irr::KEY_KEY_Q)){
                    if(haymenu){
                       /* delete(gui);//->dropMenuP();
                        smgr = device->getSceneManager();
                        smgr->clear();
                        env->clear();
                        haymenu=false;*/
                        app->setPartida();
                        haymenu=false;
                    }

                }
                if(receiver.IsKeyDown(irr::KEY_KEY_W)){
                    if(!haymenu){
                        app->setMenu();
                        haymenu=true;
                    }
                /* p=new Partida(smgr);
                  p->SetJugadores("../bin/agenteBarreras2.py","../bin/agenteBarreras2.py");
                     p->iniciarPartida();
                    smgr->addCubeSceneNode();*/

                   //p->SetEscala(2,2,2);
                   // b->setPosicion(x--,y,z);
                }
               
 
                if(receiver.IsKeyDown(irr::KEY_RETURN)){

                        if(!curso){
                        curso=true;
                       // while(!app->hayGanador()){
                             app->SiguienteJugada();
                             //device->sleep(1000,true);
                       // }


                    }
                   
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

