#ifndef _SKIN_HPP
#define	_SKIN_HPP
#include"Exepciones.hpp"
#include<irrlicht/irrlicht.h>
#include<Opciones/ManejadorOpciones.hpp>
#include "CImageGUISkin.h"
#include "SkinLoader.h"

#include<vector>
using namespace irr;
namespace Grafico{
/**
 * Esta clase es la que se dedica a cargar todos los modelos y texturas que ocupará el juego.
 * asi como tambien la tipografia de la GUI
 */
class Skin {
public:
    /**
     *Constructor de la clase skin, manda a llamar a todos los metodos set.
     * @param smgr un apuntador al manejador de la escena del juego
     * @param env un puntador al manejador de la GUI del juego
     * @param fsys proporciona un apuntador al sistema de archivos
     * @param opciones Un objeto que sirve para manejar las opciones de creacion de la aplicacion
     */
    Skin(scene::ISceneManager* smgr, gui::IGUIEnvironment* env,
                io::IFileSystem* fsys,Opciones::ManejadorOpciones &opciones);
    /**
     *Constructor copia de la clase skin
     * @param orig un objeto de la clase skin
     */
    Skin(const Skin& orig);
    /**
     *Destructor
     */
    virtual ~Skin();

    /**
     *Se obtiene el Mesh (la malla) con la que es dibujada la celda
     * @return un apuntador a un objeto de la clase IAnimatedMesh
     */
    scene::IAnimatedMesh* getCelda();
    /**
     *Se obtiene el Mesh (la malla) con el que es dibujado el tablero
     * @return un apuntador a un objeto de la clase IAnimatedMesh
     */
    scene::IAnimatedMesh* getTablero();
    /**
     *Obtiene el Mesh (la malla) con el que es dibujado el jugador 1
     * @return un apuntador a un objeto de la clase IAnimatedMesh
     */
    scene::IAnimatedMesh* getJugador1();
    /**
     *Obtiene el Mesh (la malla) con el que es dibujado el jugador 2
     * @return un apuntador a un objeto de la clase IAnimatedMesh
     */
    scene::IAnimatedMesh* getJugador2();
    /**
     *Obtiene el Mesh (la malla) con la que es dibujada la Barrera
     *@return un apuntador a un objeto de la clase IAnimatedMesh
     */
    scene::IAnimatedMesh* getBarrera();
    /**
     *Obtiene la textura que le es aplicada a la malla del tablero
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTTAblero();
    /**
     *Obtiene la textura que le es aplicada a la malla de la celda
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTCelda();
    /**
     *Obtiene la textura que le es aplicada al fuego de la Antorcha
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTAntorcha();
    /**
     *Obtiene la textura que le es aplicada a la malla de la Barrera
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTBarrera();
    /**
     *Obtiene la tipografia que se le aplica a los botones del Menu
     * @return  un apuntador a un objeto de la clase IGUIFont
     */
    gui::IGUIFont* getMenuBoton();
    /**
     *Obtiene la tipografia que se  aplica por default a la GUI
     * @return  un apuntador a un objeto de la clase IGUIFont
     */
    gui::IGUIFont* getDefault();
    /**
     *Obtiene la tipografia que se le aplica al ToolTip
     * @return  un apuntador a un objeto de la clase IGUIFont
     */
    gui::IGUIFont* getMenuToolTip();
    /**
     *Obtiene la tipografia que se le aplica a los botones que no son del Menu
     * @return  un apuntador a un objeto de la clase IGUIFont
     */
    gui::IGUIFont* getGUIBoton();
    /**
     *Obtiene la tipografia que se le aplica a el titulo de la ventana
     * @return  un apuntador a un objeto de la clase IGUIFont
     */
    gui::IGUIFont* getGUIWindow();
    /**
     *Regresa el nodo escena que contiene el terreno
     * @return un objeto de la clase scene::ITerrainSceneNode
     */
   io::IReadFile* getheightMapFile();
    /**
     *Obtiene la textura que le es aplicada Terreno del juego
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTTerrain();
    /**
     *Obtiene la textura que le es aplicada al domo (el cielo del juego)
     * @return  un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getTSkydome();
    /**
     *Regresa el skin de la gui que sera utilizada.
     * @return un apuntador a un objeto de la clase skin::IGUISkin
     */
    gui::IGUISkin*  getSkinGui();
    /**
     * regresa la textura del boton de la partida
     * @param i indice del boton que se desea obtener si textura
     * @return un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getBotonPartida(int i);
    /**
     * regresa la textura del boton de la partida cuando esta presionado
     * @param i indice del boton que se desea obtener si textura
     * @return un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getBotonPartidaPres(int i);
    /**
     * Regresa la imagen de los crditos del juego
     * @return un apuntador a un objeto de la clase ITexture
     */
    video::ITexture* getImagenCreditos();


private:
    /**
     * Carga la malla de la antorcha y la textura del fuego.
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setFuego(scene::ISceneManager* smgr);
    /**
     *Carga la malla y la textura del tablero .
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setTablero(scene::ISceneManager* smgr);
    /**
     *Carga la malla de los jugadores y de su sombra
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setJugadores(scene::ISceneManager* smgr);
    /**
     *Carga la malla y la textura de la barrera.
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setBarrera(scene::ISceneManager* smgr);
    /**
     *Carga la malla y la textura de la celda .
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setCelda(scene::ISceneManager* smgr);
    /**
     *Carga la tipografia de los botones del Menu
     * @param env un apuntador al manejador de la gui de irrlicht.
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setMenuBoton(gui::IGUIEnvironment* env);
    /**
     *Carga la tipografia que tendrá por default
     * @param env un apuntador al manejador de la gui de irrlicht
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setDefault(gui::IGUIEnvironment* env);
    /**
     *Carga la tipografia del ToolTip
     * @param env un apuntador al manejador de la gui de irrlicht.
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setMenuToolTip(gui::IGUIEnvironment* env);
    /**
     * Carga la tipografia de los botones que no son del Menu
     * @param env un apuntador al manejador de la tipografa
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setGUIBoton(gui::IGUIEnvironment* env);
    /**
     *Carga la tipografia que se le aplica al titulo de la ventana
     * @param env un apuntador al manejador de la gui de irrlicht
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setGUIWindow(gui::IGUIEnvironment* env);
    /**
     *Carga las texturas del Terreno.
     * @param smgr un apuntador al manejador de escena del juego
     * @param fsys proporciona un apuntador al sistema de archivos
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setTerrain(scene::ISceneManager* smgr, io::IFileSystem* fsys);
    /**
     *Carga la textura del skydome .
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setSkyDome(scene::ISceneManager* smgr);
    /**
     *Carga el skin que será utilizado por la gui.
     * @param env un apuntador al manejador de la gui de irrlicht
     * @param fsys un apuntador al sistema de archivos.
     * @param driver un apuntador al  driver de video
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setSkinGui(gui::IGUIEnvironment* env, io::IFileSystem* fsys,video::IVideoDriver* driver);
    /**
     * Carga los iconos de los botones del menu de la partida.
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setBotonesPartida(scene::ISceneManager* smgr);
    /**
     * Carga la Imagen de los Creditos.
     * @param smgr un apuntador al manejador de escena del juego
     * @throws SkinNoCargado si algun elemento no pudo ser cargado
     */
    void setImagenCreditos(scene::ISceneManager* smgr);
    /**
     * Malla con la que se dibujará el tablero
     */
     scene::IAnimatedMesh* Tablero;
     /**
      * Malla con la que se dibujará la celda
      */
     scene::IAnimatedMesh* Celda;
     /**
      * Malla con la que se dibujará el Jugador 1
      */
     scene::IAnimatedMesh* Jugador1;
     /**
      * Malla con la que se dibujará el Jugador 2
      */
     scene::IAnimatedMesh* Jugador2;
     /**
      * Malla con la que se dibujará la barrera
      */
     scene::IAnimatedMesh* Barrera;
     /**
      * Textura que le será aplicada a la malla de la barrera
      */
     video::ITexture* TBarrera;
     /**
      *  Textura que le será aplicada a la malla de la celda
      */
     video::ITexture* TCelda;
     /**
      * Textura que le será aplicada al fuego
      */
     video::ITexture* TAntorcha;
     /**
      *  Textura que le será aplicada a la malla del tablero
      */
     video::ITexture* TTAblero;
     /**
      * Tipografia que tendrá los botones del menú
      */
     gui::IGUIFont*  MenuBoton;
     /**
      * Tipografia que tendrá la GUI por default
      */
     gui::IGUIFont* Default;
     /**
      * Tipografia que tendrá el ToolTip
      */
     gui::IGUIFont* MenuToolTip;
     /**
      * Tipografia que tendrá los botones que no son del menú
      */
     gui::IGUIFont* GUIBoton;
     /**
      * Tipografia que tendrá el titulo de las Ventana
      */
     gui::IGUIFont* GUIWindow;
     /**
      *Archivo del terreno con las que se dibujan las montañas, valles etc.
      */
     io::IReadFile* heightMapFile;
     /**
      *  Textura que le será aplicada al terreno del juego
      */
     video::ITexture* TTerrain;
     /**
      *  Textura que le será aplicada al skydome
      */
     video::ITexture* Tskydome;
     /**
      * skin de la GUi que sera cargadda a irrlicht.
      */
     gui::CImageGUISkin* skin;
     /**
      * Imagenes de los  iconos de los botones de la partida
      */
     std::vector<video::ITexture*> botonesPartida;
     /**
      * Imagenes de los  iconos de los botones de la partida cuando estan presionados
      */
     std::vector<video::ITexture*> botonesPartidaPresionado;
     /**
      * Imagen de los creditos.
      */
     video::ITexture* creditos;
     /**
     * Contiene los metodos para recuperar los paths de la skin.
     */
    Opciones::ManejadorOpciones *opciones;
};
}
#endif	/* _SKIN_HPP */

