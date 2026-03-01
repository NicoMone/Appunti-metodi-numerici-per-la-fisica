#include <stdio.h>

// Queste le funzioni che andremo a definire a seguito del MAIN
double POWER(double numero,double elevazione); // la funzione da integrare: legge di potenza
double funzione(double varF); // routine che presa una x restituisce la y=f(x)
double areaTrapezio(double lato1,double lato2,double altezza); // routine che calcola l'area di un singolo trapezio
double trapezioIntegral(double a,double b,int colonne); // integrazione con metodo dei trapezi
double simpsonIntegral(double a,double b,int colonne); // integrazione con Simpson

int main(int argc,char**argv){
	// main: qui si chiama TrapezioIntegral per fare un'integrazione da a=1 a b=2
	// con l'intervallo diviso in 5000 parti uguali
	printf("[ trapezi: %lf ]\n", trapezioIntegral(1,2,5000));
	printf("[ simpson  %lf ]\n", simpsonIntegral(1,2,5000));
	}
	
double POWER(double numero,int elevazione){
	double ret = 1;
	for(int i=elevazione;i>0;i--){
		ret *= numero;
		}
	return ret;
	}
double funzione(double varF){
	double y = POWER(varF,2);
	return y;
	}

double areaTrapezio(double lato1,double lato2,double altezza){
	return (lato1+lato2)*altezza/2;
	}

double trapezioIntegral(double a,double b,int colonne){
	double area = 0;
	double nPartiUguali = (b-a)/colonne;

	for(int i=0;i<colonne-1;i++){
		area += areaTrapezio(funzione(a),funzione(a+nPartiUguali),nPartiUguali);
		a += nPartiUguali;
		}
		area += areaTrapezio(funzione(a),funzione(b),b-a);
		return area;
	}

	// Simpson composito su [a,b] con "colonne" sottointervalli (deve essere pari)
double simpsonIntegral(double a,double b,int colonne){
    if(colonne < 2) colonne = 2;
    if(colonne % 2 != 0) colonne -= 1;   // Simpson richiede un numero pari di sottointervalli

    double h = (b-a)/colonne;
    double somma = funzione(a) + funzione(b);

    // termini con coefficiente 4: i dispari
    for(int i=1; i<=colonne-1; i+=2){
        double x = a + i*h;
        somma += 4.0 * funzione(x);
    }

    // termini con coefficiente 2: i pari (esclusi estremi)
    for(int i=2; i<=colonne-2; i+=2){
        double x = a + i*h;
        somma += 2.0 * funzione(x);
    }

    return (h/3.0) * somma;
}