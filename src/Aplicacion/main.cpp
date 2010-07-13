
#include"Aplicacion.hpp"


int main(int argc, char** argv) {
    char op;
    std::cout<<"modo grafico? s/n"<<std::endl;
    std::cin>>op;
    Aplicacion *app=new Aplicacion("","",op=='s'?"AUTO":"NULL",false,250);
    app->run();
    delete(app);
    return 0;
}

