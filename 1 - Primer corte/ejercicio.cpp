#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "omp.h"

using namespace std;

int main() {

    ofstream label1("salida.dat");

    long long int n;

    int i;

    double sum = 0;
    label1.setf(ios::fixed);
    label1.precision(100);

    cout.setf(ios::fixed);
    cout.precision(100);
    int numeroHilos;
    int numeroProcesadores = omp_get_num_procs();

    printf("Ingresar el numero de hilos: ");
    cin >> numeroHilos;
    cout << "Numero de terminos de la serie = ? ";

    cin >> n;
    omp_set_num_threads(numeroHilos);
    printf("Este computador usa %d procesador(es)\n", numeroProcesadores);
    printf("En este ejemplo se desea usar %d hilo(s)\n", omp_get_max_threads());
    printf("En este momento se esta(n) ejecutando %d hilo(s)\n", omp_get_num_threads());


#pragma omp parallel 
    {
        int idHilo = omp_get_thread_num();
        printf("Hola, soy el hilo %d, en este momento se esta(n) ejecutando %d hilo(s)\n", idHilo, omp_get_num_threads());
        
    #pragma omp for reduction(+:sum) 
        for (i = 0; i < n; i++) {

            sum = sum + pow(-1, i) * (4.0 / (2.0 * i + 1));

        }
    }
    printf("Acabo de salir de la seccion paralela\n");

    cout << "El valor PI obtenido es: ";
    cout << sum << "\n";


    return 0;
}