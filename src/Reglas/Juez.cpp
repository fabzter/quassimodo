/**
 */

#include <vector>
#include <iostream>

#include "Juez.hpp"

Reglas::Juez::Juez(Tablero& t)
{
  this->tablero = &t;
}

Reglas::Juez::Juez(const Juez& orig)
{
    this->tablero = orig.tablero;
}

Reglas::Juez::~Juez()
{
}

Reglas::Jugada Reglas::Juez::siguienteJugada(int idJugador)
{
    AyudanteDeAgente ayudante(this->tablero);
    Jugada j(this->tablero->getJugador(idJugador).siguienteJugada(*this->tablero,
                                                                    ayudante));
    this->revisar_reglas(j, idJugador);
    return j;
}

int Reglas::Juez::hayGanador()
{
    for(int id = 0; id < this->tablero->jugadores.size(); id++)
    {
        const Celda &meta = this->tablero->grafo->getMeta(id);
        //Si la meta que le corresponde al Jugador con id id es hija de la celda
        //donde se encuentra el mismo jugador....
        if(this->tablero->getCelda(id).tieneHijo(meta) )
            return id;
    }
    return -1;
}

void Reglas::Juez::revisar_reglas(Jugada& j, int idJugador)
{
  this->regla_1(j, idJugador);
  this->regla_2(j, idJugador);
  this->regla_3(j, idJugador);
  this->regla_4(j, idJugador);
  this->regla_5(j, idJugador);
  this->regla_6(j, idJugador);
  this->regla_7(j, idJugador);
}

void Reglas::Juez::regla_1(Jugada& j, int idJugador)
{
    Celda celdaActual (this->tablero->getCelda(idJugador) );
    std::ostringstream strs;

    if( j.getTipoDeJugada() == MOVIMIENTO )
    {
        std::vector<int> pos_actual(celdaActual.getPosicion());
        //Si se queda en la misma posicion, mandamos una excepcion avisando!
        if( j.getPosicion().at(0) == pos_actual.at(0) &&
                j.getPosicion().at(1) == pos_actual.at(1) )
        {
            strs << "El Jugador " <<  idJugador << " ha hecho una Jugada de "
                    "tipo MOVIMIENTO, pero se ha quedado en su misma posicion: "
                    "(" << pos_actual.at(0) << ',' << pos_actual.at(1) <<
                    ')';
            throw ReglasRotas(strs.str().c_str());
        }
    }
    else if(j.getTipoDeJugada() != BARRERA)
    {
        //Si no es ni MOVIMIENTO ni BARRERA, es un error!
         strs << "El Jugador " << idJugador << " realizo un tipo de Jugada "
                "desconocida (" << j.getTipoDeJugada() << ").";
        throw ReglasRotas(strs.str().c_str());
    }

}

void Reglas::Juez::regla_2(Jugada& j, int idJugador)
{
    std::ostringstream strs;

    //se revisa que no se coloque fuera.
    if(j.getPosicion().at(0) >= tablero->size_x ||
       j.getPosicion().at(0) < 0 ||
       j.getPosicion().at(1) >= tablero->size_y ||
       j.getPosicion().at(1) < 0)
    {
        strs << "El Jugador " << idJugador << " trato de colocar una pieza "
                "fuera del tablero, en: " << '(' << j.getPosicion().at(0)
                << ',' << j.getPosicion().at(1) << ").";
        throw ReglasRotas(strs.str().c_str());
    }
    //se revisa que la barrera no se salga
    if(j.getTipoDeJugada() == BARRERA)
    {
        Barrera b(j);

        if(b.getPuntoMedio().at(0) >= this->tablero->size_x ||
           b.getPuntoMedio().at(0) < 0 ||
           b.getPuntoMedio().at(1) >= this->tablero->size_y ||
           b.getPuntoMedio().at(1) < 0)
        {
            strs << "El Jugador " << idJugador << " trato de colocar una Barrera "
                "que sale del tablero, en: " << '(' << j.getPosicion().at(0)
                << ',' << j.getPosicion().at(1) << ").";
            throw ReglasRotas(strs.str());
        }
    }
}

void Reglas::Juez::regla_3(Reglas::Jugada& j, int idJugador)
{
    if(j.getTipoDeJugada() != MOVIMIENTO)
        return;

    std::ostringstream strs;
    int dir_jugada =
            this->getDireccionMovimiento(j.getPosicion(),
                             this->tablero->getCelda(idJugador).getPosicion());
    if(dir_jugada != -1)
    {
        try
        {
            this->tablero->getCelda(idJugador).getHijo((Direccion)dir_jugada);
        }
        catch(SinHijo &e)
        {
            strs << "El Jugador " << idJugador << " intento moverse en hacia una"
                    "direccion bloqueada por una barrera: (" <<
                    j.getPosicion().at(0) << ',' << j.getPosicion().at(1) <<
                    ").";
            throw ReglasRotas(strs.str());
        }
    }
}

void Reglas::Juez::regla_4(Jugada& j, int idJugador)
{
    if(j.getTipoDeJugada() != MOVIMIENTO)
        return;
    
    std::ostringstream strs;
    bool res = false;
    int dir_jugada = (int)j.getDireccion();
    
    const Celda &celdaActual = this->tablero->getCelda(idJugador);
    const Celda &celdaJugada = this->tablero->getCelda(j.getPosicion());

    res = celdaActual.tieneHijo(celdaJugada);

    if(!celdaJugada.estaLibre())
    {
        try
        {
            strs << "El Jugador " << idJugador << " intento colocarse sobre el Jugador "
                "contrario, en la celda: (";
            strs <<
                celdaActual.getHijo(j.getDireccion()).getPosicion().at(0)
                << ',' <<
                celdaActual.getHijo(j.getDireccion()).getPosicion().at(1)
                <<").";
        }
        catch(SinHijo &e){}
        throw ReglasRotas(strs.str());
  }

  if(!res)
  {
      //si se quiere llegar a una celda que no es hijo de la actual...
      //nos movemos una posicion en la direccion de la Jugada.
      //calculamos la direccion
      std::vector<int> vect_dir(this->getVectDireccion(j.getPosicion(),
                                                    celdaActual.getPosicion()));
      dir_jugada = this->getDireccionMovimiento(vect_dir);
      //si se meueve en diagonal
      if(dir_jugada == -1)
      {
          //si se mueve mas de un lugar
          if( !( (vect_dir.at(0) == 1 || vect_dir.at(0) == -1) &&
             (vect_dir.at(1) == 1 || vect_dir.at(1) == -1) ) )
              res=false;
          
          //si se mueve un lugar
          else{
              //obtenemos la direccion Horizontal del movimiento
              std::vector<int> vect_dir_A = j.getPosicion();
              Direccion dir_H=(Direccion)this->getDireccionMovimiento(vect_dir_A,celdaActual.getPosicion());
              //obtenemos la direccion Vertical del movimiento
              vect_dir_A = celdaActual.getPosicion();
              Direccion dir_V=(Direccion)this->getDireccionMovimiento(vect_dir_A,celdaActual.getPosicion());

              try
              {
                  res = ( !celdaActual.getHijo(dir_V).estaLibre() ||
                          !celdaActual.getHijo(dir_H).estaLibre() );
              }
              catch(SinHijo &e){}
          }
      }
      else
      {
          this->regla_3(j, idJugador);
          Celda& hijo = celdaActual.getHijo((Direccion)dir_jugada);
            if( !hijo.estaLibre() )
            {
                try
                {
                    //revisamos si la celda de la Jugada
                    const Celda &nieto = hijo.getHijo((Direccion)dir_jugada);
                    if( ! (nieto == celdaJugada) )
                        res = false;
                    else
                    res = true;
                }
                catch(SinHijo &e)
                {
                    strs << "El Jugador " << idJugador << " esta tratando de "
                            "llegar a una celda bloqueada por una barrera.";
                    throw ReglasRotas(strs.str());
                }
            }
      }
  }

  if(!res)
  {
   strs << "El Jugador " << idJugador << " intento colocarse sobre "
                      "una celda que no esta a su alcance: (" <<
                      celdaJugada.getPosicion().at(0) << ',' <<
                      celdaJugada.getPosicion().at(1) <<").";
              throw ReglasRotas(strs.str());
  }
}

int Reglas::Juez::getDireccionMovimiento(const std::vector<int> &vect_dir)
{
    if(vect_dir.at(0) == 0)
    {
        return vect_dir.at(1) > 0? NORTE : SUR;
    }
    else if (vect_dir.at(1) == 0)
    {
        return vect_dir.at(0) > 0? ESTE: OESTE;
    }
    return -1;
}
int Reglas::Juez::getDireccionMovimiento(const std::vector<int> &dir_jugada,
                                        const std::vector<int> &dir_actual)
{
     std::vector<int> vect_dir(getVectDireccion(dir_jugada,dir_actual));
     return this->getDireccionMovimiento(vect_dir);
}
std::vector<int> Reglas::Juez::getVectDireccion(const std::vector<int> &dir_jugada,
                                        const std::vector<int> &dir_actual)
{
    std::vector<int> vect_dir;
    vect_dir.push_back(dir_jugada.at(0) - dir_actual.at(0));
    vect_dir.push_back(dir_jugada.at(1) - dir_actual.at(1));

    return vect_dir;
}

void Reglas::Juez::regla_5(Reglas::Jugada& j, int idJugador)
{
    std::ostringstream strs;
    if(this->tablero->getJugador(idJugador).getBarrerasDisponibles() <= 0
       && j.getTipoDeJugada() == BARRERA)
    {
        strs << "El jugador " << idJugador << " intento colocar una barrera "
                "cuando ya no tenia mas barreras disponibles.";
        throw ReglasRotas(strs.str());
    }
}

void Reglas::Juez::regla_6(Reglas::Jugada& j, int idJugador)
{
    if(j.getTipoDeJugada() != BARRERA)
        return;

    std::ostringstream strs;
    strs << "El jugador " << idJugador << " intento colocar una barrera en (" <<
            j.getPosicion().at(0) << ',' << j.getPosicion().at(1) << ") que se "
            "traslapa con otra.";

    Barrera b(j);

    std::list<Barrera>::iterator it;
    for(it = this->tablero->barreras_colocadas.begin();
        it != this->tablero->barreras_colocadas.end(); it++)
    {
        if(j.getDireccion() == it->getDireccion())
        {
            if( (b.getPosicion().at(0) == it->getPosicion().at(0) &&
               b.getPosicion().at(1) == it->getPosicion().at(1))
                 ||
               (it->getPuntoMedio().at(0) == b.getPosicion().at(0) &&
               it->getPuntoMedio().at(1) == b.getPosicion().at(1))
                ||
               (b.getPuntoMedio().at(0) == it->getPosicion().at(0) &&
                b.getPuntoMedio().at(1) == it->getPosicion().at(1))
                )
            {
                throw ReglasRotas(strs.str());
            }
        }
        else
        {
            if(b.getPuntoMedio().at(0) == it->getPuntoMedio().at(0) &&
               b.getPuntoMedio().at(1) == it->getPuntoMedio().at(1))
            {
                throw ReglasRotas(strs.str());
            }
        }
    }
}

void Reglas::Juez::regla_7(Reglas::Jugada& j, int idJugador)
{
    if(j.getTipoDeJugada() != BARRERA)
        return;
    std::ostringstream strs;

    Barrera b;
    b.colocar(j.getPosicion(), j.getDireccion());

    if(this->estaPuestaEnOrillas(b))
    {
        strs << "EL Jugador " << idJugador << " intento colocar una barrera en un"
                "sitio y direccion no permitida. (una orilla del tablero)";
        throw ReglasRotas(strs.str());
    }

    this->tablero->setBarrera(idJugador, b);

    bool hayCamino = true;
    for(int i = 0; i < this->tablero->jugadores.size(); i++)
    {
        int id = this->tablero->jugadores.at(i)->getIdentificador();
        bool temp = this->tablero->grafo->hayCaminoMeta(id);
        hayCamino = hayCamino && temp;
    }
    this->tablero->quitarBarrera(idJugador, b);

    if(!hayCamino)
    {
        strs << "El jugador " <<idJugador << " intento colocar una barrera en ("
                << j.getPosicion().at(0) << ',' << j.getPosicion().at(1) <<
                ") que cierra el camino a la meta.";
        throw ReglasRotas(strs.str());
    }
}

bool Reglas::Juez::estaPuestaEnOrillas(Barrera& b)
{
    //Si esta en la orilla ESTE u OESTE y apunta al NORTE
    bool res = ( ((b.getPosicion().at(0) == 0 || b.getPosicion().at(0) == 9) &&
       b.getDireccion() == NORTE)
         || //ó
       //Si esta en la orilla SUR o NORTE y apunta al ESTE
       ((b.getPosicion().at(1) == 0 || b.getPosicion().at(1) == 9) &&
       b.getDireccion() == ESTE)
       );
    return res;
}