/* 
 * File:   Grafo.cpp
 * Author: fabrizio
 * 
 * Created on 31 de enero de 2010, 04:32 PM
 */

#include <vector>

#include "Grafo.hpp"
#include "Tablero.hpp"

Reglas::Grafo::Grafo(Tablero &tablero)
{
    this->tablero = &tablero;

    this->init_grafo();
}

Reglas::Grafo::Grafo(const Grafo& orig)
{
}

Reglas::Grafo::~Grafo()
{
}

void Reglas::Grafo::init_grafo()
{
    for(int i = 0; i < this->tablero->size_y; i++)
    {
        for(int j = 0; j < this->tablero->size_x; j++)
        {//navegamos por el Tablero
            Celda *celda = (Celda*)&this->tablero->getCelda(j, i);
            std::vector<int> pos(celda->getPosicion());

            //Si la Celda está hasta el NORTE, la conectamos por el NORTE con la
            //meta_norte
            if(pos.at(1) == this->tablero->size_y - 1)
            {
                celda->setHijo(NORTE, this->meta_norte);
            }
            //Si no esta hasta el NORTE, la conectamos por el NORTE on la Celda
            //de arriba.
            else
            {
                celda->setHijo(NORTE, this->tablero->getCelda(j, i + 1));
            }

            //Si la celda está hasta el SUR, la conectamos por el SUR con la
            //meta_sur.
            if(pos.at(1) == 0)
            {
                celda->setHijo(SUR, this->meta_sur);
            }
            //Si no está hasta el SUR, la conectamos por el SUR con la Celda de
            //abajo.
            else
            {
                celda->setHijo(SUR, this->tablero->getCelda( j, i - 1));
            }

            //Si la Celda está hasta el OESTE, bloqueamos su lado OESTE.
            if(pos.at(0) == 0)
            {
                celda->bloquearDireccion(OESTE);
            }
            //Si no está hasta el OESTE, la conectamos por el OESTE con la Celda
            //de la izquierda.
            else
            {
                celda->setHijo(OESTE, this->tablero->getCelda(j - 1, i));
            }

            //Si la Celda está hasta el ESTE, bloqueamos su lado ESTE.
            if(pos.at(0) == this->tablero->size_x - 1)
            {
                celda->bloquearDireccion(ESTE);
            }
            //Si no está hasta el ESTE, la conectamos por el ESTE con la Celda
            //de la derecha
            else
            {
                celda->setHijo(ESTE, this->tablero->getCelda(j + 1, i));
            }
        }
    }
}

bool Reglas::Grafo::hayCaminoMeta(int idJugador) const
{
  std::list<Celda*> visitados;

  return
    this->busqueda_recursiva( this->tablero->getCelda(idJugador),
                           idJugador == 0 ? this->meta_norte: this->meta_sur,
                           visitados);
}

bool Reglas::Grafo::busqueda_recursiva(const Celda& inicio, const Celda& meta,
                                       std::list<Celda*>& visitados) const
{
  //revisamos si la meta es alcanzable a través de inicio
  for(int i = (int)NORTE; i <= (int)OESTE; i++)
  {
    if(inicio.getHijo((Direccion)i) == meta)
      return true;
  }

  visitados.push_front((Celda*)&inicio);

  for(int i = (int)NORTE; i <= (int)OESTE; i++)
  {
    //revisamos si se puede avanzar y que no está en visitados...
    if(inicio.estaLibreDireccion((Direccion)i) && 
       std::count(visitados.begin(), visitados.end(),
                  &inicio.getHijo((Direccion)i)) == 0 )
    {
      if(this->busqueda_recursiva(inicio.getHijo((Direccion)i), meta, visitados) )
        return true;
    }
  }

  return false;

}