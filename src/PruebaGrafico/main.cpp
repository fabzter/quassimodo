
#include"Aplicacion.hpp"


int main(int argc, char** argv) {
    char op;
    std::cout<<"modo grafico? s/n"<<std::endl;
    std::cin>>op;
    Aplicacion *app=new Aplicacion("","",op=='y'?"AUTO":"NULL");
    app->run();
   
    return 0;
}

