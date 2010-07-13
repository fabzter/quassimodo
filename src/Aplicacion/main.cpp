#include "Aplicacion.hpp"
#include "ManejadorOpciones.hpp"

int main(int argc, char* argv[])
{
    ManejadorOpciones mo(argc, argv);
    Aplicacion *app=new Aplicacion(mo.getAgente(0), mo.getAgente(1),
                                   mo.getVideoMode().c_str(), mo.isFullScreen(),
                                   250);
    app->run();
    delete(app);
    return 0;
}

