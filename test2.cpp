#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// EQUAZIONE DEL TIPO:   du/dt = -c * du/dx

int main()  {

    const int N = 200;
    const int M = 2000;
    const double pi = 3.14159265358979323846;
    //float XX[N], tt[M];
    double ff[M][N];
    const double L = 2*pi, dx= L / N;
    const double T = 10.0, dt= T / M;

    for(int j=0; j<N; j++){                 // carico inizialmente la condizione iniziale u(x, t=0)
        ff[0][j] = sin( 2* j * dx ) +  sin( 10 * j * dx ) / 8 ;
    }

    /* questa è la nostra funzione al tempo t=0 per ogni x
     andiamo a calcolare l'evoluzione temporale con 
     forward time e centered space*/
    
    for (int k = 1; k < M-1; k++){

        
        for (int i = 1; i < N-1; i++){
            // Ho spostato la derivata direttamente qui dentro
            double ksucc = (ff[k-1][i+1] - ff[k-1][i-1]) / (2 * dx);

            ff[k][i] = ff[k-1][i] + dt*ksucc; 
        }
        
        // condizioni al contorno
        ff[k][0] = ff[k-1][0] + dt * ( ff[k-1][1] - ff[k-1][N-1] ) / ( 2 * dx );               
        ff[k][N-1] = ff[k-1][N-1] + dt * ( ff[k-1][0] - ff[k-1][N-2] ) / ( 2 * dx );
    }
 
    // SALVATAGGIO SU FILE PER PYTHON
    ofstream file("output.txt");
    if (!file) {
        cout << "Errore apertura file!\n";
        return 1;
    }

    for (int k = 0; k <= M; ++k) {
        for (int i = 0; i < N; ++i) {
            file << ff[k][i] << "\n";
        }
        file << "\n";   
    }

    file.close();
    cout << "File output.txt generato!\n";

    return 0;
}