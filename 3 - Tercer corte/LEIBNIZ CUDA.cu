// 2182059 - Alejandro Romero Serrano

//Librerias utilizadas
#include <stdio.h>
#include <math.h>

#define N 1000000000

//función global de leibniz
__global__ void pi_elem(float *values)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    values[i] = pow((float)-1,(float)i);
    values[i] /= (2*i+1);
}

int main(int argc, char *argv[]) {
    float pi = 0;
    int i;
    float *d_values, *h_values;

    cudaMalloc((void**)&d_values,N*sizeof(float));
    h_values = (float*)malloc(N*sizeof(float));

    pi_elem <<< N/32, 32 >>>(d_values);

    cudaMemcpy(h_values,d_values,N*sizeof(float), cudaMemcpyDeviceToHost);

    printf("%f\n", h_values[0]);
    for (i = 0; i < N; i++) {
        pi += h_values[i];
    }
    printf("%f\n", 4*pi);
    return 0;
}
