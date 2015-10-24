
#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"


/*
 * A simple generic stack implementation in c
 *
 * The implementation of the stack's methods
 *
 * File:   Stack.c
 * Author: Fredrik Wallgren <fredrik@wallgren.me>
 *
 * Link: https://github.com/walle/cStack
 *
 * @license MIT
 */

#include <stdlib.h>
#include <string.h>

#include "simpletools.h"                      // incluir simpletools para controladores de la placa
#include "abdrive.h"                          // Inckuir abdrive para encoders y servos
#include "ping.h"

#include "Stack.h"

void StackNew(Stack* s, int elemSize)
{
  s->allocatedSize = 16; // Default to a stack that is 16 elements high
  s->size = 0;
  s->elemSize = elemSize;
  s->elements = malloc(s->allocatedSize * s->elemSize); // Allocate the needed memory
}

void StackDestroy(Stack* s)
{
  free(s->elements); // Free the allocated memory
}

void StackPush(Stack* s, void* valueAddress)
{
  if (s->size == s->allocatedSize)
  {
    s->allocatedSize *= 2; // Use a doubling strategy. Every time the stack is too small, double it's size
    s->elements = realloc(s->elements, s->allocatedSize * s->elemSize); // Reallocate the memory to the new size
  }

  void* target = (char*) s->elements + s->size * s->elemSize; // Find the memory segment where to insert the new data
  memcpy(target, valueAddress, s->elemSize); // Copy the data from the valueAddress to target
  s->size += 1;
}

void StackPop(Stack* s, void* returnAddress)
{
  if (s->size > 0)
  {
    s->size -= 1; // This "removes" the element, it will be overwritten the next time push is called
    void* source = (char*) s->elements + s->size * s->elemSize; // Find the memory segment where to fetch the data from
    memcpy(returnAddress, source, s->elemSize); // Copy the data to the returnAddress
  }
  // Else error!
}

void StackPeek(Stack* s, void* returnAddress)
{
  if (s->size > 0)
  {
    void* source = (char*) s->elements + (s->size - 1) * s->elemSize; // Find the memory segment where to fetch the data from
    memcpy(returnAddress, source, s->elemSize); // Copy the data to the returnAddress
  }
  // Else error!
}

double ConvertTick(double tamanio){
  double tick = tamanio/3.25; //medido en mm
  return tick;
} 

void girar(int vuelta){
    if(vuelta == 0){ //vuelta a la derecha
      drive_speed(64,0);
      pause(650); 
      modificarBrujula(0);
    }
    else{ //vuelta a la izquierda
      drive_speed(0,64);
      pause(650); 
      modificarBrujula(1);
    }        
}

void vuelta(){
      drive_speed(62,-62);
      pause(650); 
      modificarBrujula(2);
           
}


int irIzq, irDer; //Variables para el estado de los infrarojos

//Metodo para revisar el estado del sensor infrarojo izquierdo
//Retorna 1 si si hay algo; 0 si no.
/*int leerIzq(){
  int estadoIzq = 0;
  for(int dacVal = 0; dacVal < 200; dacVal += 6)  
    {                                               
      dac_ctr(26, 0, dacVal);                      
      freqout(11, 1, 38000);                      
      irIzq += input(10);                        
    }
  if(irIzq>30){
    estadoIzq = 0;
  }
  
  else if(irIzq<30){
    estadoIzq = 1;
  }
  irIzq = 0;
  return estadoIzq;
}

int leerDer(){
  int estadoDer = 0;
  for(int dacVal = 0; dacVal < 200; dacVal += 6) 
    { 
      dac_ctr(27, 1, dacVal);                      
      freqout(1, 1, 38000);
      irDer += input(2);                       
    }
  if(irDer>30){
    estadoDer = 0;
    print("No hay pared derecha \n");
  }
  
  else if(irDer<30){
    estadoDer = 1;
    print("Hay pared derecha \n");
  }
  irDer = 0;
  return estadoDer;
          
}
*/

int brujula [4] = {0,-90,180,90}; //Nos dice los grados de giro para que se adecue el mapa cardinal
int brujTemp = 0;
//N-O-S-E

void modificarBrujula(int giros){ //0 es derecha; 1 izquierda; 2 si es vuelta completa
    if (giros==0){
      brujTemp = brujula[3];
      brujula[3] = brujula[2];
      brujula[2] = brujula[1];
      brujula[1] = brujula[0];
      brujula[0] = brujTemp; 
    }
    
    if (giros==1){
      brujTemp = brujula[0];
      brujula[0] = brujula[1];
      brujula[1] = brujula[2];
      brujula[2] = brujula[3];
      brujula[3] = brujTemp; 
    }
    
    if (giros==2){
      brujTemp = brujula[2];
      brujula[2] = brujula[0];
      brujula[0] = brujTemp;
      
      brujTemp = brujula[3];
      brujula[3] = brujula[1];
      brujula[1] = brujTemp; 
    }
    
}

/*
int leer(int lado, int veces){ //metodo para leer con el ultrasonico para los 3 lados.
  int retornando = 0;
  
  girar(lado);
  
  if(ping_cm(8) < 7){
    retornando = 1;
  }
  
  return retornando;
} */

int leer(){ //metodo para leer con el ultrasonico para el frente. Retorna 0 si no hay nada; 1 si si
  int retornando = 0;
  
  if(ping_cm(8) < 3
  ){
    retornando = 1;
  }
  
  return retornando;
}

int leerIzq(){ //metodo para leer con el ultrasonico para el frente. Retorna 0 si no hay nada; 1 si si
  int retornando = 0;
  
  if(ping_cm(9) < 10){
    retornando = 1;
  }
  
  return retornando;
}

int leerDer(){ //metodo para leer con el ultrasonico para el frente. Retorna 0 si no hay nada; 1 si si
  int retornando = 0;
  
  if(ping_cm(7) < 10){
    retornando = 1;
  }
  
  return retornando;
}

int distLeft[2], distRight[2];


int main(int argc, char** argv)
{
  drive_setRampStep(12);
  int inf = 1; //while
  int algoDer = 1;
  int algoIzq = 1;
  int algoEnf = 0;
  
  int avanzar = 5; //5 = true; 6 = false
  int girando = 0;
  
  while(inf = 1){
    //double tamanio = 125.0; //Depende del tamanio de cada "celda"
  
    int avance = 0;
    
    int giroder = 0;
    int giroizq = 0;
  
    // Stack de la cantidad de pasos que se han dado
    Stack pasos;
    StackNew(&pasos, sizeof(int));
    
    // Stack de la cantidad de giros para ver a donde regresar
    Stack giros;
    StackNew(&giros, sizeof(int));
    
    // Stack que indica hacia donde se puede girar. Guarda binarios
    //Izquierda-Derecha-Adelante
    Stack caminos;
    StackNew(&caminos, sizeof(int));
    
    //Aqui comienza

    drive_getTicks(&distLeft[0], &distRight[0]); //distancia inicial
 

    algoEnf = leer();
    if (algoEnf == 0){
      avanzar = 5; 
      girando = 0;
      StackPush(&giros, &girando); 
    }
    else {
      avanzar = 6;
    }    
    
    algoDer = leerDer();
    algoIzq = leerIzq();
    int izquierda; 
    int derecha;
    
    if((algoDer == 0) || (algoIzq == 0)){ //Si si hay paso en algun lado
      drive_getTicks(&distLeft[1], &distRight[1]); //Se manejo esta distancia
      StackPush(&pasos, &distLeft[1]);
      StackPush(&pasos, &distRight[1]);
      distLeft[0] = 0;
      distRight[0] = 0;
      distLeft[1] = 0;
      distRight[1] = 0;
      
      
      //Para estas variables true va a ser 7; false es 8

      if (algoIzq == 0){
        izquierda = 7;  
      }
      else if (algoIzq == 1){
        izquierda = 8;
      }
      
      StackPush(&caminos, &izquierda);
      
      if (algoDer == 0){
        derecha = 7;  
      }
      else if (algoDer == 1){
        derecha = 8;
      }
      
      StackPush(&caminos, &derecha);
  
    }
    
    else {
      izquierda = 8;
      derecha = 8;
      StackPush(&caminos, &izquierda);
      StackPush(&caminos, &derecha);
    }
    
    StackPush(&caminos, &avanzar);
    
    StackPop(&caminos, &avanzar);
    
    
    
      //adelante
    if (avanzar == 5){
      avanzar = 6;
      StackPush(&caminos, &avanzar);
      while ((algoDer == 1) && (algoIzq == 1) && (algoEnf == 0)){ //"avanzar"
        drive_speed(90,90);
        pause(100);
        algoDer = leerDer();
        algoIzq = leerIzq();
        algoEnf = leer();
      }
    }
    
    else if (avanzar==6){
      StackPop(&caminos, &derecha);
      if (derecha == 7){
        girar(0);
        girando = 90;
        StackPush(&giros, &girando);
        avanzar = 
      }    
      else if (derecha == 8){
        StackPop(&caminos, &izquieda);
        girar(1);
        girando = -90;   
      }
    }      
    
    
    
    
    
        
    
    
    
    drive_rampStep(0,0);
    //Si se sale del while, llegamos a un nodo oooo un tope
    drive_getTicks(&distLeft[1], &distRight[1]); //Se manejo esta distancia
//    vuelta();
    StackPush(&pasos, &distLeft[1]);
    StackPush(&pasos, &distRight[1]);
    
    
    
    
    int prueba1;
    StackPop(&pasos, &prueba1);
    int prueba2;
    StackPop(&pasos, &prueba2);
    drive_goto(prueba1, prueba2);
    drive_rampStep(0,0);
   }
  
  /*
  // Counter for the for loops, must compile in C99 mode to allow initial declarations in for loops
  int i;

  // Add values 0 - 99
  for(i=0; i < 100; i += 1)
  {
    StackPush(&s, &i);
  }

  // Pop the value 99
  StackPop(&s, &result);
  printf("top = %i\n", result);

  // Add values 200 - 299
  int i1;
  for(i1 = 200; i < 300; i += 1)
  {
    StackPush(&s, &i);
  }

  // Pop the values 299 - 200
  int i2;
  for(i2 = 0; i < 100; i += 1)
  {
    StackPop(&s, &result);
    printf("top = %i\n", result);
  }

  // Peek the value 98 three times
  StackPeek(&s, &result);
  printf("top = %i\n", result);

  StackPeek(&s, &result);
  printf("top = %i\n", result);

  StackPeek(&s, &result);
  printf("top = %i\n", result);

  // Pop all the remaining values
  while(s.size > 0)
  {
    StackPop(&s, &result);
    printf("top = %i\n", result);
  }

  // Free the memory allocated
  StackDestroy(&s);

  */
  return 0;
}


