/**
 */

#ifndef _EXCEPCIONES_HPP__
#define	_EXCEPCIONES_HPP__
#include <sstream>

namespace Scripting{

class Excepcion: public std::exception
{
public:
    virtual const char* what() const throw();
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

class ScriptMalo: public Excepcion{
public:
     ScriptMalo();
     ScriptMalo(std::string what);
};

}

#endif	/* _EXCEPCIONES_HPP */

