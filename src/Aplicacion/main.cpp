#include "Aplicacion.hpp"
#include "ManejadorOpciones.hpp"

int main(int argc, char* argv[])
{
    ManejadorOpciones mo(argc, argv);
    Aplicacion *app = new Aplicacion(mo.getAgentePath(0), mo.getAgentePath(1),
                                   mo.getVideoMode().c_str(), mo.isFullScreen(),
                                   mo.getVelocidad());
    app->run();
    delete(app);
    return 0;
}

