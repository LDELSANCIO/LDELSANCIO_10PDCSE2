/* Copyright 2017-2018, Eric Pernia
 * All rights reserved.
 *
 * This file is part of sAPI Library.
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
 */

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
// sAPI header
#include "sapi.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
SemaphoreHandle_t conectado, datoAEnviar, dato_mutex;
uint8_t conectado_sim =0;
uint8_t dato_sim;  
char *trama_a_enviar;
/*==================[definiciones de datos externos]=========================*/


DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
TickType_t get_diff();

void clear_diff();
void enviar_dato( int dato_sim );
// Prototipo de funcion de la tarea
void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );
void tarea_uart( void* taskParmPtr );
void tarea_enviar_datos( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();
    uint8_t Error_state = 0;
    // UART for debug messages
    uartConfig( UART_232, 115200 );
    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "Modulo SIM7600 4G + GPS" );
    uint8_t conectado=0;
    // Led para dar se�al de vida
    gpioWrite( LED1, ON );

	// Crear tarea en freeRTOS
	BaseType_t res =
	xTaskCreate(
		tarea_led,                     	// Funcion de la tarea a ejecutar
		( const char * )"tarea_led",   	// Nombre de la tarea como String amigable para el usuario
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		&conectado,                	// Parametros de tarea
		tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
		0                           	// Puntero a la tarea creada en el sistema
	);

    res =
    xTaskCreate(
    	tarea_tecla,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_tecla",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        0,                          	// Parametros de tarea
        tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );
    
     res =
    xTaskCreate(
    	tarea_uart,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_uart",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        0,                          	// Parametros de tarea
        tskIDLE_PRIORITY+2,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );

     res =
    xTaskCreate(
    	tarea_enviar_datos,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_enviar_datos",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        trama_a_enviar,                          	// Parametros de tarea
        tskIDLE_PRIORITY+3,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );


    if(res == pdFAIL)
    {
    	//error
    }
    if (NULL == (dato_mutex = xSemaphoreCreateMutex())){
   	   Error_state =1;
   } 
   if (NULL == (conectado = xSemaphoreCreateBinary())){
	   Error_state =1;
   }
    if (NULL == (datoAEnviar = xSemaphoreCreateBinary())){
	   Error_state =1;
   }
      if (0 == Error_state){         
  	  vTaskStartScheduler();
   } else{
	  printf("Error al iniciar el sistema !!!!!!!!!!!!!!");
   }

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/
///////////////////////////////tarea tecla/////////////////////////////////////////
void tarea_tecla( void* taskParmPtr )
{
 char teclaPress; 
 fsmButtonInit();            
 
	while( 1 )
	{
   if( !gpioRead( TEC1 ) )    teclaPress = TEC1;

   fsmButtonUpdate( teclaPress );                 // UTILIZO LA FUNCION ANTIREBOTE
   vTaskDelay( 1 / portTICK_RATE_MS );	
   uint8_t dif = get_diff();  
   
   if (dif > 0)
   {
 
   if (dif == 1)    { dato_sim = 1;     xSemaphoreGive( datoAEnviar);    }      // LIBERO EL SEMAFORO DE ENVIAR DATOS
   clear_diff();                                                                // BORRO EL VALOR DE LA TECLA
   }
   } 
}


////////////////////////////////////////////////tarea USB///////////////////////////////////
void tarea_uart( void* taskParmPtr )
{
   int8_t data;
   fsmInicio7600();   // inicio la mef
  while(1) 
  {   
   if( uartReadByte( UART_USB, &data ) )         // UTILIZO EL PUERTO USB DE DEBUG Y CON LAS TECLAS LLAMO A FUNCIONES
         {
         if( data == 'c' ) {            
           
            debugPrintlnString( "conectando..." );   
            vTaskDelay( 1000/ portTICK_RATE_MS );         
            fsmConfig7600( );                       // LLAMO LA MEF PARA LA CONEXION
               
         }
         if( data == 'g' ) {            
            debugPrintlnString( "gps..." );              
            gps();                                    // LLAMO LA FUNCION PARA LEER EL GPS
            vTaskDelay( 1000 / portTICK_RATE_MS );            
                           }
         }	
     
         vTaskDelay( 100 / portTICK_RATE_MS );
         
}	
}
/////---------------------------tarea enviar datos---------------------------------


void tarea_enviar_datos( void* taskParmPtr )
{
  while(1)
  { 
  if(pdTRUE ==  xSemaphoreTake( datoAEnviar, portMAX_DELAY ))      // SI RECIBO PDTRUE ENVIO DATOS SINO QUEDA BLOQUEADO
  { 
   debugPrintlnString( "Enviando datos:" );          
   xSemaphoreTake(dato_mutex,portMAX_DELAY);          
   dato_sim = trama_a_enviar;  
   xSemaphoreGive(dato_mutex);              
   enviar_dato(dato_sim);       
   }   
   vTaskDelay( 100 / portTICK_RATE_MS );
   
   
}   
}


////////////////////  tarea led  //////////////////////
void tarea_led( void* taskParmPtr )                        // EL PARAMETRO TASKPARMPTR ES "CONECTADO"
{
    // ---------- CONFIGURACIONES ------------------------------

    // ---------- REPETIR POR SIEMPRE --------------------------
while(TRUE)
{
	uint8_t conectado = valorMef();
   //TickType_t dif = *( (TickType_t*)  taskParmPtr );
   if(conectado==0)
   {
      
	gpioWrite( LEDB , 1 );
	vTaskDelay( 500 / portTICK_RATE_MS );           // SIN CONEXION PARPADEO
	gpioWrite( LEDB , 0 );
   vTaskDelay( 800 / portTICK_RATE_MS );	
   }else {
   gpioWrite( LEDB , 0 );                          // CONECTADO ENCIENDO LED VERDE
   gpioWrite( LEDG , 1 );    
 }     
}
}
/*==================[fin del archivo]========================================*/
