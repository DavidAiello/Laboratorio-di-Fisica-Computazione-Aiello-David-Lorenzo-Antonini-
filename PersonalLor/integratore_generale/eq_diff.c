#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define G (9.81)
#define SQ(x) ((x)*(x))

/*
  Programma per integrare un sistema meccanico
  Istruzioni: Si deve definire una funzione che prenda in input una struct Vector e resituisca
  un'altra struct Vector con  le derivate di quella in input. Inoltre si deve definire una funzione
  potenziale opportuna che resituisca l'energia potenziale del sistema ad ogni "istante"
  A queste vanno associati i puntatori opportuni che si trovano sotto.
*/


typedef struct {
  double x;
  double v;
  double a;
} Vector;

typedef struct {
  double m;
  unsigned long long int nsteps;
  double dt;
  int stride;
  //Inserire le costanti del sistema in esame
} System;

typedef Vector (*f) (Vector *, System*);//Punta alla funzione che fa le derivate
typedef void (*integratore) (Vector *, System *, f);//Punta alla funzine che fa l'integrazione
typedef double (*potenziale) (Vector *, System *);//Punta alla funzione che definisce il potenziale

//Funzioni----------------------------------------------------------------------------------------------------------------------------

//Integratori
void eulero(Vector *, System *, f);
void eulero_cromer(Vector *, System *, f);
void velocity_verlet(Vector *, System *, f);
void rk2(Vector *, System *, f);
void rk4(Vector *, System *, f);

//Funzioni ausiliarie
//Fisiche
double energia(Vector *, System *, potenziale);

//Validazione
void validazione(System *, int);

//MAIN--------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char * argv[]){
  if(argc != 8) {
    fprintf(stderr, "Usage %s metodo dt nsteps stride x(0) v(0) sist\n", argv[0]);
    exit(1);
  }
  
  //Questa parte è uguale in ogni sistema
  Vector p;
  System s;
  int metodo = atoi(argv[1]);
  s.dt = atof(argv[2]);
  s.nsteps = atoi(argv[3]);
  s.stride = atoi(argv[4]);

  //Inizializzazione elementi dell'array al tempo 0
  p.x = atof(argv[5]);
  p.v = atof(argv[6]);

  //Inizializzazione dei parametri del sistema

  //Controllo dell'input
  validazione(&s, metodo);
  
  
  //Inizializzare le variabili alle funzioi opportune
  f derivata;
  potenziale pot;
  
  //Scelta del metodo
  integratore passo;
  if(metodo == 1){
    passo = eulero;
    
  } else if (metodo == 2) {
    passo = eulero_cromer;
    
  } else if (metodo == 3) {
    passo = velocity_verlet;  

  }else if (metodo == 4){
    passo = rk2;
    
  } else if (metodo == 5) {
    passo = rk4;
  }

  //Integrazione numerica
  for(int i = 0; i<s.nsteps; i++){

    //Stampa del passo i-simo
    if(i % s.stride == 0) {
      fprintf(stdout,"%lf %lf %lf %lf \n",s.dt*i, p.x, p.v, energia(&p,&s,pot));
    }

    //Ottenimento del passo (i+1)-esimo
    passo(&p,&s, derivata);
  }
  
  //Stampaggio finale
  fprintf(stdout,"%lf %lf %lf %lf\n",s.dt*s.nsteps,p.x ,p.v, energia(&p,&s, pot));	
}

//Corpo delle funzioni----------------------------------------------------------------------------------------------------------------

void validazione(System *s, int metodo){
  if(metodo != 1 && metodo != 2 && metodo !=3 && metodo != 4 && metodo != 5){
    fprintf(stderr, "Usage: 1 = Eulero , 2 = Eulero-Cromer, 3 = Velocity Verlet, 4 = Runge kutta 4, 5 = Runge kutta 5\n");
    exit(1);
  }
  else if(s->dt <= 0 ){
    fprintf(stderr,"dt deve essere positivo\n");
    exit(1);
  } 
  else if(s->nsteps <= 0){
    fprintf(stderr,"nsteps deve essere positivo\n");
    exit(1); 
  }
}

void eulero(Vector *p, System *s, f derivata){
  Vector tmp;
  tmp = derivata(p,s);
  p->v += tmp.x * s->dt;
  p->x += tmp.v * s->dt;
}

void eulero_cromer(Vector *p, System *s, f derivata){
  Vector tmp;
  tmp = derivata(p,s);
  p->v += tmp.v * s->dt;
  tmp = derivata(p,s);
  p->x += tmp.x * s->dt;
}


void velocity_verlet(Vector *p, System *s, f derivata){
  Vector tmp;
  
  //Mezzo passo per v
  p->v += 0.5 * p->a * s->dt;

  //Passo completo per x
  tmp = derivata(p,s);
  p->x += tmp.x * s->dt;

  //Calcolo della nuova accelerazione (e suo salvataggio per dopo)
  tmp = derivata(p,s);
  p->a = tmp.v;

  //Mezzo passo per v
  p->v += 0.5 * p->a * s->dt;
  
}


void rk2(Vector *p, System *s, f derivata) {
  Vector tmp;
  tmp = derivata(p,s);

  //Mezzo passo con eulero
  tmp.x = p->x + tmp.x * s->dt * 0.5;
  tmp.v = p->v + tmp.v * s->dt * 0.5;

  //Passo completo con la nuova stima della derivata
  tmp = derivata(&tmp, s);
  p->x += tmp.x * s->dt;
  p->v += tmp.v * s->dt;
}

void rk4(Vector *p, System *s, f derivata){
  Vector tmp;
  Vector K[4];

  //Derivata all'inizio dell'intervallo
  K[0] = derivata(p, s);

  //Aggiornamento di mezzo passo
  tmp.x = p->x + K[0].x * s->dt * 0.5;
  tmp.v = p->v + K[0].v * s->dt * 0.5;

  //Prima derivata al centro dell'intervallo
  K[1] = derivata(&tmp,s);
  
  //Altro aggiornamento di mezzo passo
  tmp.x = p->x + K[1].x * s->dt * 0.5;
  tmp.v = p->v + K[1].v * s->dt * 0.5;

  //Seconda derivata al centro dell'intervallo
  K[2] = derivata(&tmp, s);

  //Aggiornamento di un passo intero
  tmp.x = p->x + K[2].x * s->dt * 0.5;
  tmp.v = p->v + K[2].v * s->dt * 0.5;

  //Derivata alla fine dell'intervallo
  K[3] = derivata(&tmp, s);

  //Aggiornamento finale
  p->x += ( 1 / 6.0 ) * (K[0].x + 2*K[1].x + 2*K[2].x + K[3].x);
  p->v += ( 1 / 6.0) *  (K[0].v + 2*K[1].v + 2*K[2].v + K[3].v);
}

double energia(Vector *p, System *s, potenziale pot){
  double K = 0.5 * s->m * SQ(p->v);
  double V = pot(p,s);//Mettere una funzione per l'energia potenziale
  return K+V;
}

//Funzioni del sistema----------------------------------------------------------------------------------------------------------------
