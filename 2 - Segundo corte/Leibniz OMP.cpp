// LEIBNIZ - OPENMP.cpp : 2182059 Alejandro Romero Serrano

#include <iostream>
#include <omp.h>
#include <cmath>
using namespace std;

// La generalización global del método numérico de Leibniz
// para posteriormente aplicarse en el main.

double Leibniz(int cantidadIteraciones) {
    double X = 4;
    double Y = 1;
    double Z = 1;

    double val = 0;
    for (int i = 0; i < cantidadIteraciones; i++) {
        val = val + Z * (X / Y);
        Z = Z * (-1);
        Y += 2;
    }
    return val;
}
int main()
{
    cout << "El objetivo del programa es la realizacion simultanea de ecuaciones de Leibniz con variacion en las iteraciones de ejecución, por favor, ingrese la cantidad de iteraciones base con la que desea trabajar: ";
    double* PI;
    int cProcesadores = omp_get_num_procs();
    PI = (double*)malloc(cProcesadores * sizeof(double));
    int cantidadIteraciones;
    cin >> cantidadIteraciones;

    // Una vez definidos la cantidad de procesadores, y asignadas la cantidad de 
    // iteraciones para aplicarse al método numérico, se empieza la paralelización:

#pragma parallel for
    for (int i = 0; i < cProcesadores; i++) {
        if (cantidadIteraciones == 0) {
            PI[i] = Leibniz(0);
        }
        PI[i] = Leibniz(pow(cantidadIteraciones,i));
    }
    
    // El resultado saliente es un puntero con los PI's resultantes de cada uno de los
    // hilos ejecutados, ahora con ellos lo unico que haremos sera imprimirlo exhibiendo
    // el hilo correspondiente.

    for (int i = 0; i < cProcesadores; i++) {
        
        cout << "--------------------------------------- \n";
        cout << "El PI obtenido de hacer " << cantidadIteraciones << " iteraciones de leibniz elevado a "<< i << " (id del procesador en ejecucion), es decir " << pow(cantidadIteraciones, i) << " iteraciones es " << PI[i] << "\n\n";
        cout << "--------------------------------------- \n";
    }
    return 0;
}