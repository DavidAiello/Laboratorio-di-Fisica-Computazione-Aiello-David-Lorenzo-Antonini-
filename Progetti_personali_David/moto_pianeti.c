#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define G (4.0 * M_PI * M_PI) // Costante gravitazionale in UA^3 / (M_sole * yr^2)
#define M 1.0                 // Massa del Sole in masse solari
// Qual è un modo efficiente per scalare le unità di misura?
typedef struct{
  double r[2];
  double v[2];
  double a[2];
  double m;
}Pianeta;

typedef double (*accelerazione) (double,double);
typedef void (*integratore) (Pianeta *p, accelerazione a, double dt);


double acc(double,double);

double E(Pianeta *  );

double E_interazione(Pianeta *  );

void periodo(Pianeta *, double *, double *); // Tx e Ty

void v_v(Pianeta *, accelerazione a, double dt);

int main(int argc, char *argv[]){
  accelerazione a;
  FILE *fp; // per la scrittura su file
  integratore passo;
  a = acc;
  passo = v_v;
  //Controllo argv_argc
  if (argc != 4) {
    fprintf(stderr, "Uso: %s N_pianeti dt nsteps \n", argv[0]);
    return 1;
  }
  

  //printf("%lf\n",casuale);
  
  //Dichiarazioni variabili
  int N = atoi(argv[1]);
  double dt = atof(argv[2]);
  int nsteps = atoi(argv[3]);

  //Dichiarazione array di strutture 
  Pianeta *pianeti = calloc(N, sizeof(Pianeta));
  char *filename = malloc(N * sizeof(char));
  //Dichiarazione condizioni iniziali pianeta
  // 0: ellisse interna, 1: cerchio, 2: parabola, 3: iperbole
  double alpha[] = { 1.0, sqrt(2.0), 1.6,0.7};
  
 for (int i = 0; i < N; i++) {
    // Distanza dal Sole crescente: es. 1.0 UA, 2.0 UA, ecc.
    double r_iniziale = 1.0 + i * 0.3; 
    double v_circ = sqrt((G * M) / r_iniziale);
    
    pianeti[i].m = 3e-6; // Massa tipica di un pianeta roccioso (es. Terra ~ 3 * 10^-6 M_sole)
    
    // Posizione iniziale sull'asse x
    pianeti[i].r[0] = r_iniziale;
    pianeti[i].r[1] = 0;

    
    pianeti[i].v[0] = 0.0 ;
    pianeti[i].v[1] = v_circ * 0.7;

    // Accelerazione iniziale calcolata subito nella posizione di partenza
    pianeti[i].a[0] = a(pianeti[i].r[0], pianeti[i].r[1]);
    pianeti[i].a[1] = a(pianeti[i].r[1], pianeti[i].r[0]);
}

  // Controllo per l'array di strutture
  if(pianeti == NULL) {
    fprintf(stderr, "Impossibile allocare la memoria\n");
    return 1;
  }
  
  //Inizio algoritmo di integrazione
  
  for(int t = 0; t < N; t++){
    sprintf(filename,"pianeti%i.dat",t);
    fp = fopen(filename,"w");
    for(int k = 0; k < nsteps; k++){
      fprintf(fp,"%lf %lf %lf %lf %lf\n",k*dt, pianeti[t].r[0], pianeti[t].r[1], pianeti[t].v[0],pianeti[t].v[1]);
      passo((pianeti+t),a,dt);
    }
    fclose(fp);
  }
  
  
  free(pianeti);
  free(filename);
}

//Velocity Verlet
void v_v(Pianeta *p , accelerazione a, double dt){
  // 0 = componente x; 1 = componente y;
    p->v[0] += 0.5 * p->a[0] * dt;
    p->v[1] += 0.5 * p->a[1] * dt;
    p->r[0] += p->v[0] * dt;
    p->r[1] += p->v[1] * dt;
    p->a[0] = a(p->r[0],p->r[1]);
    p->a[1] = a(p->r[1],p->r[0]);
    p->v[0] += 0.5 * p->a[0] * dt;
    p->v[1] += 0.5 * p->a[1] * dt;
}

double acc(double c1, double c2){ //Componente 1 e 2
  return - ((G*M*c1) / pow((sqrt(pow(c1,2) + pow(c2,2))),3));
}
