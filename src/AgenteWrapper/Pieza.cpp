#include <boost/python.hpp>
#include <Reglas/Pieza.hpp>
using namespace boost::python;
using namespace Reglas;

struct PiezaWrapper: Pieza, wrapper<Pieza>
{
    const std::vector<int> &getPosicion() const
    {
        if(override getPosicion = this->get_override("getPosicion"))
            return getPosicion();
        return Pieza::getPosicion();
    }
    const std::vector<int> &default_getPosicion() const
    {
        return this->Pieza::getPosicion();
    }
    
    bool operator ==(const PiezaWrapper& otro) const
    {
        if(override o = this->get_override("__eq__"))
            return o(otro);
        return Pieza::operator ==(otro);
    }

};

void export_pieza()
{
    class_<PiezaWrapper, boost::noncopyable>("Pieza")
    
    .def("getPosicion", &Pieza::getPosicion, &PiezaWrapper::default_getPosicion, 
            return_value_policy<copy_const_reference>() )
    .def(self == self)
    ;
}
