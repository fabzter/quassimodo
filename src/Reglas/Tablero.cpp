/**
 */
#include "Tablero.hpp"

Reglas::Tablero::Tablero()
{
    //le damos el tamaño al tablero y construimos las Celdas.
    this->init_tablero();
    //Posicionamos las Celdas en el Grafo y las conectamos correctamente.
    this->grafo = new Grafo(*this);
}

Reglas::Tablero::Tablero(const Tablero& orig)
{
    //le damos el tamaño al tablero y construimos las Celdas.
    this->init_tablero();
    //Posicionamos las Celdas en el Grafo y las conectamos correctamente.
    this->grafo = new Grafo(*this);
    
    //iteramos por todas las celdas
    for(int y = 0; y < this->size_y; y++)
    {
        for(int x = 0; x < this->size_x; x++)
        {
            if(!orig.datos.at(y).at(x).estaLibre())
            {
                this->datos.at(y).at(x).bloquear();
                this->celdas_ocupadas.push_back((Celda*)&orig.datos.at(y).at(x));
            }
            //iteramos por los lados de las celdas
            for(int lado = (int)NORTE; lado <= (int)OESTE; lado++)
            {
                if( !orig.datos.at(y).at(x).estaLibreDireccion((Direccion)lado) )
                {
                    this->datos.at(y).at(x).bloquearDireccion((Direccion)lado);
                }
            }
        }
    }
    this->barreras_colocadas = orig.barreras_colocadas;

    this->jugadores.resize(orig.jugadores.size());
    for(int i = 0; i < this->jugadores.size(); i++)
    {
        this->jugadores.at(i) = new Jugador(*orig.jugadores.at(i));
    }

    //copiamos celdas ocupadas.
    this->celdas_ocupadas.resize(orig.celdas_ocupadas.size());
    for(int i = 0; i < orig.celdas_ocupadas.size(); i++)
    {
        this->celdas_ocupadas.at(i) =
                (Celda*)&this->getCelda(orig.celdas_ocupadas.at(i)->getPosicion());
    }
}

Reglas::Tablero::~Tablero()
{
}

void Reglas::Tablero::init_tablero()
{
    //le damos el tamaño vertical
    this->datos.reserve(this->size_y);
    this->datos.resize(this->size_y);

    //ahora el tamaño horizontal
    for(std::size_t i = 0; i < this->datos.size(); i++){
        this->datos.at(i).reserve(this->size_x);
        this->datos.at(i).resize(this->size_x);
    }

    //ahora le damos su posición a cada celda...
    for(std::size_t i = 0; i < this->datos.size(); i++){
        for(std::size_t j = 0; j < this->datos.at(i).size(); j++){
            this->datos.at(i).at(j).colocar(j, i);
        }
    }

    //ponemos a los jugadores en blanco.
    this->jugadores.reserve(this->num_jugadores);
}

void Reglas::Tablero::setJugadores(const std::vector<Jugador*> &jugadores)
{
    std::cout<<"en reglas setjugadores iddel 0 ="<<jugadores.size()<<std::endl;
    this->jugadores.resize(jugadores.size());
    this->jugadores = jugadores;

    Celda *c1 = &(this->datos.at(0).at(this->size_x/2));
    Celda *c2 = &(this->datos.at(this->size_y - 1).at(this->size_x/2));
    //marcamos como ocupados los lugares
    c1->bloquear(true);
    c2->bloquear(true);
    //marcamos los jugadores como colocados
    std::vector<int> *_pos = new std::vector<int>( c1->getPosicion() );
    this->jugadores.at(0)->colocar(*_pos);
    delete _pos;
    _pos = new std::vector<int>( c2->getPosicion() );
    this->jugadores.at(1)->colocar(*_pos);
    delete _pos;
    //ponemos las celdas en ocupadas.
    this->celdas_ocupadas.resize(this->num_jugadores);
    this->celdas_ocupadas.at(0) = c1;
    this->celdas_ocupadas.at(1) = c2;
}

void Reglas::Tablero::setBarrera(Reglas::Jugador &j,
        const Reglas::Barrera &b) //TODO: dividir en funciones :(
{
    //vemos la direccion de las primeras Celdas que vamos a bloquearles.
    Direccion d_bloqueada = b.getDireccion() == NORTE? OESTE: SUR;
    //encontramos las celdas a las que vamos a bloquearles la direccion.
    Celda *celdas[this->tam_barrera];
    celdas[0] = &this->datos.at(b.getPosicion().at(1)).at(b.getPosicion().at(0));
    celdas[1] = &this->datos.at(b.getPuntoMedio().at(1)).at(b.getPuntoMedio().at(0));

    for(int i = 0; i < this->tam_barrera; i++)
    {
        celdas[i]->bloquearDireccion(d_bloqueada);
    }
    //agregamos la barrera a barreras colcoadas
    this->barreras_colocadas.push_front(b);
    //se la quitamos al Jugador.
    j.decBarrerasDisponibles();
}

void Reglas::Tablero::setBarrera(int idJugador, const Reglas::Barrera &b)
{
    this->setBarrera(*(Jugador*)&this->getJugador(idJugador), b);
}

void Reglas::Tablero::quitarBarrera(Reglas::Jugador &j, Reglas::Barrera &b)
{
    //revisamos que b este en barreras disponibles
    if( std::find(this->barreras_colocadas.begin(),this->barreras_colocadas.end(), b)
                  == this->barreras_colocadas.end() )
    {
        return;
    }
    //vemos la direccion de las primeras Celdas que vamos a desbloquearles.
    Direccion d_bloqueada = b.getDireccion() == NORTE? OESTE: SUR;
    //encontramos las celdas a las que vamos a desbloquearles la direccion.
    Celda *celdas[this->tam_barrera];
    celdas[0] = &this->datos.at(b.getPosicion().at(1)).at(b.getPosicion().at(0));
    celdas[1] = &this->datos.at(b.getPuntoMedio().at(1)).at(b.getPuntoMedio().at(0));

    for(int i = 0; i < this->tam_barrera; i++)
    {
        //Calculamos la posicion de la celda hijo.
        std::vector<int> pos_hijo(celdas[i]->getPosicion());
        if(d_bloqueada == OESTE)
            pos_hijo.at(0) -= 1;
        else
            pos_hijo.at(1) -= 1;
        celdas[i]->desbloquearDireccion(d_bloqueada,
                                        *(Celda*)&this->getCelda(pos_hijo));
    }
    //se la regresamos al Jugador.
    j.incBarrerasDisponibles();
    //quitamos la barrera de la lista.
    this->barreras_colocadas.erase( std::find(this->barreras_colocadas.begin(),
                                              this->barreras_colocadas.end(),
                                              b)
                                    );
}
void Reglas::Tablero::quitarBarrera(int idJugador, const Reglas::Barrera &b)
{
    Barrera b2(b);
    this->quitarBarrera(*(Jugador*)&this->getJugador(idJugador), b2);
}

void Reglas::Tablero::moverJugador(int jugador, Reglas::Celda& celdaNueva)
{
    if(!this->getJugador(jugador).estaColocado())
    {
        std::ostringstream strs;
        strs << "Se intento mover al Jugador " << jugador << " sin haberlo "
                "colocado antes.";
        throw JugadorNoColocado(strs.str());
    }
    //movemos al Jugador
    this->jugadores.at(jugador)->mover(celdaNueva.getPosicion());
    //actualizamos las celdas ocupadas.
    this->celdas_ocupadas.at(jugador)->bloquear(false);
    this->celdas_ocupadas.at(jugador) = &celdaNueva;
    this->celdas_ocupadas.at(jugador)->bloquear(true);
}

void Reglas::Tablero::moverJugador(int jugador, const std::vector<int> &pos)
{
    if(pos.size() != 2)
    {
        std::ostringstream strs;
        strs << "Se le paso un vector de posicion con un tamanho de " <<
                pos.size() <<" al metodo moverJugador de la clase Tablero.";
        throw ParametrosMalos(strs.str());
    }
    this->moverJugador(jugador, this->datos.at(pos.at(1)).at(pos.at(0)) );
}

void Reglas::Tablero::moverJugador(int jugador, int x, int y)
{
    std::vector<int> pos_jugador(this->datos.at(y).at(x).getPosicion());
    this->moverJugador(jugador, pos_jugador);
}

bool Reglas::Tablero::tieneCelda( const Reglas::Celda& celda) const
{
    std::vector<int> pos(celda.getPosicion());
    bool tiene = true;
    try{
        this->datos.at(pos.at(1) ).at(pos.at(0) );
    }catch(std::out_of_range& e){
        tiene = false;
    }

    return tiene;
}

const std::vector<int> &Reglas::Tablero::getPosicion(int idJugador) const
{
    return this->getJugador(idJugador).getPosicion();
}

const std::list<Reglas::Barrera>& Reglas::Tablero::getBarrerasColocadas() const
{
    return this->barreras_colocadas;
}

const std::vector<Reglas::Celda>& Reglas::Tablero::getMetas(int idJugador) const
{
    if(idJugador >= this->num_jugadores || idJugador < 0)
        throw JugadorNoColocado(); //TODO: info del error.

    int indice = idJugador == 0? 8: 0;

    return this->datos.at(indice);
}

const Reglas::Celda& Reglas::Tablero::getCelda(const std::vector<int> &pos) const
{
    if(pos.size() != 2)
    {
        std::ostringstream strs;
        strs << "Se le paso un vector de posicion con un tamanho de " <<
                pos.size() <<" al metodo moverJugador de la clase Tablero.";
        throw ParametrosMalos(strs.str());
    }
   return this->datos.at(pos.at(1)).at(pos.at(0));
}

const Reglas::Celda &Reglas::Tablero::getCelda(int idJugador) const
{
    return this->getCelda(this->getJugador(idJugador).getPosicion());
}

const Reglas::Celda& Reglas::Tablero::getCelda(int x, int y) const
{
    return this->datos.at(y).at(x);
}

const Reglas::Jugador& Reglas::Tablero::getJugador(int id) const
{
    //revisamos que haya un jugador con ese id
    if(!this->tieneJugador(id))
        throw JugadorNoColocado();
    //obtenemos al dichoso jugador.
    Jugador* j = NULL;
    for(int i = 0; i < this->jugadores.size(); i++)
    {
        if(this->jugadores.at(i)->getIdentificador() == id)
            j = this->jugadores.at(i);
    }
    return *j;
}

bool Reglas::Tablero::tieneJugador(int id) const
{
    Jugador* j = NULL;
    for(int i = 0; this->jugadores.size() > 0 && i < this->jugadores.size(); i++)
    {
        if(this->jugadores.at(i)->getIdentificador() == id)
            j = this->jugadores.at(i);
    }
    return !(j == NULL);
}

std::ostream& operator<< (std::ostream &out, const Reglas::Tablero &tab)
{
    Reglas::Tablero *tablero = (Reglas::Tablero*)&tab;
    
    for(int i = tablero->size_y - 1; i >= 0  ; i--){
        int k;
        int esta_libre;

        for(k = 0; k < tablero->size_x; k++){ //imprimimos la barrera Norte.
            esta_libre=
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::NORTE)?
                        1 : 0;
            out << ' ';
            esta_libre? out << ' ' : out << '_';
            out <<"  " ;
        }
        out << '\n';

        //aqui primero imprimimos la barrera oeste, luego la celda,  luego la barrera este.
        for(k = 0; k < tablero->size_x; k++)
        {
            esta_libre =
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::OESTE)?
                        1: 0;
            esta_libre? out << ' ' : out << '|';

            esta_libre =
                    tablero->getCelda(k, i).estaLibre()? 1: 0;
            out << esta_libre;

            esta_libre =
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::ESTE)?
                        1: 0;
            esta_libre? out << "  " : out << "| ";
        }
        out << '\n';

        //ahora imprimimos la disponibilidad de la barrera sur.
        for(k = 0; k < tablero->size_x; k++){
            esta_libre=
                    tablero->getCelda(k, i).estaLibreDireccion(Reglas::SUR)?
                        1 : 0;
            out << ' ';
            esta_libre? out << ' ' : out << '_';
            out <<"  " ;
        }
        out << '\n';
    }

    return out;
}