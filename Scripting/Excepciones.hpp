/**
 */

#ifndef _EXCEPCIONES_HPP__
#define	_EXCEPCIONES_HPP__
#include <sstream>

namespace Scripting{

class Excepcion: public std::exception
{
    virtual const char* what() const throw();
public:
    Excepcion(std::string what);
    Excepcion();
    ~Excepcion() throw();
protected:
    std::string msg;
};

class ModuloNoCargado: public Excepcion{
public:
     ModuloNoCargado();
     ModuloNoCargado(std::string what);
};


}

#endif	/* _EXCEPCIONES_HPP */

