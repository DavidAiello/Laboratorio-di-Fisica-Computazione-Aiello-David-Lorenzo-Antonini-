#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
  double x;
  double v;
}Punto; 

typedef struct{
  double k,m,dt,tmax;
  int n_passi;
}Sistema;

void inserimento(Punto *, Sistema *, int, char **);

void Eulero(Punto *, Sistema *);

void Eulero_Cromer(Punto *, Sistema *);

int main(int argc, char *argv[]){
  Punto punto;
  Sistema sistema;
  inserimento(&punto,&sistema, argc, argv);
  return 0;
}

void inserimento(Punto *punto, Sistema *sistema, int n, char *d[]){
  punto -> x = atof(d[1]);
  punto -> v = atof(d[2]);
  sistema -> k = atof(d[3]);
  sistema -> m = atof(d[4]);
  sistema -> dt = atof(d[5]);
  sistema -> tmax = atof(d[6]);
  sistema -> n_passi = (int) (sistema -> tmax / (double)sistema -> dt);
  fprintf(stderr,"Sono stati inseriti i seguenti valori:\n x = %lf v = %lf k = %lf m = %lf dt = %lf t_max = %lf\n", punto->x, punto->v,sistema->k,sistema->m,sistema->dt,sistema->tmax);
}
