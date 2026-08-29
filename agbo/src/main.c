#include <stdio.h>
#include <stdlib.h>
#define MAX_TAMANHO_ENTRADA 100
#define MAX_NOME

int verificaEntrada(char args[],char argumento[]){
    char *endptr;
    int numConvertido = strtol(args,&endptr,10);
    if(*endptr != "\0"){
        fprintf(stderr,"ERRO: argumento %s escrito errado", argumento);
        exit(1);
    }

    if(numConvertido <= 0){
        fprintf(stderr,"ERRO: argumento %s precisa ter um tamanho maior",argumento);
        exit(1);
    }

    return numConvertido;
}



int main(int argc, char *argv[]) {

    if(argc != 5){
        fprintf(stderr,"quantidade de argumentos invalida");
        exit(1);
    }
    
    return 0;
}