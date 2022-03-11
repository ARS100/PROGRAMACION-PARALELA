// PARCIAL2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <omp.h>
#include <ctime> 


using namespace std;

int main()
{
    unsigned t0, t1;

    float a, b, k, tot, res, s;
    int n, h;

    cout << "Ingrese los limites (inferior y superior, respectivamente) :" << endl;
    cin >> a;
    cin >> b;

    cout << "Ahora indique el valor que indique la mayor cantidad en la sumatoria: " << endl;
    cin >> n;

    res = 0;
    float delta = (b - a) / n;


    int numeroHilos;
    cout << "Ingrese ahora, el numero de hilos con los que desea trabajar: ";
    cin >> numeroHilos;

    omp_set_num_threads(numeroHilos);
    int numeroProcesadores = omp_get_num_procs();

    cout << "El computador está usando " << numeroProcesadores << " procesadores ahora mismo." << endl;

    
    cout << endl << "Entramos a la estancia paralela" << endl;

    // Este pragma generalmente se usa en los ciclos for. -EL for que se encuentra posteriormente divide cada ciclo o iteración
    // dentro de los hilos implicados. Generalmente hacer omp parallel for ocasiona menor tiempo de compilación, y ejecuta al tiempo
    // tanto el comando para subdividir procesos como el de implicación de los hilos por los bucles.
    t0 = clock();
    #pragma omp parallel for
        for (int i = 1; i <= n; i++) {
            k = i - 0.5;
            s = sin(a + k * delta);
            tot = s * delta;
            res = res + tot;
            cout << "n en este instante es :" << i << endl;
        }
        cout << "En este momento definitivo, se implican dentro del trabajo esta cantidad de hilos (al hacerse el JOIN): " << omp_get_num_threads() << endl;
        cout << "El resultado de la integral es: " << res << endl;
    t1 = clock();

    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "El tiempo de ejecución en la zona paralizada fue: " << time << " segundos" << endl;
    
}
