/**
 */

#ifndef _ENUMS_HPP
#define	_ENUMS_HPP

namespace Reglas{
/**
     * Este enum contiene los tipos de movimiento disponibles para una Jugada
     */
    enum TipoDeJugada{
        BARRERA,
        MOVIMIENTO
    };

    /**
     * Este enum contiene las direcciones posibles para la Jugada.
     */
    enum Direccion{
        NORTE = 0,
        ESTE = 1,
        SUR = 2,
        OESTE = 3
    };

}
#endif	/* _ENUMS_HPP */

