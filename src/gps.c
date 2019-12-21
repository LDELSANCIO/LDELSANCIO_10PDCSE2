/*=====[modulo UART]=====================================================
 *Copyright 2019 Del Sancio Leonardo <gumyds@mail.com>
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/10/9
 * Version: 1
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "gps.h"
#include "string.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/


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


char gps (char Trama[])
{
   char linea[72] = "";  
   char Latitud[72] = "";
   char Longitud[72] = "";
   char GPS_Hora[72] = "";
   char GPS_Fecha[72] = "";
   char GPS_Ubicacion1;     
   char GPS_Ubicacion2;     
   long intento=0;  
   int x;
   char character;
   uint8_t gpsLeido=0;
   uint8_t j=0;
   uint8_t k =0;
   uint8_t m=0;
   uint8_t n=0;
   uint8_t conta=0;       

   uint8_t cont,bien;
   uint8_t comienzo=0;
   uint8_t coma=0;
   uint8_t i=0;
   uint8_t p=0;
   uint8_t flag=0;      
   
   uartWriteString( UART_232, "AT+CGPS=1\r\n" );    // activo gps
   delay(1000);
   uartWriteString( UART_232, "AT+CGPSINFO=1\r\n" );   // envio comando para que cada 1 segundo me mande la trama
   contador=0;
   uartWriteString( UART_USB, "Bajando datos...\r\n" );      
   delay(3000);
   while( uartReadByte( UART_232, &dato ) )  {} // limpio el buffer
     
  while (intento<6000 && gpsLeido==0)              
         {
                
     while( uartReadByte( UART_232, &dato ) )         // pregunto si hay datos en la uart y la guardo en &dato
      {
      intento=0; 
      linea[p]= dato;                              // voy armando el paquete en linea con el indice p
      p++;                                         // incremento p
     // uartWriteByte( UART_USB, dato );             // voy imprimiendo la trama en el monitor serial
      
      if(dato == 13 && p>70)      // VERIFICO QUE EL TAMAÑO DEL GPS SEA 70 SINO NO LO TOMO... 
      {
       uartWriteString( UART_USB, "\r\nTrama original:\r\n" );            
       uartWriteString( UART_USB, linea );  
       dato=1;    
       p=0;
       flag=0;
        //   LA TRAMA GPS DEL SIM7600 ES DISTINTAS A LAS DE LOS GPS CONVENCIONALES, TUVE QUE IMPLEMENTARLA YO
         
        //+CGPSINFO: 2650.89602,S,06512.044857,W,271119,160019.0,420.2,0.0,183.1       
        //+CGPSINFO: [<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC time>],[<alt>],[<speed>],[<course>]          
        
                  j=0;
                  for (i=13;i<=23;i++)  { Latitud[j] = linea[i]; j++;  }      // saco latitud
                  j=0;
                  for (i=27;i<=38;i++)  { Longitud[j] = linea[i]; j++; }       // longitud        
                  j=0;
                  for (i=42;i<=47;i++)  { GPS_Fecha[j] = linea[i]; j++; }      // fecha
                  j=0;
                  for (i=49;i<=54;i++)  { GPS_Hora[j] = linea[i]; j++; }     //hora
                  
                  for (i=0;i<72;i++) 
                  {
         //         if (linea[i]==':') {  i++; LatD=linea[i]; i++; LatU=linea[i];      }   
                  if (linea[i]==',') {  coma++; i++; }                                                      
                  if(coma==1) { GPS_Ubicacion1=linea[i];  }                  // saco ubicacion Norte Sur
       //           if(coma==2) {  i++; LonD=linea[i]; i++; LonU=linea[i];  }                  // long
                  if(coma==3) { GPS_Ubicacion2=linea[i];  }                   // este oeste
                  }
                  
                  uartWriteString( UART_USB, "\n Latitud:" );      
                  uartWriteString( UART_USB, Latitud );
                  uartWriteString( UART_USB, "\n Longitud:" );      
                  uartWriteString( UART_USB, Longitud );
                  uartWriteString( UART_USB, "\n ubicacion:" );      
                  uartWriteByte( UART_USB, GPS_Ubicacion1 );
                  uartWriteByte( UART_USB, GPS_Ubicacion2 );
                  uartWriteString( UART_USB, "\n fecha:" );      
                  uartWriteString( UART_USB, GPS_Fecha );           
                  uartWriteString( UART_USB, "\n Hora:" );     
                  uartWriteString( UART_USB, GPS_Hora );   
                  uartWriteString( UART_USB, "\n ---------------------------------------------------------------\r\n" );  
  
     ////---------   EN ESTA SECCION VOY ARMANDO LA TRAMA FINAL QUE ES UN STRING, PERO ME FALTAN DATOS AUN, 
      ////          DE A POCO VOY ARMANDO MI PROGRAMA PARA EL PROYECTO FINAL, EN ESTE TP SOLO ENVIO DATOS FIJOS AL SERVER
                  
     /*             int latEntero1 = (int) Latitud[2];
                  int latEntero2 = (int) Latitud[3];
                  int latEntero3 = (int) Latitud[5];
                  int latEntero4 = (int) Latitud[6];
                  int latEntero5 = (int) Latitud[7];
                  int latEntero6 = (int) Latitud[8];
                  
                  long LatitudFinal = latEntero1*100000 + latEntero2*10000+ latEntero3*1000+latEntero4*100+latEntero5*10+latEntero6;
                  LatitudFinal = LatitudFinal/60;
                  uartWriteString( UART_USB, "\n Latitud final:" );     
                  uartWriteByte( UART_USB, LatitudFinal );  
                  
                  Trama[0]='-';    
                  Trama[1]= Latitud[0];
                  Trama[2]= Latitud[1];
                  Trama[3]='.';
                  Trama[4]= LatitudFinal/1000000;
         
*/
/*
                  for(i=0;i<50;i++)
                  { 
                  
                  if(i<10) Trama[i]=Latitud[i]; 
                  if(i==10) Trama[i]=';';   
                  if(i>=11 && i<24) Trama[i]=Longitud[i-11];  
                  if(i==24) Trama[i]=';';      
                  if(i>=25 && i<31) Trama[i]=GPS_Fecha[i-25];
                  if(i==31) Trama[i]=';';         
                  if(i>=32 && i<38) Trama[i]=GPS_Hora[i-32];   
                  }   
                                 
                  uartWriteString( UART_USB, Trama );                       
                  
  */             
               ////////////////////////////////////////////////////////////////   
                  j=0;
                  k =0;
                  m=0;
                  n=0;
                  conta=0;  
                  comienzo=0;    
                  coma=0;                  
                  
                  ///////////////////////////////////reinicio todo el array
                  for(i=0;i<72;i++)
                  {
                  linea[i]=' ';                  
                  }   
                  for(i=0;i<72;i++)
                  {   
                  Latitud[i]=' ';
                  Longitud[i]=' ';
                  GPS_Hora[i]=' ';
                  GPS_Fecha[i]=' ';   
                  }  
                  delay(500);
                  uartWriteString( UART_232, "AT+CGPS=0\r\n" );
                  gpsLeido=1;  
              
      }
   }
   intento++;
   delay(1);
         }
      if(gpsLeido == 0)  uartWriteString( UART_USB, "\nTIMEOUT ERROR\r\n" );   
      else { uartWriteString( UART_USB, "\n-DATOS GUARDADOS-\r\n" ); 
            //return Trama[72];
             }
 }
 
 


