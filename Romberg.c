#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPS  1.0e-6f   // accuratezza relativa desiderata
#define JMAX 20        // massimo numero di raffinamenti (livelli trapezio)
#define K    5         // punti usati nell’estrapolazione (Romberg "ordine" ~ 2K)

// Prototipi: definiscili da qualche parte nel tuo progetto
float trapzd(float (*func)(float), float a, float b, int j);
void  polint(const float xa[], const float ya[], int n, float x, float *y, float *dy);

static void nrerror(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

// Romberg via: trapezi successivi + estrapolazione polinomiale in h^2 verso 0
float qromb(float (*func)(float), float a, float b)
{
    float s[JMAX];      // s[j] = stima trapezoidale al livello j (0-based)
    float h[JMAX];      // h[j] = variabile proporzionale a (passo)^2
    float ss = 0.0f;    // stima extrapolata dell’integrale (valore a h^2=0)
    float dss = 0.0f;   // stima errore dell’estrapolazione

    // h[0] è solo una scala: conta il rapporto tra livelli.
    // Ad ogni raffinamento, il passo h_geom dimezza => (passo)^2 /4.
    h[0] = 1.0f;

    for (int j = 0; j < JMAX; ++j) {

        // trapezio al livello (j+1) per restare compatibili con NR
        s[j] = trapzd(func, a, b, j + 1);

        // quando ho almeno K valori, estrapolo in h^2 fino a 0
        if (j + 1 >= K) {
            // uso gli ultimi K punti: h[j-K+1 .. j] e s[j-K+1 .. j]
            polint(&h[j - K + 1], &s[j - K + 1], K, 0.0f, &ss, &dss);

            // criterio di arresto: errore stimato relativo sotto EPS
            if (fabsf(dss) <= EPS * fabsf(ss)) {
                return ss;
            }
        }

        // preparo la "x" per il prossimo livello: h^2 scala di 1/4 quando il passo scala di 1/2
        if (j + 1 < JMAX) {
            h[j + 1] = 0.25f * h[j];
        }
    }

    nrerror("Too many steps in routine qromb");
    return 0.0f; // non ci arriva mai
}