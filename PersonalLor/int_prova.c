#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
File di prova per provare un'integrazione con il metodo di Eulero ed Eulero-Cromer,
userò la semplice equzione della caduta libera di un corpo
*/

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Error, usage %s x0(float) v0(float) tf(float) N(int)\n", argv[0]);
        exit(1);
    }

    double x = atof(argv[1]);
    double v = atof(argv[2]);
    double tf = atof(argv[3]);
    long long int N = atoi(argv[4]);
    double dt = tf/N;
    double a = 0;
    FILE *fp;
    fp = fopen("FreeFall.dat", "w+");
    fprintf(fp, "\"x(t)\",\"v(t)\"\n");

    for (int i = 0; i < N; i++) {
        a = 9.81;
        v += a*dt;
        x += v*dt;
        fprintf(fp, "%f,%f\n", x, v);
    }

    fclose(fp);
}