// 2182059 - Alejandro Romero Serrano

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define N 1


//Función global de la generación de la matriz de la EC de calor para ser llamada en el MAIN como Kernel.

__global__ void sacarMatrizEcDeCalor(int **M, float k, float sT, float sX, float Ui, float Uf,float Uc, int Filas, int Columnas, int n)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    M = (int**)malloc(Filas*sizeof(int*));
    M[j] = (int*)malloc(Columnas*sizeof(int)); 
    
    float lamda = k*sT /pow(sX,2);
    // Acá se deben obtener los valores correspondientes:
    M[i][j] = Ui;
      if(j==0)
        {
         M[i][0]=Uc;
        }
        if(j==Columnas-1){
        M[i][Columnas-1]=Uf;
        }
        if(j!=0 & i!=0 & j!=Columnas-1)
        {
          M[i][j] = M[i-1][j] + lamda*(M[i-1][j+1]-2*M[i-1][j]+M[i-1][j-1]);
        }
        if(M[i][j]< 0.15)
        {
          M[i][j] = 0.1;
               if(M[i][j]< 0.1){
                   M[i][j] = 0.05;
               }
               if(M[i][j]< 0.01){
                   M[i][j] = 0.005;
               }
        }
    
}

int main(int argc, char *argv[]) {
    

    // Se inicializan los punteros para asignar valores en D,
    // con un tamaño de N. Estos se inicializan al igual que los de H.

    int **Valores_Matriz, **Valores_H;

    
    int Filas = 15;
    int Columnas = 20;
    
    cudaMalloc((void**)&Valores_Matriz,Filas*sizeof(int));
    Valores_H = (int**)malloc(Columnas*sizeof(int));


    // Se llama hace el llamado al Kernel, con un hilo por cada
    // bloque de ejecución correspondiente a la cantidad de iteraciones.

    sacarMatrizEcDeCalor <<< N, 2 >>>(Valores_Matriz, 10, 1, 2, 15, 20, 17, Filas, Columnas, 2);

    // Se copia en el host los valores generados desde GPU.
    cudaMemcpy(Valores_H,Valores_Matriz,N*sizeof(int),cudaMemcpyDeviceToHost);

    // Este es el primer valor tras el comienzo de las iteraciones
    for(int i = 0; i< Filas; i++)
    {
        for(int j = 0; j< Columnas; j++){
          printf("%f\t", Valores_H[i][j]);
        }
        printf("%f\n");
    }
  




    return 1;
}
