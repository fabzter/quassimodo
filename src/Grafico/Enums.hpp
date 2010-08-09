
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
    B_AYUDA,
    B_SALIR,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    B_COUNT=6

};
/**
 * Este enum, contiene los identificadores para los botones de seleccionar agente.
 */
enum GUI_BOTONES_AGENTES{

    BA_AGENTE_1=10,
    BA_AGENTE_2,
    /*este es sólo para indicar el número maximo de menús que hay, si se desea agregar depues mas botones
     se agregan antes de COUNT*/
    BA_COUNT=2

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
    BO_COUNT=2

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
L"Agente VS Maquina",
L"Agente VS Agente",
L"Opciones",
L"Creditos",
L"Ayuda",
L"Salir",
0
};
/**
 * Arreglo que contiene el ToolTip de cada Boton del Menú
 */
const wchar_t* const GUI_BOTONES_TOLTIP[B_COUNT+1]=
{
L"Partida rápida en el que el segundo competidor es el Agente Integrado en la aplicación",
L"Partida rápida de dos Agentes Inteligentes creados por algun usuario",
L"Cambia algunas propiedades de la Aplicación",
L"Creditos",
L"Ayuda de como usar la Aplicación",
L"Sale de la Aplicación",
0
};
}

#endif	/* _ENUMS_HPP */

