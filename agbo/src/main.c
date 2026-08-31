#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_TAMANHO_ENTRADA 100
#define MAX_NOME


int verificaEntrada(char args[],char argumento[]){
    char *endptr;
    int numConvertido = strtol(args,&endptr,10);
    if(*endptr != '\0'){
        fprintf(stderr,"ERRO: argumento %s escrito errado", argumento);
        exit(1);
    }

    if(numConvertido <= 0){
        fprintf(stderr,"ERRO: argumento %s precisa ter um tamanho maior",argumento);
        exit(1);
    }

    return numConvertido;
}

int mandelbrot(int maxIteracoes,double numComplexoReal, double numComplexoImag){
    double zReal = 0;
    double zImag = 0;
    int qtdIteracoes = 0;
    for(int k = 0; k < maxIteracoes; k++){

        if(zImag*zImag + zReal*zReal > 4){
            return k;
        }
        double temp = zReal;
        double novozReal = temp*temp - zImag*zImag + numComplexoReal;
        zReal = novozReal;
        zImag = 2 * temp * zImag + numComplexoImag;
        qtdIteracoes++;
    }
    return qtdIteracoes;
}



int main(int argc, char *argv[]) {
    //./mandelbrot <altura> <largura> <maxIteracoes> <maxThreads>
    if(argc != 5){
        fprintf(stderr,"quantidade de argumentos invalida");
        exit(1);
    }

    int altura = verificaEntrada(argv[1],"altura");
    int largura = verificaEntrada(argv[2],"largura");
    int maxIteracoes = verificaEntrada(argv[3],"maxIteracoes");
    int maxThreads = verificaEntrada(argv[4],"maxThreads");

    int *matriz = (int*)malloc(sizeof(int) * altura * largura);
    if(matriz == NULL){
        fprintf(stderr,"ERRO: erro no malloc");
    }

    double xMenor =  -2;
    double xMaior =  1;
    double yMenor =  -1.5;
    double yMaior =  1.5;

    double larguraPixel = (xMaior - xMenor)/largura;
    double alturaPixel = (yMaior - yMenor)/altura;

    struct timespec inicio, fim;
    
    clock_gettime(CLOCK_MONOTONIC,&inicio);
    for(int i = 0; i < altura; i++){//py
        for(int j = 0; j < largura; j++){//px
            double numComplexoReal = xMenor + j * larguraPixel;
            double numComplexoImag = yMenor + i * alturaPixel;
            int iteracoes = mandelbrot(maxIteracoes,numComplexoReal,numComplexoImag);
            int indice = i*largura + j;
            matriz[indice] = iteracoes/(double)maxIteracoes * 255;
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) * 0.000000001;

    FILE *timesfile = fopen("times.txt","a");
    if(timesfile == NULL){
        fprintf(stderr,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(timesfile, "serial: %f\n", tempo);
    fclose(timesfile);

    FILE *arquivo = fopen("mandelbrot_agbo_serial.pgm","w");
    if(arquivo == NULL){
        fprintf(stderr,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            fprintf(arquivo,"%d ",matriz[i*largura + j]);
        }
        fprintf(arquivo,"\n");
    }

    fclose(arquivo);

    for(int i = 0; i < altura; i++){
        for(int j = 0; j<largura; j++){
            printf("%d ",matriz[i*largura + j]);
        }
        printf("\n");
    }
    
    return 0;
}