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


int send_comando_AT(char* comando_AT, char* valor_esperado, unsigned int timeout, int respuesta)
{
   
   bool_t received = FALSE;  
   int reintento=0;
   
   uint8_t answer=0;
   uartWriteString( UART_USB, "Enviando comando AT: \r\n" );
   uartWriteString( UART_USB, comando_AT );                   // verificoo el comando AT por usb 
   uartWriteString( UART_232, comando_AT );                   // envio por uart el comando AT
     
   while( retorno == 0 && reintento<=3 ){                   // realizo 3 intentos de enviar el mje y recibir el dato esperado
   
      received = waitForReceiveStringOrTimeoutBlocking(
                    UART_232,
                    comando_AT,
                    strlen(valor_esperado),
                    timeout
                 );


         if( received ){       // si recibe true es porque llego el valor esperado
            uartWriteString( UART_USB, "\r\nLlego el mensaje esperado\r\n" );    // llego el valor esperado
            retorno= 1;                                                      // respuesta = 1 y salgo
            delay(1000);
            return retorno;
            break;
         }
         
         else{                                                  // si no recibe el dato esperado 
            uartWriteString( UART_USB, "\r\nTimeout\r\n" );     // imprimo timeout
            retorno = 0;                                  
            reintento++;    
            delay(2000);            // sumo los reintentos           
            uartWriteString( UART_232, comando_AT );            // vuelvo a enviar el comando AT          
            
         }


   } 
}


