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
    drive_rampStep(115,115);                //instruccion que permite que elrobot se mantenga a una velocidad constante

      while(ping_cm(8) < 33){               //advertencia del sensor,un objeto a menos de 35cm
        while(ping_cm(8)<20){                //advertencia si el objeto esta a menos de 20cm
          drive_speed(-26,25);              //gira hasta que el espacio este libre
          pause(150);
        }          
        int urn = rand() % 3;                          // Random
        drive_speed(-60,-60);                         //retrcede por unn momento
        pause(250);
        if(urn == 1) {                              // gira a la derecha
          drive_speed(26, -25);
          pause(450);  
        }                            
        else if(urn == 2) {                      //gira a la izquierda
          drive_speed(-25,26);
          pause(450);
        }          
        else{                                 // dara una vuelta de aprox 180 grados, para mayor precicion se recomienda el uso del drive_goto()
          drive_speed(52,-52);
          pause(450);
        }          
      }
   }  
} 
