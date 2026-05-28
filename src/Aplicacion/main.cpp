#include "Aplicacion.hpp"
#include <Opciones/ManejadorOpciones.hpp>

int main(int argc, char *argv[]) {
  Opciones::ManejadorOpciones mo(argc, argv);
  Aplicacion *app = new Aplicacion(mo);
  app->run();
  delete (app);
  return 0;
}
