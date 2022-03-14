// Ec calor OMP.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <omp.h>
#include <ctime> 

using namespace std;


int** M, Filas, Columnas, n;
float k;
float sX = 2;
float sT = 8;
float Ui;
float Uf;
float Uc;

int tiempoDeseado = 3;
int temperaturaDeseada = 3;

unsigned t0, t1, t2;

#define omp_get_num_threads() 2
#define Filas 10
#define Columnas 5
#define k 7
#define Ui 10
#define Uc 14
#define Uf 18
int main() {

    system("color 7C");


    M = (int**)malloc(Filas * sizeof(int*));
    for (int i = 0; i < Filas; i++) {
        M[i] = (int*)malloc(Columnas * sizeof(int));
    }
    float tamannoenCentimetros = sX * Columnas;
    float lamda = k * sT / pow(sX, 2);
    t0 = omp_get_wtime();
#pragma omp parallel for private (Uc, Ui, Uf) shared (sX, sT) 
    for (int i = 0; i < Filas; i++)
    {
        M[0][0] = Uc;

        for (int j = 0; j < Columnas; j++)
        {
            M[i][j] = Ui;
            if (j == 0)
            {
                M[i][0] = Uc;
            }
            if (j == Columnas - 1) {
                M[i][Columnas - 1] = Uf;
            }
            if (j != 0 & i != 0 & j != Columnas - 1)
            {
                M[i][j] = M[i - 1][j] + lamda * (M[i - 1][j + 1] - 2 * M[i - 1][j] + M[i - 1][j - 1]);
            }
            if (M[i][j] < 0.15)
            {
                M[i][j] = 0.1;
                if (M[i][j] < 0.1) {
                    M[i][j] = 0.05;
                }
                if (M[i][j] < 0.01) {
                    M[i][j] = 0.005;
                }
            }

            cout << M[i][j] << "\t";

        }
        cout << "\n";
        sX = i * sX;
        sT = sT + 1;


    }
    t1 = omp_get_wtime();

    int pos = omp_get_thread_num();

#pragma parallel for
    for (pos = 0; pos < omp_get_num_threads(); pos++) {

        cout << "El resultado deseado puede ser: " << M[tiempoDeseado][pos] << " en el hilo numero: " << pos << "\n";
    }
    t2 = omp_get_wtime();
    cout << "\n\nEl tiempo de ejecucion en el primer bucle es de: " << t1 - t0 << " segundos, mientras que en el 2do es de: " << t2 - t1 << " segundos.";
    return 0;
}