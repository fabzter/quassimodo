
#ifndef __ENUMS_HPP_
#define	__ENUMS_HPP_
namespace Grafico{
/**
 * Enum que contiene los identificadores para cada boton en em Menú de la aplicación
 */
enum GUI_BOTONES{

    B_AGENTE_VS_MAKINA=0,
    B_AGENTE_VS_AGENTE,
    B_OPCIONES,
    B_CREDITOS,
    B_SALIR,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    B_COUNT=5

};
/**
 * Enum que contiene los identificadores de las opciones del cuadro de dialogo de selecciona agentes
 */
enum GUI_BOTONES_OPCION{

    BO_INICIA=100,
    BO_CANCELA,
    BO_CREDITOS,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BO_COUNT=3

};
/**
 *  Enum que contiene los identificadores de las opciones del a partida como son Vistas, salir al menú y pausa
 */
enum GUI_BOTONES_PARTIDA{

    BP_VISTA1=1000,
    BP_VISTA2,
    BP_VISTA3,
    BP_VISTA4,
    BP_PAUSA,
    BP_MENU,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BP_COUNT=6

};
/**
 * Enum que contiene los identificadores de las opciones para que un humano pueda jugar la partida.
 */
enum GUI_BOTONES_JUGADOR{

    BJ_MOVER=300,
    BJ_BARRERA,
    BJ_GIRA_ESTE,
    BJ_GIRA_NORTE,
    BJ_LISTO,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BJ_COUNT=5

};
/**
 * Enum que contiene los identificadores de los cuadros de dialogo. si es de advertencia, error, etc;
 */
enum GUI_BOTONES_OK{
    BOK_ERROR=200,
    BOK_ADVERTENCIA,
    BOK_CREDITOS,
    BOK_INFO,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BOK_COUNT=5
};

/**
 * Arreglo en el cual se contiene el texto que tendrá el menu del juego
 */
const wchar_t* const GUI_BOTONES_TEXTO[B_COUNT+1]=
{
L"Humano VS Máquina",
L"Humano VS Humano",
L"Opciones",
L"Créditos",
L"Salir",
0
};
/**
 * Arreglo que contiene el ToolTip de cada Boton del Menú
 */
const wchar_t* const GUI_BOTONES_TOLTIP[B_COUNT+1]=
{
L"Partida rápida en el que el segundo competidor es el Agente Integrado en la aplicación",
L"Partida rápida de dos usuarios",
L"Cambia algunas propiedades de la Aplicación",
L"Créditos",
L"Sale de la Aplicación",
0
};
const wchar_t* const GUI_BOTONES_TOLTIP_PARTIDA[2]=
{
L"Regresa al menú principal",
0
};
}

#endif	/* _ENUMS_HPP */

