/* Copyright 2018, Eric Pernia.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "sapi.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
typedef enum
{
   STATE_CONFIG_AT,
   STATE_CONFIG_1,
   STATE_CONFIG_2,
   STATE_CONFIG_3,
   STATE_CONFIG_4
} fsmEstado_t;

void fsmButtonError( void );
void fsmInicio7600( void );
void fsmConfig7600( void  );


fsmEstado_t fsmEstado;

TickType_t tiempo_down;
TickType_t tiempo_up;
TickType_t tiempo_diff;
static uint8_t conectado=0;
int respuesta=0;
uint32_t contador=0;
int retorno;

/* prototipo de la tarea led   */
void tarea_led( void* taskParmPtr );




TickType_t valorMef( void )
{
	return conectado;
}


void fsmError( void )
{
    fsmEstado = STATE_CONFIG_AT;
}

void fsmInicio7600( void )
{
    fsmEstado = STATE_CONFIG_AT;  // Set initial state
}

#define DEBOUNCE_TIME 40

// FSM Update Sate Function
void fsmConfig7600( )
{
   // static bool_t flagFalling = FALSE;
    static bool_t flagRising = FALSE;

    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsmEstado )
    {
        case STATE_CONFIG_AT:
           uartWriteString( UART_USB, "conf 1: \r\n" ); 
           send_comando_AT("AT+CIPMODE=0\r\n", "OK" , 3000, respuesta ); 
           conectado=0;
           vTaskDelay( 1000 / portTICK_RATE_MS );
           
        if(respuesta == TRUE)                                // llego el dato esperado entonces paso al siguiente estado
         {   
         uartWriteString( UART_USB, "Resultado OK paso de estado" );   
         fsmEstado = STATE_CONFIG_1;	
         respuesta = 0 ;    
         }else                                             // no llega el dato esperado o timeout, vuelvo al estado test
               {
                 respuesta = 0;
               }       
                
        fsmEstado = STATE_CONFIG_1;
        tiempo_diff=5000;
           break;

        case STATE_CONFIG_1:
            uartWriteString( UART_USB, "conf 2: \r\n:" );
            send_comando_AT("AT+CGATT=1\r\n","OK" , 3000, respuesta ); 
            vTaskDelay( 1000 / portTICK_RATE_MS );
            fsmEstado = STATE_CONFIG_2;
            tiempo_diff=5000;
            break;

        case STATE_CONFIG_2: 
        uartWriteString( UART_USB, "conf 3: \r\n" ); 
        send_comando_AT("AT+CGDCONT=1,\"IP\",\"gprs.claro.com.ar\"\r\n","OK" , 3000, respuesta ); 
        fsmEstado = STATE_CONFIG_3;
        tiempo_diff=5000;
        vTaskDelay( 1000 / portTICK_RATE_MS );
            break;
            
        case STATE_CONFIG_3:
            uartWriteString( UART_USB, "conf 4: \r\n" );         
            send_comando_AT("AT+NETOPEN\r\n","OK" , 3000, respuesta ); 
            fsmEstado = STATE_CONFIG_4;
            vTaskDelay( 1000 / portTICK_RATE_MS );
            break;
       
        case STATE_CONFIG_4:
            uartWriteString( UART_USB, "conf 5: \r\n" ); 
            send_comando_AT("AT+AT+CGATT?\r\n","OK" , 3000, respuesta );          
            fsmEstado = STATE_CONFIG_4;           
       
        conectado=1;
       vTaskDelay( 1000 / portTICK_RATE_MS );
            break;

        default:
            fsmError();
            break;
    }
}
