// 2182059 y 2162112 (Parcial 1)

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;


int **M, Filas, Columnas, n;
float k;
float sT;
float sX;
float Ui;
float Uf;
float Uc;
int main()
{
	system("color 7C");
	cout << "\t\t|PROGRAMA PARA GRAFICAR LA TEMPERATURA DE UNA BARRA| \n";
	cout << "Bienvenido\n";
	cout << "PRESENTADO POR: 2182059 - Alejandro Romero y 2162112 - Sergio Roa.\n";
    cout << "\nPara comenzar, ingrese la cantidad de filas que desea que tenga la barra: ";
    cin >> Filas;
    cout << "\nIngrese la cantidad de columnas que tendr\xA0 la barra: ";
    cin >> Columnas;
    cout << "\nAhora, ingrese la constante t\x82rmica deseada (en grados): ";
    cin >> k;
    cout << "\nTambien, asigne las temperaturas: Inicial (comienzo de la barra), de la esquina izquierda y de la esquina derecha,\n respectivamente dando ENTER: " << "\n";
    cin >> Ui;
    cin >> Uc;
    cin >> Uf;
    vector<int>U(Columnas);
    cout << "\nAhora, en qu\x82 centimetros de salto desea de X, y posteriormente, el salto temporal:\n";
    cin >> sX;
    cin >> sT;
    printf("LA MATRIZ GENERADA ES LA SIGUIENTE:\n");
    M = (int**)malloc(Filas*sizeof(int*)); 
    for (int i=0;i<Filas;i++) {
		M[i] = (int*)malloc(Columnas*sizeof(int)); 
    }
    float tamannoenCentimetros = sX*Columnas;
    float lamda = k*sT /pow(sX,2);
    int tiempoDeseado;
    int temperaturaDeseada;
    
    cout << "\n";
    for(int i = 0; i<Columnas; i++){
        cout << "T" << i << "\t";
    }
    cout << "\n\n";
    
    for(int i= 0; i<Filas; i++)
    {
        M[0][0] = Uc;
        cout << "L= " << i << "\t";
        for(int j = 0; j<Columnas; j++)
        {
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
            cout << M[i][j] << "\t";
            
        }
        cout <<"\n";
        sX = i*sX;
        sT = sT+1;
    }
    cout << "\nElija la fila deseada de la cual requiere revisar las temperaturas de la varilla (en segundos):\n ";
    cin >> tiempoDeseado;    
    for(int i = 0; i<=Columnas-1; i++)
    {
        cout <<  M[tiempoDeseado][i] << "\t";
    }
    
    float tiempoTotal = Filas*sT;
    cout << "\nEl tiempo total es:\n ";
    cout << tiempoTotal;
    cout << "\nAhora, ingrese la temperatura del nodo que desea mirar: \n";
    cin >> temperaturaDeseada;
    cout << "La temperatura deseada es: " << "\n";
    cout << M[tiempoDeseado][temperaturaDeseada];
}