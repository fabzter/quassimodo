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
#include<irrlicht/irrlicht.h>
#include<vector>

using namespace irr;
using namespace Grafico;

int main(int argc, char** argv) {

video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	// create device
       EventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(800, 600), 16, false, true, false,&receiver);


	if (device == 0)
		return 1; // could not create selected driver.



	device->setWindowCaption(L"Prueba téxturas tablero");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

      int x=0,y=0,z=0;
     // scene::ICameraSceneNode* cam= smgr->addCameraSceneNode(0, core::vector3df(x,y,z), core::vector3df(0,0,0));
     scene::ICameraSceneNode* cam =  smgr->addCameraSceneNodeMaya(0,200.f,200.f,0);

                //texturas!!!!
        video::ITexture *barrera= driver->getTexture( "Texturas/barrera.jpg" ) ;
        video::ITexture *celda= driver->getTexture( "Texturas/celda7.jpg" ) ;
         video::ITexture *tablero= driver->getTexture( "Texturas/celda7.jpg" ) ;
           //video::ITexture *sky= driver->getTexture( "Texturas/skydome2.jpg" ) ;
            smgr->setShadowColor(video::SColor(150,0,0,0));
            //scene::IAnimatedMesh* mesh = smgr->getMesh("Texturas/Antorcha2.3ds");

        //driver->makeNormalMapTexture(tablero,1.0f);
	//CSampleSceneNode *n =
	//	new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666);

         f32 sh=100.0f ,shc=50.0f;
scene::ISceneNode *n;
           //hacemos la base pal tablero
        n= smgr->addCubeSceneNode(TAM_PIEZA,0,666, core::vector3df(4*SCAL_TABLERO_ANCHO,-( TAM_PIEZA*( SCAL_CELDA_GROSOR+(SCAL_CELDA_GROSOR/2) ) ),4*SCAL_TABLERO_ANCHO),core::vector3df(90,90,0), core::vector3df(SCAL_TABLERO_ANCHO, SCAL_TABLERO_ANCHO, 1));
        n-> setMaterialTexture( 0, tablero);
        //n-> addShadowVolumeSceneNode();
	//smgr->setShadowColor(video::SColor(150,0,0,0));(video::EMT_SOLID irr::video::EMT_REFLECTION_2_LAYER
        n->setMaterialType(video::EMT_REFLECTION_2_LAYER);

        n->setMaterialFlag (irr::video::EMF_LIGHTING,true);
        n->getMaterial(0).NormalizeNormals=true;
      //  n->getMaterial(0) .Shininess=sh;


        ///hacemos las luces fuego etc
        int a=(TAM_PIEZA*SCAL_CELDA_ANCHO*9)+(TAM_PIEZA*9);//-(TAM_PIEZA*4);
        int b=-(TAM_PIEZA*5);

         std::vector<Antorcha> ant ;
         ant.reserve(4);

        for(int i=0;i<2;i++)
        {
            for(int j=0;j<2;j++)
                 {

               //antorcha!!

                int x=i==0? b:a, y=j==0? b:a ;
               ant.push_back(Antorcha(smgr, x,y)) ;
               // n=ant[k]->getNodo();
               // antorcha *a=new antorcha(smgr,x,y);
               // n=a->getNodo();


            }

        }
while(device->run())
	{

            if (device->isWindowActive())
		{

		driver->beginScene(true, true, video::SColor(0,00,00,00));



                smgr->drawAll();
                driver->endScene();
            }
            else{
             device->yield();
            }
	}

	device->drop();

    return (EXIT_SUCCESS);
}

