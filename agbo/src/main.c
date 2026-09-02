#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define MAX_TAMANHO_ENTRADA 100
#define MAX_NOME

typedef struct thread{
    int *matriz;
    int largura;
    int altura;
    int maxIteracoes;
    int linhaInicial;
    int linhaFinal;
    int numThreads;
    double xMenor;
    double yMenor;
    double larguraPixel;
    double alturaPixel;
}thread;

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

void* execThread2(void* arg){
    thread *temp = (thread*)arg;

    for(int i = temp->linhaInicial; i < temp->altura; i = i + temp->numThreads ){
        for(int j = 0; j < temp->largura; j++){
            double numComplexoReal = temp->xMenor + j * temp->larguraPixel;
            double numComplexoImag = temp->yMenor + i * temp->alturaPixel;
            int iteracoes = mandelbrot(temp->maxIteracoes,numComplexoReal,numComplexoImag);
            int indice = i*temp->largura + j;
            temp->matriz[indice] = iteracoes/(double)temp->maxIteracoes * 255;

        }
    }
    return NULL;
}

void* execThread(void* arg){
    thread *temp = (thread*)arg;

    for(int i = temp->linhaInicial; i < temp->linhaFinal; i++ ){
        for(int j = 0; j < temp->largura; j++){
            double numComplexoReal = temp->xMenor + j * temp->larguraPixel;
            double numComplexoImag = temp->yMenor + i * temp->alturaPixel;
            int iteracoes = mandelbrot(temp->maxIteracoes,numComplexoReal,numComplexoImag);
            int indice = i*temp->largura + j;
            temp->matriz[indice] = iteracoes/(double)temp->maxIteracoes * 255;

        }
    }
    return NULL;
}


int verificaEntrada(char args[],char argumento[],FILE *filerro){

    char *endptr;
    int numConvertido = strtol(args,&endptr,10);
    if(*endptr != '\0'){
        fprintf(filerro,"ERRO: argumento %s escrito errado", argumento);
        exit(1);
    }

    if(numConvertido <= 0){
        fprintf(filerro,"ERRO: argumento %s precisa ter um tamanho maior",argumento);
        exit(1);
    }

    return numConvertido;
}


int main(int argc, char *argv[]) {
    //arquivo de erros
    FILE* filerro = NULL;
    filerro = fopen("erros.txt","w");
    if(filerro == NULL){
        exit(1);
    }
    //./mandelbrot <altura> <largura> <maxIteracoes> <numThreads>
    if(argc != 5){
        fprintf(filerro,"quantidade de argumentos invalida");
        exit(1);
    }

    int largura = verificaEntrada(argv[1],"largura",filerro);
    int altura = verificaEntrada(argv[2],"altura",filerro);
    int maxIteracoes = verificaEntrada(argv[3],"maxIteracoes",filerro);
    int numThreads = verificaEntrada(argv[4],"numThreads",filerro);

    int *matriz = (int*)malloc(sizeof(int) * altura * largura);
    if(matriz == NULL){
        fprintf(filerro,"ERRO: erro no malloc");
        exit(1);
    }

    double xMenor =  -2;
    double xMaior =  1;
    double yMenor =  -1.5;
    double yMaior =  1.5;

    double larguraPixel = (xMaior - xMenor)/largura;
    double alturaPixel = (yMaior - yMenor)/altura;

    struct timespec inicio, fim;
    //serial
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

    FILE *timesfileserial = fopen("times.txt","w");
    if(timesfileserial == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(timesfileserial, "serial: %f\n", tempo);
    fclose(timesfileserial);

    FILE *arquivoserial = fopen("mandelbrot_agbo_serial.pgm","w");
    if(arquivoserial == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            fprintf(arquivoserial,"%d ",matriz[i*largura + j]);
        }
        fprintf(arquivoserial,"\n");
    }

    fclose(arquivoserial);

    //fim do serial
    //openmp
    
    clock_gettime(CLOCK_MONOTONIC,&inicio);
    
    #pragma omp parallel for num_threads(numThreads)
    
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

    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) * 0.000000001;

    FILE *timesfileopenmp = fopen("times.txt","a");
    if(timesfileopenmp == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(timesfileopenmp, "openmp: %f\n", tempo);
    fclose(timesfileopenmp);

    FILE *arquivoopenmp = fopen("mandelbrot_agbo_openmp.pgm","w");
    if(arquivoopenmp == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            fprintf(arquivoopenmp,"%d ",matriz[i*largura + j]);
        }
        fprintf(arquivoopenmp,"\n");
    }

    fclose(arquivoopenmp);
    //fim openmp
    //inicio pthread1

    pthread_t *ids = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    thread *threads = (thread*)malloc(sizeof(thread) * numThreads);

    if(ids == NULL){
        fprintf(filerro,"ERRO: erro ao alocar memoria para os ids dos Threads");
        exit(1);
    }

    if(threads == NULL){
        fprintf(filerro,"ERRO: erro ao alocar memoria para o vetor da struct de threads");
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC,&inicio);
    
    for(int t = 0; t < numThreads; t++){
        threads[t].altura = altura;
        threads[t].largura = largura;
        threads[t].xMenor = xMenor;
        threads[t].yMenor = yMenor;
        threads[t].maxIteracoes = maxIteracoes;
        threads[t].larguraPixel = larguraPixel;
        threads[t].alturaPixel = alturaPixel;
        threads[t].linhaInicial = altura/numThreads * t;
        threads[t].linhaFinal = altura/numThreads * (t+1);
        threads[t].matriz = matriz;
        pthread_create(&ids[t],NULL,execThread,&threads[t]);

    }

    for(int t = 0; t < numThreads; t++){
        pthread_join(ids[t],NULL);

    }

    clock_gettime(CLOCK_MONOTONIC,&fim);

    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) * 0.000000001;

    FILE *timesfilepthreads1 = fopen("times.txt","a");
    if(timesfilepthreads1 == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(timesfilepthreads1, "pthreads1: %f\n", tempo);
    fclose(timesfilepthreads1);

    free(ids);
    free(threads);

    FILE *arquivopthreads1 = fopen("mandelbrot_agbo_pthreads1.pgm","w");
    if(arquivopthreads1 == NULL){
        fprintf(stderr,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            fprintf(arquivopthreads1,"%d ",matriz[i*largura + j]);
        }
        fprintf(arquivopthreads1,"\n");
    }

    fclose(arquivopthreads1);
    //fim pthread1
    //iniciopthread2
    
    ids = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    threads = (thread*)malloc(sizeof(thread) * numThreads);

    if(ids == NULL){
        fprintf(stderr,"ERRO: erro ao alocar memoria para os ids dos Threads");
        exit(1);
    }

    if(threads == NULL){
        fprintf(filerro,"ERRO: erro ao alocar memoria para o vetor da struct de threads");
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC,&inicio);
    
    for(int t = 0; t < numThreads; t++){
        threads[t].numThreads = numThreads;
        threads[t].altura = altura;
        threads[t].largura = largura;
        threads[t].xMenor = xMenor;
        threads[t].yMenor = yMenor;
        threads[t].maxIteracoes = maxIteracoes;
        threads[t].larguraPixel = larguraPixel;
        threads[t].alturaPixel = alturaPixel;
        threads[t].linhaInicial = t;
        threads[t].linhaFinal = altura/numThreads * (t+1);
        threads[t].matriz = matriz;
        pthread_create(&ids[t],NULL,execThread2,&threads[t]);

    }

    for(int t = 0; t < numThreads; t++){
        pthread_join(ids[t],NULL);

    }

    clock_gettime(CLOCK_MONOTONIC,&fim);

    tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) * 0.000000001;

    FILE *timesfilepthreads2 = fopen("times.txt","a");
    if(timesfilepthreads2 == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(timesfilepthreads2, "pthreads2: %f\n", tempo);
    fclose(timesfilepthreads2);

    free(ids);
    free(threads);

    FILE *arquivopthreads2 = fopen("mandelbrot_agbo_pthreads2.pgm","w");
    if(arquivopthreads2 == NULL){
        fprintf(filerro,"ERRO: erro ao abrir o arquivo");
        exit(1);
    }

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            fprintf(arquivopthreads2,"%d ",matriz[i*largura + j]);
        }
        fprintf(arquivopthreads2,"\n");
    }

    fclose(arquivopthreads2);

    //fimpthread2
    fclose(filerro);
    return 0;
}