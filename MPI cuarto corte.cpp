// MPI cuarto corte.cpp : 2182059 - Alejandro Romero Serrano y 2162112 - Sergio Roa.

#include <mpi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


using namespace std;

int main(int argc, char** argv) {
    
    // declaración de variables:

    int Filas, Columnas, n, tiempoDeseado, temperaturaDeseada;
    float k, sT, sX, Ui, Uf, Uc;
    double** M;
    double** m;
    double t1, t2;

    
    // Se inicializa la ejecución de MPI, declarando el comunicador global, 
    // es decir, por aquel que haremos generalmente el trabajo y aplicando una
    // variable de estado que será usada en las funciones de envío y recepción.
    

    MPI_Status status;
    MPI_Comm comunicadorPrincipal;

    
    MPI_Init(&argc, &argv);
    t1 = MPI_Wtime();
    system("color 7C");


    // Declaración para visualizar el número de procesos globales:
    int world_size;
    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // Para mostrar el rango del proceso:
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    // Menú de bienvenida al programa.
    cout << "\t\t|PROGRAMA PARA GRAFICAR LA TEMPERATURA DE UNA BARRA| \n";
    cout << "Bienvenido\n";
    cout << "PRESENTADO POR: 2182059 - Alejandro Romero y 2162112 - Sergio Roa.\n";
    cout << "\nPara comenzar, ingrese la cantidad de filas que desea que tenga la barra: ";
    cin >> Filas;
    cout << "\nIngrese la cantidad de columnas que tendr\xA0 la barra: ";
    cin >> Columnas;
    cout << "\nAhora, ingrese la constante t\x82rmica deseada (en grados): ";
    cin >> k;
    cout << "\nTambien, asigne las temperaturas: Inicial (comienzo de la barra), de la esquina izquierda" <<
        "y de la esquina derecha, \n respectivamente dando ENTER : " << "\n";
    cin >> Ui;
    cin >> Uc;
    cin >> Uf;
    cout << "\nAhora, en qu\x82 centimetros de salto desea de X, y posteriormente, el salto temporal:\n";
    cin >> sX;
    cin >> sT;


    // Se hace para obtener el nombre del procesador: 
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("LA MATRIZ GENERADA ES LA SIGUIENTE:\n");

    // Asignación de dirección en una matriz y su
    // posterior impresión:

    M = (double**)malloc(Filas * sizeof(double));
    m = (double**)malloc(Filas * sizeof(double));

    for (int i = 0; i < Filas; i++) {

        M[i] = (double*)malloc(Columnas * sizeof(double));
        m[i] = (double*)malloc(Columnas * sizeof(double));
    }
    int total = Filas * Columnas;

    double tamannoenCentimetros = sX * Columnas;
    double lamda = k * sT / pow(sX, 2);
    // Empieza la impresión de la matriz, sin embargo,
    // solamente se hace sobre identificadores de filas
    // y columnas, para posteriormente ASIGNAR y mostrar
    // los datos reales de las barras.

    cout << "\n";
    for (int i = 0; i < Columnas; i++) {
        cout << "T" << i << "\t";
    }
    cout << "\n\n";

    // Acá empieza la asignación real de los valores
    // con la ecuación de calor.


    for (int i = 0; i < Filas; i++)
    {
        M[0][0] = Uc;
        cout << "L= " << i << "\t";
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
            if (j != 0 && i != 0 && j != Columnas - 1)
            {
                // Como en el resto de condicionales se agregan valores de los
                // extremos de la barra, se hace el calculo para las variaciones
                // centrales con el siguiente despeje a la incognita parcial.

                M[i][j] = M[i - 1][j] + lamda * (M[i - 1][j + 1] - 2 * M[i - 1][j] + M[i - 1][j - 1]);
            }
            // Estas aproximaciones, se hacen para evitar que los valores 
            // sean lo suficientemente grandes para cubrir a sus allegados.
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
            // Se envían los datos trabajados desde M al proceso numero 2 (proceso id=1)
            // para que existan diferentes procesos manejados al tiempo. Con M las asignaciones y con m
            // las impresiones finales.

            MPI_Send(&M, total, MPI_DOUBLE, world_rank + 1, 0, MPI_COMM_WORLD);

            MPI_Recv(&m, total, MPI_DOUBLE, world_rank, 0, MPI_COMM_WORLD, &status);

        }
        cout << "\n";
        // El salto temporal y espacial en el algoritmo se denota así.
        sX = i * sX;
        sT = sT + 1;
    }
    cout << "\nElija la fila deseada de la cual requiere revisar las temperaturas de la varilla (en segundos):\n ";
    cin >> tiempoDeseado;
    for (int i = 0; i <= Columnas - 1; i++)
    {

        cout << m[tiempoDeseado][i] << "\t";
    }
    // EL barrier se hace para esperar que ambos procesos ya hayan sido ejecutados
    // y asegurarnos de que el programa está relativamente algoritmizado para
    // empezar a hacer las impresiones correspondientes y las evaluaciones manuales.

    MPI_Barrier(MPI_COMM_WORLD);
    float tiempoTotal = Filas * sT;

    // Se realizan ciertas ejecuciones de salida, de resolución a problemas matematicos planteados.

    cout << "\nEl tiempo total es:\n ";
    cout << tiempoTotal;
    cout << "\nAhora, ingrese la temperatura del nodo que desea mirar: \n";
    cin >> temperaturaDeseada;
    cout << "La temperatura deseada es: " << "\n";
    cout << m[tiempoDeseado][temperaturaDeseada];
    cout << "\n\n" << "Trabajo ejecutado en la consola: ";
    // Se obtiene el tiempo final de ejecución y se calcula el total empleado.
    t2 = MPI_Wtime();
    cout << "El tiempo de ejecución total fue de aproximadamente: " << t2 - t1 << " segundos";
    // Finalizamos la ejecución MPI.
    MPI_Finalize();
}