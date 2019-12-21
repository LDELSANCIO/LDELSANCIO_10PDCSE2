/*=====[uart .h]=====================================================
 *Copyright 2019 Del Sancio Leonardo <gumyds@mail.com>
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/10/9
 * Version: 1
 */
/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _GPS_H_
#define _GPS_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/

#include "sapi.h"
/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/
int8_t dato;
uint32_t contador;
 char TramaGps [72];
/*=====[Macros estilo funcion publicas]======================================*/


/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado

// Tipo de datos estructua, union o campo de bits

/*=====[Prototipos de funciones publicas]====================================*/

//void LED_encender( gpioMap_t lampara );

//void LED_apagar( gpioMap_t lampara );

//void LED_titilar( gpioMap_t lampara, int32_t tiempoON , uint8_t cantVeces);

char gps (char TramaGps[]);

/*=====[Prototipos de funciones publicas de interrupcion]====================*/


/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _LEDS_H_ */
