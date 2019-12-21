/*=====[modulo UART]=====================================================
 *Copyright 2019 Del Sancio Leonardo <gumyds@mail.com>
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/10/9
 * Version: 1
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "send.h"
#include "string.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/
int retorno;


/*=====[Macros de definicion de constantes privadas]=========================*/
DEBUG_PRINT_ENABLE
/*=====[Macros estilo funcion privadas]======================================*/

/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico

// Tipo de datos de puntero a funcion

// Tipo de datos enumerado

// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/

/*=====[Definiciones de Variables globales publicas]=========================*/

/*=====[Definiciones de Variables globales privadas]=========================*/



/*=====[Prototipos de funciones privadas]====================================*/

//static void toggleLED( gpioMap_t lampara);

/*=====[Implementaciones de funciones publicas]==============================*/


int enviar_dato (int dato_sim)
{
   uint8_t respuesta=0;
   send_comando_AT("AT+CIPMODE=0\r\n", "OK" , 3000, respuesta );  // UTILIZO LA FUNCION SEND PARA VERIFICAR LOS DATOS QUE LLEGAN DEL MODULO
   delay(1000);                                     // PONGO DELAY BLOQUEANTES NO ME INTERESA OTRA TAREA MAS QUE MANDAR DATOS "POR AHORA"
  send_comando_AT("AT+CIPOPEN=0,\"TCP\",\"api.thingspeak.com\",80\r\n","OK" , 3000, respuesta ); 
   delay(1000); 
  send_comando_AT("AT+CIPSEND=0,74\r\n",">" , 3000, respuesta ); 
  delay(1000); 
  uartWriteString( UART_232, "GET https://api.thingspeak.com/update?api_key=OHGDQY3AZGDFVMB5&field1=0\r\n" );
  delay(1000); 
 send_comando_AT("AT+CIPMODE=0\r\n", "OK" , 3000, respuesta ); 
 delay(500); 
  uartWriteString( UART_USB, "GET https://api.thingspeak.com/update?api_key=OHGDQY3AZGDFVMB5&field1=0\r\n" );
 delay(600); 
  
  
}










