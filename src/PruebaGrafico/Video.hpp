

#ifndef _VIDEO_HPP
#define	_VIDEO_HPP
#include<string>
#include<irrlicht/irrlicht.h>
#include<iostream>
using namespace irr;
/**
 * Clase encargada de encontrar un  driver de video para la computadora en que se inicie
 * la aplicacion
 */
class Video {
public:
  /**
   *Constructor de la clase Video busca o inicializa el tipo de video segun el parametro dado
   * por default busca el driver adecuado.
   * @param video
   */
    Video(std::string video="AUTO");
    /**
     *Constructor copia de la clase video
     * @param orig un objeti de la clase video
     */
    Video(const Video& orig);
    /**
     *Destructor
     */
    virtual ~Video();
    /**
     *Regresa el tipo de driver que  será utilizado por la aplicacion
     * @return un valor del enum video::E_DRIVER_TYPE
     */
    video::E_DRIVER_TYPE getVideoType();
private:
    /**
     *Busca el tipo de driver que se utilizara en la aplicacion y lo almacena
     */
    void SeleccionaDriverType();
    /**
     *Almacena el tipo de driver que será utilizado por la aplicación
     * @param tipo un valor del enum video::E_DRIVER_TYPE
     */
    void setDriver(video::E_DRIVER_TYPE tipo);
    /**
     * El tipo de video que será usado por la aplicacion
     */
    video::E_DRIVER_TYPE type;

};

#endif	/* _VIDEO_HPP */

