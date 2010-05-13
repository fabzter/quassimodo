

#ifndef __EXEPCIONES_HPP__
#define	__EXEPCIONES_HPP__
#include <sstream>
namespace Grafico{

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

class SkinNoCargado: public Excepcion{
public:
     SkinNoCargado();
     SkinNoCargado(std::string what);
};


}

#endif	/* _EXEPCIONES_HPP */

