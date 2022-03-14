#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


#include "math.h"

#define N 3   // Se definen 3 hilos de trabajo únicamente, aunque
              // también se puede considerar al numero de semillas como
              // la cantidad de hilos. 

using namespace std;
using std::ofstream;
using std::endl; using std::string;
using std::fstream;

// Acá quedan las estructuras y generación de semillas
// apoyadas mediante el código del profesor.

typedef struct{
    float x; 
    float y;
    float t; 
}posParticle;



__device__ void generarSemillas(posParticle *semillas, const  int maxParticles) 
{

    unsigned int num_semillas = maxParticles;
    for (int i = 0; i < num_semillas; ++i)
    {
        const float radio = 0.1;
        const float alfa  = 2.0f * M_PI * (float)i / (float)num_semillas;
        semillas[i].t = 0.0;
        semillas[i].x = 0.5f + radio*cos(alfa);
        semillas[i].y = 0.5f + radio*sin(alfa);
    }
    
}

__global__ void iniciarYterminarIntegral(double t, posParticle *se, float *dev_x, float *dev_y, float *dev_t){
    generarSemillas(se,N);

    // se asignan el valor inicial y final del tiempo a
    // el puntero referenciado de arriba semilla, conectado a
    // esta función como 'se'.

    se[0].t = 0;
    se[N].t = 1;

    // Se declararán nuevos punteros para asignar v1
    // correspondientes a la posición posterior a la integral
    // ejecutada sobre cada valor.

    double *X = new double[(int)N];
    double *Y = new double[(int)N];
    
    // Este es el ID que será recorrido
    // para cada uno de los valores de asignación
    // para los vectores posición inicial.
    int h;

    
    double *coordenadasX = new double[(int)N];
    double *coordenadasY = new double[(int)N];

    dev_x[0] = se[0].x;
    dev_y[0] = se[0].y;
    dev_t[0] = se[0].t;
    

    int columna = threadIdx.x; 
    //int fila = threadIdx.y; 

    //int pos = columna + N*fila; 

    
    h = (t)/(N);
    se[columna].t = t;
    if(columna < N){
      double v1 = sin(se[columna].t*M_PI/180);
        // se[columna].x = v1;
      X[columna] = v1;
      double v2 = cos(se[columna].t*M_PI/180);
        // se[columna].y = v2;
      Y[columna] = v2;
    }
    
    // inmediatamente después a cada valor comprendido
    // entre el 0 y 1, le asignamos su integral y se 
    // identificará como la nueva coordenada.


    double booleX = ((7 * X[columna] + 32 * X[columna+h] +
                12 * X[columna+2*h] +32 * X[columna+3*h]
                + 7 * X[columna+4*h] *
                 2 * h / 45));
    dev_x[columna] = dev_x[columna] + booleX;

    // La regla de integración aplicada es la regla de Boole.

    double booleY = ((7 * Y[columna] + 32 * Y[columna+h] +
                12 * Y[columna+2*h] +32 * Y[columna+3*h]
                + 7 * Y[columna+4*h] *
                 2 * h / 45));
    dev_y[columna]= dev_y[columna] + booleY;

    
   
}


// Sigue el main, donde visualizaremos cada función paralelizada
// desde las declaraciones y el direccionamiento entre host y device
// como la llamada al kernel y las asignaciones correspondientes para
// posteriormente imprimir.

int main(){
    
    // Se DECLARAN los espacios en memoria CPU y GPU.

    int tiempo;
    cout << "Ingrese el tiempo correspondiente a utilizarse (es decir, la cantidad de intervalos): ";
    cin >> tiempo;
    float *hst_x, *hst_y, *hst_t; 
    float *dev_x, *dev_y, *dev_t; 

    // Se hace la RESERVA en el host.
    hst_x = (float*)malloc(N*sizeof(float));
    hst_y = (float*)malloc(N*sizeof(float));
    hst_t = (float*)malloc(N*sizeof(float));
    
    // Se hace la RESERVA en el device.
    cudaMalloc( (void**)&dev_x, N*sizeof(float));
    cudaMalloc( (void**)&dev_y, N*sizeof(float));
    cudaMalloc( (void**)&dev_t, N*sizeof(float));

    // Se inicializan los espacios asignados anteriormente.
    for(int i=0; i<tiempo; i++) 
    { 
        dev_t[i] =  (float)i; 
    }
    // Copiamos los datos y los obtenemos tanto en CPU como en GPU.
    cudaMemcpyToSymbol(dev_x, hst_x, N*sizeof(float));
    cudaMemcpyToSymbol(dev_y, hst_y, N*sizeof(float));
    cudaMemcpyToSymbol(dev_t, hst_t, N*sizeof(float));

    // lanzamiento de kernel
    iniciarYterminarIntegral<<<1,N>>>(1, 0, dev_x, dev_y, dev_t); 
    for(int i=0; i<tiempo; i++) 
    { 
        hst_t[i] =  (float)i; 
    }
    // copiar los datos recogidos.
    cudaMemcpy( hst_x, dev_x, N*sizeof(float), cudaMemcpyDeviceToHost );
    cudaMemcpy( hst_y, dev_y, N*sizeof(float), cudaMemcpyDeviceToHost );
    
    printf("Resultados:\n");
    printf("Intervalos de tiempo: ");
    cout << tiempo;

    string archivo = "prueba.txt";
    ofstream salida(archivo);

    //salida.open("/content/drive/MyDrive/prueba.txt");
    
    for(int i=0; i<tiempo; i++) 
    { 
      for(int j=0; j<N;j++) 
      {
           if(i==0){
           }
        
       cout << " \n En el tiempo # " << i << " se tiene en x, y, t respectivamente: "; 
       cout << hst_x[j] << "   ";
       cout << hst_y[j] << "   ";
       cout << hst_t[j] << "   ";
       cout << "\n";

        salida << " \n En el tiempo #  se tiene en x, y, t respectivamente: ";
        salida << hst_x[j] << "   ";
        salida << hst_y[j] << "   ";
        salida << hst_t[j] << "   ";
        salida << "\n";     
     
      } 
    } 
    salida.close();
    system("pause");
    return 0;
}
