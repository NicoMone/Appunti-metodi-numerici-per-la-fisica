#include <iostream>
#include <fstream>
#include <cmath>

// EQUAZIONE DEL TIPO:   du/dt + c * du/dx = d^2u/dx^2

using namespace std;
const int N = 50;

// Derivata prima / differenze finite / condizioni al contorno periodiche
inline double derivata_prima(double ff[][N], int k, int i, double dx, int N){
    int ip = (i + 1) % N;          
    int im = (i - 1 + N) % N;      

    return (ff[k][ip] - ff[k][im]) / (2.0 * dx);
}

// Derivata seconda / differenze finite / condizioni al contorno periodiche
inline double derivata_seconda(double ff[][N], int k, int i, double dx, int N){
    int ip = (i + 1) % N;
    int im = (i - 1 + N) % N;

    return (ff[k][ip] - 2.0 * ff[k][i] + ff[k][im]) / (dx * dx);
}

// Avanzamento in avanti nel tempo
inline double forward_time(double u, double dudx, double dt, double c ){
    return u + dt * dudx;
}// il segno meno per il termine du/dx è stato incluso nella chiamata della funzione

int main()  {
    const int M = 1000;
    const double pi = 3.14159265358979323846;
    
    static double ff[M][N];
    const double L = 2*pi, dx= L / N;
    const double T = 10, dt= T / M;
    double v = 0.008; /*viscosità cinematica*/

    for(int j=0; j<N; j++){                 
        ff[0][j] = sin( 2* j * dx );
    }
    
    for (int k = 1; k < M; k++){
        
        for (int i = 0; i < N; i++){
            double ksucc = (-derivata_prima(ff, k-1, i, dx, N) + v*derivata_seconda(ff, k-1, i, dx, N));

            ff[k][i] = forward_time(ff[k-1][i], ksucc, dt, 1.0);
        }
        
    }
 
    // SALVATAGGIO SU FILE PER PYTHON
    ofstream file("output.txt");
    if (!file) {
        cout << "Errore apertura file!\n";
        return 1;
    }

    for (int k = 0; k < M; ++k) {
        for (int i = 0; i < N; ++i) {
            file << ff[k][i] << "\n";
        }
        file << "\n";   
    }

    file.close();
    cout << "File output.txt generato!\n";

    return 0;
}