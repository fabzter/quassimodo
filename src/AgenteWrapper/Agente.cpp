#include <pybind11/pybind11.h>
#include <Reglas/Agente.hpp>

namespace py = pybind11;
using namespace Reglas;

struct PyAgente : Agente {
    using Agente::Agente;
    Jugada siguienteJugada() override {
        PYBIND11_OVERRIDE_PURE(Jugada, Agente, siguienteJugada);
    }
    void iniciar(int id) override {
        PYBIND11_OVERRIDE_PURE(void, Agente, iniciar, id);
    }
    void terminar() override {
        PYBIND11_OVERRIDE_PURE(void, Agente, terminar);
    }
};

void export_agente(py::module_& m)
{
    py::class_<Agente, PyAgente>(m, "Agente")
        .def(py::init<>())
        .def("siguienteJugada", &Agente::siguienteJugada)
        .def("iniciar", &Agente::iniciar)
        .def("terminar", &Agente::terminar);
}
