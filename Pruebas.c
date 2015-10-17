/*
  Programa para udo del sensor
*/

#include "simpletools.h"                      // incluir simpletools para controladores de la placa
#include "abdrive.h"                          // Inckuir abdrive para encoders y servos
#include "ping.h"                             // Incluir ping para sensor

int turn = 1;                                     // variable de inicio

int main()                                    // main 
{
  while(turn == 1){                         //lloppara que siempre se mantenga en movimiento
    drive_speed(90,90);                //instruccion que permite que elrobot se mantenga a una velocidad constante

    if(ping_cm(8) < 15){
      drive_speed(68,0);
      pause(650);
    }      
   
   }  
} 
