
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define WORKERS 4

pthread_mutex_t trava;

int tamanho=0, completos[100], resultados[100];
unsigned long int vetor[100];

int primo(unsigned long int vetor) {   //funcao que verifica numero primo
  unsigned long int k=0;
  int achei, contador=0;

 if(vetor==0 || vetor==1){achei=0;}
 else{
	for(k=1;k<=vetor;k++){
		if(vetor%k==0) contador++;
	}
	if(contador>2) achei=0;
	else achei=1;
 }
return achei;
}


void* worker(void *arg) {
  int *N = (int*)(arg);
  int M = (*N);
  //printf("Iniciando thread %d\n", M);
  int instancia=0;
  while(1) {

    pthread_mutex_lock(&trava);
    while( (completos[instancia]!=0) && (instancia < tamanho)) {
      instancia++;
    }

    if(instancia >= tamanho) {
      pthread_mutex_unlock(&trava);
      break;
    }

    completos[instancia]=1;
    pthread_mutex_unlock(&trava);

    resultados[instancia] = primo(vetor[instancia]);
    //xprintf("Primo(%ld) = %d\n", vetor[instancia], resultados[instancia]);
  }
}


int main(int argc, char **argv) {
  pthread_t workers[WORKERS];
  int thread_id[WORKERS];
  int i=0, soma=0;
  char temp;

 /*passando vetor de entrada*/
 while(temp != '\n'){
   scanf("%ld%c", &vetor[i], &temp);
   i++;
   tamanho++;
  }
 
 /*inicializando completos*/
 for(i=0; i<tamanho; i++){
   completos[i]=0;
}

 /*Identificadores de thread */
  for(i=0; i<WORKERS; i++){
    thread_id[i] = i;
  }

 /* Disparando thread */
  for(i=0; i<WORKERS; i++){
    pthread_create(&(workers[i]), NULL, worker, (void*) (&thread_id[i]));
  }

 /*Esperando thread */
  for(i=0; i<WORKERS; i++){
   pthread_join(workers[i], NULL);
  }

  for(i=0; i< tamanho; i++) {
   soma = soma + resultados[i];
  }

  printf("%d\n", soma);

  return 0;

}
